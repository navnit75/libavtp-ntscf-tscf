#include <arpa/inet.h>
#include <string.h>

#include "can_brief.h"
#include "util.h"
#include "can.h"




#define SHIFT_CAN_BRIEF_IDENTIFIER            (31-31)

#define MASK_CAN_BRIEF_IDENTIFIER         (BITMASK(29) << SHIFT_CAN_BRIEF_IDENTIFIER)     
     


static int get_field_value(const struct can_brief_pdu *pdu,
				enum can_brief_field field, uint64_t *val)
{
    uint32_t bitmap, mask;
	uint8_t shift;

    switch(field){
        case CAN_BRIEF_FIELD_CAN_IDENTIFIER:
            mask = MASK_CAN_BRIEF_IDENTIFIER;
            shift = SHIFT_CAN_BRIEF_IDENTIFIER;
            bitmap = ntohl(pdu->can_header);
            break;
        
        default: 
            return -EINVAL;
            break;

    }
    
    *val = BITMAP_GET_VALUE(bitmap, mask, shift);
    return 0;
    
}

int can_brief_pdu_get(const struct can_brief_pdu *pdu,
				enum can_brief_field field, uint64_t *val)
{
    int res; 
    if (!pdu || !val)
		return -EINVAL;
    
    switch(field){
    case CAN_BRIEF_FIELD_MSG_TYPE:
    case CAN_BRIEF_FIELD_MSG_LEN:
    case CAN_BRIEF_FIELD_PAD:
    case CAN_BRIEF_FIELD_MTV:
    case CAN_BRIEF_FIELD_RTR:
    case CAN_BRIEF_FIELD_EFF:
    case CAN_BRIEF_FIELD_BRS:
    case CAN_BRIEF_FIELD_FDF:
    case CAN_BRIEF_FIELD_ESI:
    case CAN_BRIEF_FIELD_CAN_BUS_ID:
        res = can_pdu_get((struct can_pdu *)pdu,(enum can_field) field,
                                 val);
        break; 
    case CAN_BRIEF_FIELD_CAN_IDENTIFIER:
        res = get_field_value(pdu, field, val);
        break; 
        
    default: 
        res = -EINVAL;
        break; 
    
    }
    return res; 

}

static int set_field_value(struct can_brief_pdu *pdu,
				enum can_brief_field field, uint64_t val)
{
	uint32_t bitmap, mask;
	uint8_t shift;
	void *ptr;

	switch (field) {
	case CAN_BRIEF_FIELD_CAN_IDENTIFIER:
        mask = MASK_CAN_BRIEF_IDENTIFIER;
        shift = SHIFT_CAN_BRIEF_IDENTIFIER;
        ptr = &pdu->can_header;
        break;  
	default:
		return -EINVAL;
	}

	bitmap = get_unaligned_be32(ptr);

	BITMAP_SET_VALUE(bitmap, val, mask, shift);

	put_unaligned_be32(bitmap, ptr);

	return 0;
}


int can_brief_pdu_set(struct can_brief_pdu *pdu,
				enum can_brief_field field, uint64_t val)
{
    int res; 
    if(!pdu)
        return -EINVAL;

    switch(field){
    case CAN_BRIEF_FIELD_MSG_TYPE:
    case CAN_BRIEF_FIELD_MSG_LEN:
    case CAN_BRIEF_FIELD_PAD:
    case CAN_BRIEF_FIELD_MTV:
    case CAN_BRIEF_FIELD_RTR:
    case CAN_BRIEF_FIELD_EFF:
    case CAN_BRIEF_FIELD_BRS:
    case CAN_BRIEF_FIELD_FDF:
    case CAN_BRIEF_FIELD_ESI:
    case CAN_BRIEF_FIELD_CAN_BUS_ID:
        res = can_pdu_set((struct can_pdu *)pdu, (enum can_field)field,
                                 val);
        break; 

    case CAN_BRIEF_FIELD_CAN_IDENTIFIER:
        res = set_field_value(pdu, field, val);
        break; 

    default: 
        res = -EINVAL;
        break; 
    
    }
    return res; 
}

int can_brief_pdu_init(struct can_brief_pdu *pdu){
    int res; 

    if(!pdu)
        return -EINVAL;

    memset(pdu,0,sizeof(struct can_brief_pdu));

    res = can_brief_pdu_set(pdu ,CAN_BRIEF_FIELD_PAD, 0);
    if(res < 0)
        return res;
    
    return 0;
}
