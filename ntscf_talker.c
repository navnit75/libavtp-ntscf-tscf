#include <argp.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "avtp_ntscf.h"
#include "common.h"
#include "can.h"


#define STREAM_ID 			0x1232454585957584
#define NO_OF_ACF_MESSAGE			1
#define DATA_LEN			((sizeof(struct can_pdu) + 8) * NO_OF_ACF_MESSAGE)
#define PDU_SIZE			(sizeof(struct avtp_ntscf_stream_pdu) + DATA_LEN)
#define CAN_PDU_SIZE		(sizeof(struct can_pdu) + 8)

static char ifname[IFNAMSIZ];
static uint8_t macaddr[ETH_ALEN];
static int priority = -1;
static int max_transit_time;

static struct argp_option options[] = {
        {"dst-addr", 'd', "MACADDR", 0, "Stream Destination MAC address" },
        {"ifname", 'i', "IFNAME", 0, "Network Interface" },
        {"prio", 'p', "NUM", 0, "SO_PRIORITY to be set in socket" },
        { 0 }
};

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
        // case 'm':
        //     max_transit_time = atoi(arg);
        //     break;
        case 'p':
            priority = atoi(arg);
            break;
    }

    return 0;
}


static struct argp argp = { options, parser };

// ***** End of argument handling ******


static int init_pdu(struct avtp_ntscf_stream_pdu *pdu)
{
    int res;

    // pdu intialised with the VERSION, SUBTYPE and SV
    res = avtp_ntscf_pdu_init(pdu);
    if (res < 0)
        return -1;

    // DATA LEN field set
    res = avtp_ntscf_pdu_set(pdu, AVTP_NTSCF_FIELD_DATA_LEN,DATA_LEN);
    if (res < 0)
        return -1;

    return 0;
}


uint8_t* payload_creation(){
    uint8_t *payload;
    payload = (uint8_t*)malloc(8*sizeof(uint8_t));
    for(int i=0;i<8;i++){
        payload[i]=i;
    }
    return payload;
}

// Intialization of the CAN pdu
static int init_can_pdu(struct can_pdu * pdu){
    int res;

    res = can_pdu_init(pdu);
    if (res < 0)
        return -1;

    res = can_pdu_set(pdu, CAN_FIELD_MSG_TYPE,1);
    if( res < 0)
        return -1;

    res = can_pdu_set(pdu, CAN_FIELD_MSG_LEN,6);
    if(res < 0)
        return -1;

    res = can_pdu_set(pdu,CAN_FIELD_RTR,1);
    if(res < 0)
        return -1;

    res = can_pdu_set(pdu,CAN_FIELD_MTV,1);
    if(res < 0)
        return -1;

    res = can_pdu_set(pdu,CAN_FIELD_EFF,0);
    if(res < 0)
        return -1;

    res = can_pdu_set(pdu,CAN_FIELD_BRS,1);
    if(res < 0)
        return -1;


    res = can_pdu_set(pdu,CAN_FIELD_FDF,1);
    if(res < 0)
        return -1;

    res = can_pdu_set(pdu,CAN_FIELD_ESI,0);
    if(res < 0)
        return -1;

    res = can_pdu_set(pdu,CAN_FIELD_CAN_BUS_ID,0);
    if(res < 0)
        return -1;

    res = can_pdu_set(pdu,CAN_FIELD_MESSAGE_TIMESTAMP,0xf234567812345678);
    if(res < 0)
        return -1;

    res = can_pdu_set(pdu,CAN_FIELD_CAN_IDENTIFIER,0x0000000);
    if(res < 0)
        return -1;

    return 0;

}

int main(int argc, char *argv[])
{
    int fd, res;

    // Socket address
    struct sockaddr_ll sk_addr;

    // use of avtp and creation of avtp_stream_pdu
    struct avtp_ntscf_stream_pdu *pdu = alloca(PDU_SIZE);

    
    struct can_pdu *can_pdu = alloca(CAN_PDU_SIZE);

    // Seq num temp variable
    uint8_t seq_num = 0;

    // Argument provided to the executable
    argp_parse(&argp, argc, argv, 0, NULL, NULL);

    // Create talker socket--> util.h
    fd = create_talker_socket(priority);
    if (fd < 0)
        return 1;

    res = setup_socket_address(fd, ifname, macaddr, ETH_P_TSN, &sk_addr);
    if (res < 0){
        printf("socket error");
        goto err;
    }


    // Init_PDU call , one function above
    res = init_pdu(pdu);
    if (res < 0){
        
        goto err;
    }
    res = init_can_pdu(can_pdu);
    if (res < 0){
        
        goto err;
    }
	
	int i = 0;
    // Infinite loop of packets to be sent to the , listener
    while (i<1) {
        ssize_t n;
    
       
     
        res = avtp_ntscf_pdu_set(pdu, AVTP_NTSCF_FIELD_SEQ_NUM, seq_num++);
        if (res < 0)
            goto err;

        uint8_t *payload = payload_creation();
        memcpy(can_pdu->can_payload,payload,8);
        
        free(payload);
        
        memcpy(pdu->avtp_payload,can_pdu,CAN_PDU_SIZE);

        
        n = sendto(fd, pdu, PDU_SIZE, 0,
                   (struct sockaddr *) &sk_addr, sizeof(sk_addr));
        if (n < 0) {
            perror("Failed to send data");
            goto err;
        }

        if (n != PDU_SIZE) {
            fprintf(stderr, "wrote %zd bytes, expected %zd\n",
                    n, PDU_SIZE);
        }
        i++;
    }


    close(fd);
    return 0;

    err:
    printf("Error...");
    close(fd);
    return 1;
}
