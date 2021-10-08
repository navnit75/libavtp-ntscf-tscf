#include <arpa/inet.h>
#include <string.h>

#include "avtp.h"
#include "avtp_stream.h"
#include "avtp_tscf.h"



int avtp_tscf_pdu_get(const struct avtp_stream_pdu *pdu,
                      enum avtp_tscf_field field, uint64_t *val)
{
    int res;

    if (!pdu || !val)
        return -EINVAL;


    switch(field){
        case AVTP_TSCF_FIELD_SV:
        case AVTP_TSCF_FIELD_MR:
        case AVTP_TSCF_FIELD_TV:
        case AVTP_TSCF_FIELD_SEQ_NUM:
        case AVTP_TSCF_FIELD_TU:
        case AVTP_TSCF_FIELD_STREAM_ID:
        case AVTP_TSCF_FIELD_TIMESTAMP:
        case AVTP_TSCF_FIELD_STREAM_DATA_LEN:
            res = avtp_stream_pdu_get(pdu, (enum avtp_stream_field) field,
                                      val);
            break;
        default:
            return -EINVAL;
            break;
    }
    return res;

}



int avtp_tscf_pdu_set(struct avtp_stream_pdu *pdu,
                      enum avtp_tscf_field field, uint64_t val)
{
    int res;
    if (!pdu)
        return -EINVAL;

    switch(field){
        case AVTP_TSCF_FIELD_SV:
        case AVTP_TSCF_FIELD_MR:
        case AVTP_TSCF_FIELD_TV:
        case AVTP_TSCF_FIELD_SEQ_NUM:
        case AVTP_TSCF_FIELD_TU:
        case AVTP_TSCF_FIELD_STREAM_ID:
        case AVTP_TSCF_FIELD_TIMESTAMP:
        case AVTP_TSCF_FIELD_STREAM_DATA_LEN:
            res = avtp_stream_pdu_set(pdu, (enum avtp_stream_field) field,
                                      val);
            break;
        default:
            return -EINVAL;
            break;
    }
    return res;

}



int avtp_tscf_pdu_init(struct avtp_stream_pdu *pdu)
{
    int res;

    if(!pdu)
        return -EINVAL;
    memset(pdu,0,sizeof(struct avtp_stream_pdu));

    res = avtp_pdu_set((struct avtp_common_pdu *)pdu,AVTP_FIELD_SUBTYPE,
                       AVTP_SUBTYPE_TSCF);
    if(res < 0)
        return res;





    res =avtp_tscf_pdu_set(pdu, AVTP_TSCF_FIELD_SV, 1);
    if(res < 0)
        return res;

    return 0;



}




