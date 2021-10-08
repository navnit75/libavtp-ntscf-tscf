#pragma once

#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct can_brief_pdu {
	uint32_t can_data;
	uint32_t can_header; 
    uint8_t can_payload[0];
} __attribute__ ((__packed__));


enum can_brief_field {
    CAN_BRIEF_FIELD_MSG_TYPE,
    CAN_BRIEF_FIELD_MSG_LEN,
    CAN_BRIEF_FIELD_PAD,
    CAN_BRIEF_FIELD_MTV,
    CAN_BRIEF_FIELD_RTR,
    CAN_BRIEF_FIELD_EFF,
    CAN_BRIEF_FIELD_BRS,
    CAN_BRIEF_FIELD_FDF,
    CAN_BRIEF_FIELD_ESI,
    CAN_BRIEF_FIELD_CAN_BUS_ID,
    CAN_BRIEF_FIELD_CAN_IDENTIFIER,
    CAN_BRIEF_FIELD_MAX
};


int can_brief_pdu_get(const struct  can_brief_pdu *pdu,
				enum can_brief_field field, uint64_t *val);

int can_brief_pdu_set(struct can_brief_pdu *pdu,
				enum can_brief_field field, uint64_t val);

int can_brief_pdu_init(struct can_brief_pdu *pdu);

#ifdef __cplusplus
}
#endif