#pragma once

#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


struct can_pdu {
    uint32_t can_data;
    uint64_t message_timestamp;
    uint32_t can_header;
    uint8_t can_payload[0];
} __attribute__ ((__packed__));


enum can_field {
    CAN_FIELD_MSG_TYPE,
    CAN_FIELD_MSG_LEN,
    CAN_FIELD_PAD,
    CAN_FIELD_MTV,
    CAN_FIELD_RTR,
    CAN_FIELD_EFF,
    CAN_FIELD_BRS,
    CAN_FIELD_FDF,
    CAN_FIELD_ESI,
    CAN_FIELD_CAN_BUS_ID,
    CAN_FIELD_MESSAGE_TIMESTAMP,
    CAN_FIELD_CAN_IDENTIFIER,
    CAN_FIELD_MAX
};


int can_pdu_get(const struct  can_pdu *pdu,
                enum can_field field, uint64_t *val);

int can_pdu_set(struct can_pdu *pdu,
                enum can_field field, uint64_t val);

int can_pdu_init(struct can_pdu *pdu);

#ifdef __cplusplus
}
#endif