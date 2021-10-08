#include <arpa/inet.h>
#include <string.h>

#include "avtp_ntscf.h"
#include "avtp.h"
#include "util.h"

#define SHIFT_NTSCF_SV          (31 - 8)
#define SHIFT_NTSCF_DATA_LEN            (31 - 23)
#define SHIFT_NTSCF_SEQ_NUM         (31 - 31)

#define MASK_NTSCF_SV          (BITMASK(1) << SHIFT_NTSCF_SV)
#define MASK_NTSCF_DATA_LEN			(BITMASK(11) << SHIFT_NTSCF_DATA_LEN)
#define MASK_NTSCF_SEQ_NUM         (BITMASK(8) << SHIFT_NTSCF_SEQ_NUM)        



static int get_field_value(const struct avtp_ntscf_stream_pdu *pdu,
				enum avtp_ntscf_field field, uint64_t *val)
{
    uint32_t bitmap, mask;
	uint8_t shift;

    switch (field) {
    case AVTP_NTSCF_FIELD_SV:
        mask = MASK_NTSCF_SV;
        shift = SHIFT_NTSCF_SV;
        bitmap = ntohl(pdu->subtype_data);
        break;
	case  AVTP_NTSCF_FIELD_DATA_LEN:
		mask = MASK_NTSCF_DATA_LEN;
		shift = SHIFT_NTSCF_DATA_LEN;
		bitmap = ntohl(pdu->subtype_data);
		break;
	case AVTP_NTSCF_FIELD_SEQ_NUM:
		mask = MASK_NTSCF_SEQ_NUM ;
		shift = SHIFT_NTSCF_SEQ_NUM;
		bitmap = ntohl(pdu->subtype_data);
		break;
	default:
		return -EINVAL;
	}

    *val = BITMAP_GET_VALUE(bitmap, mask, shift);
    return 0;
    
}


int avtp_ntscf_pdu_get(const struct avtp_ntscf_stream_pdu *pdu,
				enum avtp_ntscf_field field, uint64_t *val)
{
    int res; 
    if (!pdu || !val)
		return -EINVAL;


    switch(field){
        case AVTP_NTSCF_FIELD_SV:
        case AVTP_NTSCF_FIELD_DATA_LEN:
        case AVTP_NTSCF_FIELD_SEQ_NUM:
            res = get_field_value(pdu,field,val);
            break;
        case AVTP_NTSCF_FIELD_STREAM_ID:
            *val = be64toh(pdu->stream_id);
            res = 0;
            break;
        default:
		return -EINVAL;
    }
}


static int set_field_value(struct avtp_ntscf_stream_pdu *pdu,
				enum avtp_ntscf_field field, uint64_t val)
{
	uint32_t bitmap, mask;
	uint8_t shift;
	void *ptr;

	switch (field) {
    case AVTP_NTSCF_FIELD_SV:
        mask = MASK_NTSCF_SV;
        shift = SHIFT_NTSCF_SV;
        ptr = &pdu->subtype_data;
        break; 
	case  AVTP_NTSCF_FIELD_DATA_LEN:
		mask = MASK_NTSCF_DATA_LEN;
		shift = SHIFT_NTSCF_DATA_LEN;
		ptr = &pdu->subtype_data;
		break;
	case AVTP_NTSCF_FIELD_SEQ_NUM:
		mask = MASK_NTSCF_SEQ_NUM ;
		shift = SHIFT_NTSCF_SEQ_NUM;
		ptr = &pdu->subtype_data;
		break;
	
	default:
		return -EINVAL;
	}

	bitmap = get_unaligned_be32(ptr);

	BITMAP_SET_VALUE(bitmap, val, mask, shift);

	put_unaligned_be32(bitmap, ptr);

	return 0;
}


int avtp_ntscf_pdu_set(struct avtp_ntscf_stream_pdu *pdu,
				enum avtp_ntscf_field field, uint64_t val)
{
    int res; 

    if(!pdu) 
        return -EINVAL;
    switch(field){
        case AVTP_NTSCF_FIELD_SV:
        case AVTP_NTSCF_FIELD_DATA_LEN:
        case AVTP_NTSCF_FIELD_SEQ_NUM: 
            res = set_field_value(pdu,field,val);
            break;
        case AVTP_NTSCF_FIELD_STREAM_ID:
        	pdu->stream_id = htobe64(val);
	        res = 0;
	        break;
        default:
		    return -EINVAL;

    }
    return res; 


}




int avtp_ntscf_pdu_init(struct avtp_ntscf_stream_pdu *pdu){
    int res; 

    if(!pdu)
        return -EINVAL; 
    memset(pdu,0,sizeof(struct avtp_ntscf_stream_pdu));

    res = avtp_pdu_set((struct avtp_common_pdu *)pdu ,AVTP_FIELD_SUBTYPE,
                        AVTP_SUBTYPE_NTSCF);
    if(res < 0) 
        return res; 
    

 


    res = avtp_ntscf_pdu_set(pdu, AVTP_NTSCF_FIELD_SV, 1);
    if(res < 0) 
        return res; 

    return 0;

}