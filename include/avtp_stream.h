
#pragma once

#include <errno.h>
#include <stdint.h>

#pragma GCC visibility push(hidden)

#ifdef __cplusplus
extern "C" {
#endif

/* XXX: To be able to use the functions provided by this header,
 * without needing to direct "translate" enum values, it is necessary
 * that any format specific enum have the following fields (excluding
 * AVTP_STREAM_FIELD_MAX) in the same order as below. For instance,
 * some `enum avtp_newformat_field` would start like:
 *
 * enum avtp_newformat_field {
 *      AVTP_NEWFORMAT_FIELD_SV,
 *      AVTP_NEWFORMAT_FIELD_MR,
 *      // (other stream fields here)
 *      AVTP_NEWFORMAT_FIELD_XYZ,
 *      // (other newformat specific fields here)
 * }
 *
 * This way, one can simply cast enums when calling functions from this
 * header:
 *
 * avtp_stream_pdu_get(pdu, (enum avtp_stream_field) field, val);
 *
 * Otherwise, the mapping step would be necessary before the calls.
 */
enum avtp_stream_field {
    AVTP_STREAM_FIELD_SV,
    AVTP_STREAM_FIELD_MR,
    AVTP_STREAM_FIELD_TV,
    AVTP_STREAM_FIELD_SEQ_NUM,
    AVTP_STREAM_FIELD_TU,
    AVTP_STREAM_FIELD_STREAM_ID,
    AVTP_STREAM_FIELD_TIMESTAMP,
    AVTP_STREAM_FIELD_STREAM_DATA_LEN,
    AVTP_STREAM_FIELD_MAX
};

/* Get value from Stream AVTPDU field.
 * @pdu: Pointer to PDU struct.
 * @field: PDU field to be retrieved.
 * @val: Pointer to variable which the retrieved value should be saved.
 *
 * Returns:
 *    0: Success.
 *    -EINVAL: If any argument is invalid.
 */
int avtp_stream_pdu_get(const struct avtp_stream_pdu *pdu,
                        enum avtp_stream_field field, uint64_t *val);

/* Set value from Stream AVTPDU field.
 * @pdu: Pointer to PDU struct.
 * @field: PDU field to be set.
 * @val: Value to be set.
 *
 * Returns:
 *    0: Success.
 *    -EINVAL: If any argument is invalid.
 */
int avtp_stream_pdu_set(struct avtp_stream_pdu *pdu,
                        enum avtp_stream_field field, uint64_t val);

#ifdef __cplusplus
}
#endif

#pragma GCC visibility pop