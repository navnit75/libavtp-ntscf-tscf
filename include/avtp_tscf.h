#pragma once

#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


enum avtp_tscf_field{
    AVTP_TSCF_FIELD_SV,
    AVTP_TSCF_FIELD_MR,
    AVTP_TSCF_FIELD_TV,
    AVTP_TSCF_FIELD_SEQ_NUM,
    AVTP_TSCF_FIELD_TU,
    AVTP_TSCF_FIELD_STREAM_ID,
    AVTP_TSCF_FIELD_TIMESTAMP,
    AVTP_TSCF_FIELD_STREAM_DATA_LEN,
    AVTP_TSCF_FIELD_MAX
};


int avtp_tscf_pdu_get(const struct avtp_stream_pdu *pdu,
                      enum avtp_tscf_field field, uint64_t *val);

int avtp_tscf_pdu_set(struct avtp_stream_pdu *pdu,
                      enum avtp_tscf_field field, uint64_t val);

int avtp_tscf_pdu_init(struct avtp_stream_pdu *pdu);

#ifdef __cplusplus
}
#endif