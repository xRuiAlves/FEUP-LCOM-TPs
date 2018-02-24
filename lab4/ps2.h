#ifndef LCOM_PS2_H
#define LCOM_PS2_H


/* Symbolic constants */
#define PS2_SUCCESS                  0
#define PS2_READ_STATUS_ERROR        1
#define PS2_READ_OUTBUFFER_ERROR     2
#define PS2_INT_SUBS_ERROR           -1
#define PS2_INT_UNSUBS_ERROR         3
#define PS2_WRITE_KBC_CMD_ERROR      4
#define PS2_PARITY_TIMEOUT_ERROR     5
#define PS2_READ_OBF_TIMEOUT_ERROR   6
#define PS2_FAILED_OBF_FLUSH         7
#define PS2_MODE_CHANGE_FAILED       8

#define PS2_MAX_TRIES                3       // 3 tries correspond to about ~60 miliseconds


/**
 * @brief Subscribes and enables Mouse interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int ps2_subscribe_int();


/**
 * @brief Disables and unsubscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int ps2_unsubscribe_int();


/**
 * @brief Sets the Mouse mode to Stream Mode
 *
 * @return Returns the acknoledgement byte from the Mouse or PS2_ERROR_OCCURED ( 1 ) on status register/output buffer reading failure
 */
unsigned char ps2_set_streamMode();


/**
 * @brief Enables the Mouse's Stream Mode data reporting
 *
 * @return Returns the acknoledgement byte from the Mouse or PS2_ERROR_OCCURED ( 1 ) on status register/output buffer reading failure
 */
unsigned char ps2_enable_dataReporting();


/**
 * @brief Disables the Mouse's Stream Mode data reporting
 *
 * @return Returns the acknoledgement byte from the Mouse or PS2_ERROR_OCCURED ( 1 ) on status register/output buffer reading failure
 */
unsigned char ps2_disable_dataReporting();


/**
 * @brief Sets the Mouse mode to Remote Mode
 *
 * @return Returns the acknoledgement byte from the Mouse or PS2_ERROR_OCCURED ( 1 ) on status register/output buffer reading failure
 */
unsigned char ps2_set_remoteMode();


/**
 * @brief Requests a packet from the mouse (remote mode)
 *
 * @return Returns the acknoledgement byte from the Mouse or PS2_ERROR_OCCURED ( 1 ) on status register/output buffer reading failure
 */
unsigned char ps2_request_packet();


/**
 * @brief Writes a command to the mouse (using the KBC controller)
 *
 * @param cmd - Command to write to mouse
 *
 * @return Returns the acknoledgement byte from the Mouse or PS2_ERROR_OCCURED ( 1 ) on status register/output buffer reading failure / on kbd_write_cmd_witharg() failure
 */
unsigned char ps2_write_cmd(unsigned char cmd);


/**
 * @brief Mouse interrupt handler
 *
 * @return Returns 0 on success after reading the packetByte, and non-zero otherwise
 */
int ps2_int_handler();


/**
 * @brief Prints the Packet on the screen
 *
 * @details It prints the packetByte on the format B1 B2 B3 LB MB RB XOV YOV X Y
 *
 */
void ps2_print_packet();


/**
 * @brief Passes the packetByte to the right place in the packet array.
 *
 * @details It checks if the bit 3 of the first Byte is set to 1. If it is, it places the first Byte in the Packet's first place, and then sets the other 2 Bytes (on the following function calls)
 *
 * @return Returns 0 on success, 1 in case of the packet not being complete yet and still needs to set other Bytes or 2 if the packet wasn't synched (the bit 3 in the first Byte wasn't set to 1).
 */
int ps2_parse_packetByte();


/**
 * @brief Reads a packet byte from the output buffer (remote mode). If the output buffer is not full yet,
 *          it waits for it to be full for a few tries (communication between the mouse and the controller
 *          may be slow)
 *
 * @param packetByte Pointer to the unsigned char (byte) that will contain the packet byte after reading
 *
 * @return Returns 0 on success and non-zero otherwise
 */
int ps2_remote_read_packetByte(unsigned char * packetByte);

#endif /* LCOM_PS2_H */
