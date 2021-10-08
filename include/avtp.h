
#pragma once

#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AVTP subtypes values. For further information refer to section 4.4.3.2 from
 * IEEE 1722-2016 spec.
 */
#define AVTP_SUBTYPE_61883_IIDC			0x00
#define AVTP_SUBTYPE_MMA_STREAM			0x01
#define AVTP_SUBTYPE_AAF			0x02
#define AVTP_SUBTYPE_CVF			0x03
#define AVTP_SUBTYPE_CRF			0x04
#define AVTP_SUBTYPE_TSCF			0x05
#define AVTP_SUBTYPE_SVF			0x06
#define AVTP_SUBTYPE_RVF			0x07
#define AVTP_SUBTYPE_AEF_CONTINUOUS		0x6E
#define AVTP_SUBTYPE_VSF_STREAM			0x6F
#define AVTP_SUBTYPE_EF_STREAM			0x7F
#define AVTP_SUBTYPE_NTSCF			0x82
#define AVTP_SUBTYPE_ESCF			0xEC
#define AVTP_SUBTYPE_EECF			0xED
#define AVTP_SUBTYPE_AEF_DISCRETE		0xEE
#define AVTP_SUBTYPE_ADP			0xFA
#define AVTP_SUBTYPE_AECP			0xFB
#define AVTP_SUBTYPE_ACMP			0xFC
#define AVTP_SUBTYPE_MAAP			0xFE
#define AVTP_SUBTYPE_EF_CONTROL			0xFF

/* XXX: Fields from PDU structs should not be read or written directly since
 * they are encoded in Network order which may be different from the Host
 * order (see section 3.4.1 from IEEE 1722-2016 spec for further information).
 *
 * Any read or write operation with PDU structs should be done via getter and
 * setter APIs which handle byte order conversion.
 */
struct avtp_common_pdu {
    uint32_t subtype_data;
    uint8_t pdu_specific[0];
} __attribute__ ((__packed__));

struct avtp_stream_pdu {
    uint32_t subtype_data;
    uint64_t stream_id;
    uint32_t avtp_time;
    uint32_t format_specific;
    uint32_t packet_info;
    uint8_t avtp_payload[0];
} __attribute__ ((__packed__));

enum avtp_field {
    AVTP_FIELD_SUBTYPE,
    AVTP_FIELD_VERSION,
    AVTP_FIELD_MAX,
};

/* Get value from Common AVTPDU field.
 * @pdu: Pointer to PDU struct.
 * @field: PDU field to be retrieved.
 * @val: Pointer to variable which the retrieved value should be saved.
 *
 * Returns:
 *    0: Success.
 *    -EINVAL: If any argument is invalid.
 */
int avtp_pdu_get(const struct avtp_common_pdu *pdu, enum avtp_field field,
                 uint32_t *val);

/* Set value from Common AVTPDU field.
 * @pdu: Pointer to PDU struct.
 * @field: PDU field to be set.
 * @val: Value to be set.
 *
 * Returns:
 *    0: Success.
 *    -EINVAL: If any argument is invalid.
 */
int avtp_pdu_set(struct avtp_common_pdu *pdu, enum avtp_field field,
                 uint32_t val);

#ifdef __cplusplus
}
#endif

