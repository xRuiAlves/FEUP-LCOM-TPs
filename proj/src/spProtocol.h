#ifndef _SERIAL_PORT_PROTOCOL_H_
#define _SERIAL_PORT_PROTOCOL_H_

/** @defgroup spProtocol spProtocol
 * @{
 *
 * Serial Port communication protocol
 */

/////////////////////////
//  MESSAGE STRUCTURE  //
/////////////////////////

#define SPP_HEADER_BYTE   0x55     /**< @brief Message Header Byte */
#define SPP_TRAILER_BYTE  0x88     /**< @brief Message Trailer Byte */
#define SPP_MESSAGE_SIZE  3        /**< @brief A full message is 3 bytes long: header byte + content + trailer byte */


///////////////////////////
//  COMMUNICATION BYTES  //
///////////////////////////

#define SPP_ACK_BYTE       0xFA     /**< @brief Message Acknowledgement Byte */
#define SPP_NACK_BYTE      0xFE     /**< @brief Message Not Acknowledged Byte (Resend request) */
#define SPP_ERROR_BYTE     0xFC     /**< @brief Message Error */


///////////////////////////////////
//  OTHER COMMUNICATION DETAILS  //
///////////////////////////////////

#define SPP_WAIT_TIME           10000    /**< @brief Wait time in case of RBR Empty / THR Full (in micro-seconds)*/
#define SPP_MAX_NUM_TRIES       3        /**< @brief Maximum number of re-tries on communication errors */
#define SPP_COMPLETE_MESSAGE    0        /**< @brief Message Completed */
#define SPP_INCOMPLETE_MESSAGE  1        /**< @brief Message not Completed */
#define SPP_INVALID_HEADER      2        /**< @brief Invalid Message Header Byte */
#define SPP_INVALID_TRAILER     3        /**< @brief Invalid Message Trailer Byte */

 /** @} end of spProtocol */


#endif /* _SERIAL_PORT_PROTOCOL_H_ */
