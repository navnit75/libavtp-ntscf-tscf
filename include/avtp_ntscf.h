#pragma once

#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



struct avtp_ntscf_stream_pdu {
	uint32_t subtype_data;
	uint64_t stream_id;
	uint8_t avtp_payload[0];
} __attribute__ ((__packed__));


enum avtp_ntscf_field{
    AVTP_NTSCF_FIELD_SV,
    AVTP_NTSCF_FIELD_DATA_LEN,
    AVTP_NTSCF_FIELD_SEQ_NUM,
    AVTP_NTSCF_FIELD_STREAM_ID,
    AVTP_NTSCF_FIELD_MAX
};


int avtp_ntscf_pdu_get(const struct avtp_ntscf_stream_pdu *pdu,
				enum avtp_ntscf_field field, uint64_t *val);


int avtp_ntscf_pdu_set(struct avtp_ntscf_stream_pdu *pdu,
				enum avtp_ntscf_field field, uint64_t val);


int avtp_ntscf_pdu_init(struct avtp_ntscf_stream_pdu *pdu);

#ifdef __cplusplus
}
#endif