#include <arpa/inet.h>
#include <string.h>

#include "can.h"
#include "util.h"



#define SHIFT_MSG_TYPE          (31 - 6)
#define SHIFT_MSG_LEN           (31 - 15)
#define SHIFT_PAD           (31 - 17)
#define SHIFT_MTV           (31 - 18)
#define SHIFT_RTR           (31 - 19)
#define SHIFT_EFF           (31 - 20)
#define SHIFT_BRS           (31 - 21)
#define SHIFT_FDF           (31 - 22)
#define SHIFT_ESI           (31 - 23)
#define SHIFT_CAN_BUS_ID           (31 - 31)
#define SHIFT_CAN_IDENTIFIER            (31-31)



#define MASK_MSG_TYPE			(BITMASK(7) << SHIFT_MSG_TYPE)
#define MASK_MSG_LEN         (BITMASK(9) << SHIFT_MSG_LEN)
#define MASK_PAD         (BITMASK(2) << SHIFT_PAD)
#define MASK_MTV         (BITMASK(1) << SHIFT_MTV)
#define MASK_RTR         (BITMASK(1) << SHIFT_RTR)
#define MASK_EFF        (BITMASK(1) << SHIFT_EFF)
#define MASK_BRS         (BITMASK(1) << SHIFT_BRS)
#define MASK_FDF         (BITMASK(1) << SHIFT_FDF)
#define MASK_ESI         (BITMASK(1) << SHIFT_ESI)
#define MASK_CAN_BUS_ID         (BITMASK(5) << SHIFT_CAN_BUS_ID)
#define MASK_CAN_IDENTIFIER         (BITMASK(29) << SHIFT_CAN_IDENTIFIER)





static int get_field_value(const struct can_pdu *pdu,
                           enum can_field field, uint64_t *val)
{
    uint32_t bitmap, mask;
    uint8_t shift;

    switch(field){
        case CAN_FIELD_MSG_TYPE:
            mask = MASK_MSG_TYPE;
            shift = SHIFT_MSG_TYPE;
            bitmap = ntohl(pdu->can_data);
            break;

        case CAN_FIELD_MSG_LEN:
            mask = MASK_MSG_LEN;
            shift = SHIFT_MSG_LEN;
            bitmap = ntohl(pdu->can_data);
            break;

        case CAN_FIELD_PAD:
            mask = MASK_PAD;
            shift = SHIFT_PAD;
            bitmap = ntohl(pdu->can_data);
            break;

        case CAN_FIELD_MTV:
            mask = MASK_MTV;
            shift = SHIFT_MTV;
            bitmap = ntohl(pdu->can_data);
            break;

        case CAN_FIELD_RTR:
            mask = MASK_RTR;
            shift = SHIFT_RTR;
            bitmap = ntohl(pdu->can_data);
            break;

        case CAN_FIELD_EFF:
            mask = MASK_EFF;
            shift = SHIFT_EFF;
            bitmap = ntohl(pdu->can_data);
            break;

        case CAN_FIELD_BRS:
            mask = MASK_BRS;
            shift = SHIFT_BRS;
            bitmap = ntohl(pdu->can_data);
            break;

        case CAN_FIELD_FDF:
            mask = MASK_FDF;
            shift = SHIFT_FDF;
            bitmap = ntohl(pdu->can_data);
            break;

        case CAN_FIELD_ESI:
            mask = MASK_ESI;
            shift = SHIFT_ESI;
            bitmap = ntohl(pdu->can_data);;
            break;

        case CAN_FIELD_CAN_BUS_ID:
            mask = MASK_CAN_BUS_ID;
            shift = SHIFT_CAN_BUS_ID;
            bitmap = ntohl(pdu->can_data);
            break;

        case CAN_FIELD_CAN_IDENTIFIER:
            mask = MASK_CAN_IDENTIFIER;
            shift = SHIFT_CAN_IDENTIFIER;
            bitmap = ntohl(pdu->can_header);
            break;

        default:
            return -EINVAL;
            break;

    }

    *val = BITMAP_GET_VALUE(bitmap, mask, shift);
    return 0;

}


int can_pdu_get(const struct can_pdu *pdu,
                enum can_field field, uint64_t *val)
{
    int res;
    if (!pdu || !val)
        return -EINVAL;

    switch(field){
        case CAN_FIELD_MSG_TYPE:
        case CAN_FIELD_MSG_LEN:
        case CAN_FIELD_PAD:
        case CAN_FIELD_MTV:
        case CAN_FIELD_RTR:
        case CAN_FIELD_EFF:
        case CAN_FIELD_BRS:
        case CAN_FIELD_FDF:
        case CAN_FIELD_ESI:
        case CAN_FIELD_CAN_BUS_ID:
        case CAN_FIELD_CAN_IDENTIFIER:
            res = get_field_value(pdu, field, val);
            break;
        case CAN_FIELD_MESSAGE_TIMESTAMP:
            *val = be64toh(pdu->message_timestamp);
            res = 0;
            break;
        default:
            res = -EINVAL;
            break;

    }
    return res;

}



static int set_field_value(struct can_pdu *pdu,
                           enum can_field field, uint64_t val)
{
    uint32_t bitmap, mask;
    uint8_t shift;
    void *ptr;

    switch(field){
        case CAN_FIELD_MSG_TYPE:
            mask = MASK_MSG_TYPE;
            shift = SHIFT_MSG_TYPE;
            ptr = &pdu->can_data;
            break;

        case CAN_FIELD_MSG_LEN:
            mask = MASK_MSG_LEN;
            shift = SHIFT_MSG_LEN;
            ptr = &pdu->can_data;
            break;



        case CAN_FIELD_MTV:
            mask = MASK_MTV;
            shift = SHIFT_MTV;
            ptr = &pdu->can_data;
            break;

        case CAN_FIELD_RTR:
            mask = MASK_RTR;
            shift = SHIFT_RTR;
            ptr = &pdu->can_data;
            break;

        case CAN_FIELD_EFF:
            mask = MASK_EFF;
            shift = SHIFT_EFF;
            ptr = &pdu->can_data;
            break;

        case CAN_FIELD_BRS:
            mask = MASK_BRS;
            shift = SHIFT_BRS;
            ptr = &pdu->can_data;
            break;

        case CAN_FIELD_FDF:
            mask = MASK_FDF;
            shift = SHIFT_FDF;
            ptr = &pdu->can_data;
            break;

        case CAN_FIELD_ESI:
            mask = MASK_ESI;
            shift = SHIFT_ESI;
            ptr = &pdu->can_data;
            break;

        case CAN_FIELD_CAN_BUS_ID:
            mask = MASK_CAN_BUS_ID;
            shift = SHIFT_CAN_BUS_ID;
            ptr = &pdu->can_data;
            break;

        case CAN_FIELD_CAN_IDENTIFIER:
            mask = MASK_CAN_IDENTIFIER;
            shift = SHIFT_CAN_IDENTIFIER;
            ptr = &pdu->can_header;
            break;
            break ;
        default:
            return -EINVAL;
            break;

    }
    bitmap = get_unaligned_be32(ptr);

    BITMAP_SET_VALUE(bitmap, val, mask, shift);

    put_unaligned_be32(bitmap, ptr);

    return 0;
}


int can_pdu_set(struct can_pdu *pdu,
                enum can_field field, uint64_t val)
{
    int res;
    if(!pdu)
        return -EINVAL;
    switch(field){
        case CAN_FIELD_MSG_TYPE:
        case CAN_FIELD_MSG_LEN:
        case CAN_FIELD_PAD:
        case CAN_FIELD_MTV:
        case CAN_FIELD_RTR:
        case CAN_FIELD_EFF:
        case CAN_FIELD_BRS:
        case CAN_FIELD_FDF:
        case CAN_FIELD_ESI:
        case CAN_FIELD_CAN_BUS_ID:
        case CAN_FIELD_CAN_IDENTIFIER:
            res = set_field_value(pdu,field,val);
            break;
        case CAN_FIELD_MESSAGE_TIMESTAMP:
            pdu->message_timestamp = htobe64(val);
            res = 0;
            break;
        default:
            res = -EINVAL;
            break;

    }
    return res;
}

int can_pdu_init(struct can_pdu *pdu){
    int res;

    if(!pdu)
        return -EINVAL;

    memset(pdu,0,sizeof(struct can_pdu));

//    res = can_pdu_set(pdu,CAN_FIELD_PAD,0);
    if(res < 0)
        return res;

    return 0;
}
