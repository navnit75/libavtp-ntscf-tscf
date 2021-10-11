
#include <assert.h>
#include <argp.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <poll.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <inttypes.h>

#include "avtp.h"
#include "avtp_tscf.h"
#include "can.h"
#include "common.h"


#define STREAM_ID 			0x1232454585957584
#define CAN_PDU_SIZE		(sizeof(struct can_pdu) + 8)
#define PAYLOAD_LEN         8
#define NO_OF_ACF_MESSAGE			1
#define DATA_LEN			CAN_PDU_SIZE * NO_OF_ACF_MESSAGE
#define PDU_SIZE			(sizeof(struct avtp_stream_pdu) + DATA_LEN)


// SAMPLE Queue entry--> Linked list
// But the data here is PCM
struct sample_entry {
    STAILQ_ENTRY(sample_entry) entries;

    struct timespec tspec;

    //Saving the whole of the acf message itself
    uint8_t pdu[DATA_LEN];
};

// Intialization of Head
static STAILQ_HEAD(sample_queue, sample_entry) samples;

// interface name handling
static char ifname[IFNAMSIZ];

// Mac address handling
static uint8_t macaddr[ETH_ALEN];

// Sequence number handling
static uint8_t expected_seq;

// ARGUMENT PARSER
static struct argp_option options[] = {
        {"dst-addr", 'd', "MACADDR", 0, "Stream Destination MAC address" },
        {"ifname", 'i', "IFNAME", 0, "Network Interface" },
        { 0 }
};

// ERROR in argument providing
static error_t parser(int key, char *arg, struct argp_state *state)
{
    int res;

    switch (key) {
        case 'd':
            res = sscanf(arg, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                         &macaddr[0], &macaddr[1], &macaddr[2],
                         &macaddr[3], &macaddr[4], &macaddr[5]);
            if (res != 6) {
                fprintf(stderr, "Invalid address\n");
                exit(EXIT_FAILURE);
            }

            break;
        case 'i':
            strncpy(ifname, arg, sizeof(ifname) - 1);
            break;
    }

    return 0;
}

static struct argp argp = { options, parser };



// This function is fine and not to be touched
static int schedule_sample(int fd, struct timespec *tspec, uint8_t *sample)
{
    struct sample_entry *entry;

    entry = malloc(sizeof(*entry));


    if (!entry) {
        fprintf(stderr, "Failed to allocate memory\n");
        return -1;
    }

    entry->tspec.tv_sec = tspec->tv_sec;
    entry->tspec.tv_nsec = tspec->tv_nsec;
    memcpy(entry->pdu, sample, DATA_LEN);

    STAILQ_INSERT_TAIL(&samples, entry, entries);

    /* If this was the first entry inserted onto the queue, we need to arm
     * the timer.
     */
    if (STAILQ_FIRST(&samples) == entry) {
        int res;

        res = arm_timer(fd, tspec);
        if (res < 0) {
            STAILQ_REMOVE(&samples, entry, sample_entry, entries);
            free(entry);
            return -1;
        }
    }

    return 0;
}




// Checking validity of the packet
static bool is_valid_packet(struct avtp_stream_pdu *pdu)
{

    // Recieved avtp_common_pdu
    struct avtp_common_pdu *common = (struct avtp_common_pdu *) pdu;
    uint64_t val64;
    uint32_t val32;
    int res;


    // SUBTYPE CHECK
    res = avtp_pdu_get(common, AVTP_FIELD_SUBTYPE, &val32);


    if (res < 0) {
        fprintf(stderr, "Failed to get subtype field: %d\n", res);
        return false;
    }


    if (val32 != AVTP_SUBTYPE_TSCF) {
        fprintf(stderr, "Subtype mismatch: expected %u, got %u\n",
                AVTP_SUBTYPE_AAF, val32);
        return false;
    }


    // Version check
    res = avtp_pdu_get(common, AVTP_FIELD_VERSION, &val32);
    if (res < 0) {
        fprintf(stderr, "Failed to get version field: %d\n", res);
        return false;
    }
    if (val32 != 0) {
        fprintf(stderr, "Version mismatch: expected %u, got %u\n",
                0, val32);
        return false;
    }

    // TV bit check
    res = avtp_tscf_pdu_get(pdu, AVTP_TSCF_FIELD_TV, &val64);
    if (res < 0) {
        fprintf(stderr, "Failed to get tv field: %d\n", res);
        return false;
    }
    if (val64 != 1) {
        fprintf(stderr, "tv mismatch: expected %u, got %" PRIu64 "\n",
                1, val64);
        return false;
    }

    // MR bit check
    res = avtp_tscf_pdu_get(pdu, AVTP_TSCF_FIELD_MR, &val64);
    if (res < 0) {
        fprintf(stderr, "Failed to get mv field: %d\n", res);
        return false;
    }
    if (val64 != 0) {
        fprintf(stderr, "mr mismatch: expected %u, got %" PRIu64 "\n",
                0, val64);
        return false;
    }

    // TU bit check
    res = avtp_tscf_pdu_get(pdu, AVTP_TSCF_FIELD_TU, &val64);
    if (res < 0) {
        fprintf(stderr, "Failed to get tu field: %d\n", res);
        return false;
    }
    
    if (val64 != 0) {
        fprintf(stderr, "tu mismatch: expected %u, got %" PRIu64 "\n",
                0, val64);
        return false;
    }

    // Stream id check
    res = avtp_tscf_pdu_get(pdu, AVTP_TSCF_FIELD_STREAM_ID, &val64);
    if (res < 0) {
        fprintf(stderr, "Failed to get stream ID field: %d\n", res);
        return false;
    }
    if (val64 != STREAM_ID) {
        fprintf(stderr, "Stream ID mismatch: expected %" PRIu64 ", got %" PRIu64 "\n",
                STREAM_ID, val64);
        return false;
    }

    // SEQ NUM CHECK
    res = avtp_tscf_pdu_get(pdu, AVTP_TSCF_FIELD_SEQ_NUM, &val64);
    if (res < 0) {
        fprintf(stderr, "Failed to get sequence num field: %d\n", res);
        return false;
    }

    if (val64 != expected_seq) {
        // If there is a sequence number mismatch we are not invalidating the packet

        fprintf(stderr, "Sequence number mismatch: expected %u, got %" PRIu64 "\n",
                expected_seq, val64);
        expected_seq = val64;
    }

    expected_seq++;

    // Stream data len check
    res = avtp_tscf_pdu_get(pdu, AVTP_TSCF_FIELD_STREAM_DATA_LEN, &val64);
    if (res < 0) {
        fprintf(stderr, "Failed to get data_len field: %d\n", res);
        return false;
    }
    if (val64 != DATA_LEN) {
        fprintf(stderr, "Data len mismatch: expected %lu, got %" PRIu64 "\n",
                DATA_LEN, val64);
        return false;
    }

    return true;
}

// Recieve function,
static int new_packet(int sk_fd, int timer_fd)
{
    int res;
    ssize_t n;
    uint64_t avtp_time;
    struct timespec tspec;
    struct avtp_stream_pdu *pdu = alloca(PDU_SIZE);

    memset(pdu, 0, PDU_SIZE);

    n = recv(sk_fd, pdu, PDU_SIZE, 0);
    if (n < 0 || n != PDU_SIZE) {
        perror("Failed to receive data");
        return -1;
    }

    // 1.
    if (!is_valid_packet(pdu)) {
        fprintf(stderr, "Dropping packet\n");
        return 0;
    }


    // 2.
    res = avtp_tscf_pdu_get(pdu, AVTP_TSCF_FIELD_TIMESTAMP, &avtp_time);
    if (res < 0) {
        fprintf(stderr, "Failed to get AVTP time from PDU\n");
        return -1;
    }

    // 3.
    res = get_presentation_time(avtp_time, &tspec);
    if (res < 0)
        return -1;

    // 4.
    res = schedule_sample(timer_fd, &tspec, pdu->avtp_payload);
    if (res < 0)
        return -1;

    return 0;
}

// ----YET TO UNDERSTAND-----
static int timeout(int fd)
{
    int res;
    ssize_t n;
    uint64_t expirations;
    struct sample_entry *entry;

    n = read(fd, &expirations, sizeof(uint64_t));
    if (n < 0) {
        perror("Failed to read timerfd");
        return -1;
    }

    assert(expirations == 1);

    entry = STAILQ_FIRST(&samples);
    assert(entry != NULL);

    uint8_t data_recieved[PAYLOAD_LEN];
    struct can_pdu *pdu = (struct can_pdu*)entry->pdu;
    memcpy(data_recieved,pdu->can_payload,PAYLOAD_LEN);

    res = present_data(data_recieved, PAYLOAD_LEN);

    if (res < 0)
        return -1;

    STAILQ_REMOVE_HEAD(&samples, entries);
    free(entry);

    if (!STAILQ_EMPTY(&samples)) {
        entry = STAILQ_FIRST(&samples);

        res = arm_timer(fd, &entry->tspec);
        if (res < 0)
            return -1;
    }

    return 0;
}


// Caller
int main(int argc, char *argv[])
{
    int sk_fd, timer_fd, res;
    struct pollfd fds[2];

    argp_parse(&argp, argc, argv, 0, NULL, NULL);

    STAILQ_INIT(&samples);

    sk_fd = create_listener_socket(ifname, macaddr, ETH_P_TSN);
    if (sk_fd < 0)
        return 1;

    timer_fd = timerfd_create(CLOCK_REALTIME, 0);
    if (timer_fd < 0) {
        close(sk_fd);
        return 1;
    }

    fds[0].fd = sk_fd;
    fds[0].events = POLLIN;
    fds[1].fd = timer_fd;
    fds[1].events = POLLIN;

    while (1) {
        res = poll(fds, 2, -1);
        if (res < 0) {
            perror("Failed to poll() fds");
            goto err;
        }

        if (fds[0].revents & POLLIN) {
            res = new_packet(sk_fd, timer_fd);
            if (res < 0)
                goto err;
        }

        if (fds[1].revents & POLLIN) {
            res = timeout(timer_fd);
            if (res < 0)
                goto err;
        }
    }

    return 0;

    err:
    close(sk_fd);
    close(timer_fd);
    return 1;
}

