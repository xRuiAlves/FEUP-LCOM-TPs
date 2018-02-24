#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC
 */

#define BIT(n)              (0x01<<(n))

#define KBD_IRQ             1		 /**< @brief KBD IRQ Line  */
#define PS2_IRQ             12       /**< @brief PS/2 IRQ line */

#define KBD_DELAY           20000    /**< @brief Delay in micro-seconds (20 mili-seconds or 1/50 seconds) */

///////////////////////
// I/O Port Adresses //
///////////////////////

#define KBC_STATUS_REG      0x64		 /**< @brief KBC Status Register */
#define KBC_CMD_REG         0x64		 /**< @brief KBC Command Register */
#define KBC_IN_BUF          0x60		 /**< @brief KBC Input Buffer Register. Can also be 0x64 */
#define KBC_OUT_BUF         0x60		 /**< @brief KBC Output Buffer Register */


/////////////////////
// Status Register //
/////////////////////

#define KBC_STATUS_PARITY   BIT(7)           /**< @brief Parity error - invalid data */
#define KBC_STATUS_TIMEOUT  BIT(6)           /**< @brief Timeout error - invalid data */
#define KBC_STATUS_AUX      BIT(5)           /**< @brief Mouse data */
#define KBC_STATUS_INH      BIT(4)           /**< @brief Inhibit flag: 0 is keyboard is inhibited */
#define KBC_STATUS_A2       BIT(3)		       /**< @brief A2 input line: 0 data byte,
				                                                1 command byte */
#define KBC_STATUS_SYS      BIT(2)		       /**< @brief System Flag: 0 is system in power-on reset,
				                                              1 if system already initialized */
#define KBC_STATUS_IBF      BIT(1)		       /**< @brief Input buffer full, don't write command or arguments */
#define KBC_STATUS_OBF      BIT(0)		       /**< @brief Output buffer full - data available for reading */


//////////////////
// KBC Commands //
//////////////////

#define KBC_READ_COMMAND        0x20		 /**< @brief Returns Command Byte */
#define KBC_WRITE_COMMAND       0x60		 /**< @brief Take Arg: Command Byte */
#define KBC_SELF_TEST           0xAA		 /**< @brief Returns 0x55 if OK
				                                       0xFC if error */
#define KBC_CHECK_KBD_INTERF    0xAB		 /**< @brief Returns 0 if OK */
#define KBC_DISABLE_KBD_INTERF  0xAD		 /**< @brief Disables the Keyboard interface */
#define KBC_ENABLE_INTERF       0xAE		 /**< @brief Enables the Keyboard interface */
#define KBC_DISABLE_MOUSE       0xA7         /**< @brief Disables the mouse */
#define KBD_ENABLE_MOUSE        0xA8         /**< @brief Enables the mouse */
#define KBD_CHECK_MOUSE_IF      0xA9         /**< @brief Checks mouse interface, returning 0 is OK */
#define KBD_WRITE_BYTE_MOUSE    0xD4         /**< @brief Takes Arg: Byte to write to mouse */

#define KBC_REENABLE_KEYBOARD   0x47         /**< @brief Argument for KBC_WRITE_COMMAND, to reenable the keyboard in Minix */


////////////////////////
//    PS/2 COMMANDS   //
////////////////////////

#define PS2_CMD_RESET               0xFF     /**< @brief Mouse Reset */
#define PS2_CMD_RESEND              0xFE     /**< @brief For serial communications errors */
#define PS2_CMD_SET_DEFAULTS        0xF6     /**< @brief Set default values */
#define PS2_CMD_DISABLE_DATA_REP    0xF5     /**< @brief In stream mode, should be sent before any other command */
#define PS2_CMD_ENABLE_DATA_REP     0xF4     /**< @brief In stream mode only */
#define PS2_CMD_SET_SRATE           0xF3     /**< @brief Sets state sampling rate */
#define PS2_CMD_SET_REMOTE_MODE     0xF0     /**< @brief Send data on request only */
#define PS2_CMD_READ_DATA           0xEB     /**< @brief Send data packet request */
#define PS2_CMD_SET_STREAM_MODE     0xEA     /**< @brief Send data on events */
#define PS2_CMD_STATUS_REQUEST      0xE9     /**< @brief Get mouse configuration (3 bytes) */
#define PS2_CMD_SET_RESOLUTION      0xE8     /**< @brief Set the resolution*/
#define PS2_CMD_SET_ACCEL_MODE      0xE7     /**< @brief Acceleration mode */
#define PS2_CMD_SET_LINEAR_MODE     0xE6     /**< @brief Linear mode */


/////////////////////////////////
//  PS/2 ACKNOWLEDGMENT BYTES  //
/////////////////////////////////

#define PS2_ACK      0xFA    /**< @brief Acknowledged -> Everything is OK */
#define PS2_NACK     0xFE    /**< @brief Not Acknowledged -> Invalid Byte (May be because of a serial communication error) */
#define PS2_ERROR    0xFC    /**< @brief Error -> Second Consecutive Invalid Byte */


//////////////////////////
//  PS/2 PACKET STATES  //
//////////////////////////

#define PS2_COMPLETE_PACKET      0   /**< @brief All the three elements of the packet have been read, the packet is complete */
#define PS2_INCOMPLETE_PACKET    1   /**< @brief The packet is still missing elements to read */
#define PS2_UNSYNCHED_PACKET     2   /**< @brief The packet wasn't synched, stop the packet reading */


/////////////////////////////////////////////
//  PS/2 PACKET FIRST BYTE INTERPRETATION  //
/////////////////////////////////////////////

#define PS2_PACKET_MOUSELB            BIT(0)  /**< @brief 0 - left button not pressed  / 1 - left button pressed */
#define PS2_PACKET_MOUSERB            BIT(1)  /**< @brief 0 - right button not pressed / 1- right button pressed */
#define PS2_PACKET_MOUSEMB            BIT(2)  /**< @brief 0 - middle button not pressed / 1- middle button pressed */
#define PS2_PACKET_VERIFICATION_BIT   BIT(3)  /**< @brief 1 in the first packet, used to check synchronization of the packets */
#define PS2_PACKET_XSIGN              BIT(4)  /**< @brief To know in which direction is the mouse moving over the x axis */
#define PS2_PACKET_YSIGN              BIT(5)  /**< @brief To know in which direction is the mouse moving over the y axis */
#define PS2_PACKET_XOVF               BIT(6)  /**< @brief To know if there was an overflow in the x axis movement */
#define PS2_PACKET_YOVF               BIT(7)  /**< @brief To know if there was an overflow in the y axis movement */


//////////////////////
// KBC Command-Byte //
//////////////////////

#define KBC_CMDB_MOUSE_DIS      BIT(5)		 /**< @brief 1: Disable Mouse */
#define KBC_CMDB_KBD_DIS        BIT(4)		 /**< @brief 1: Disable Keyboard Interface */
#define KBC_CMDB_MOUSE_INT_EN   BIT(1)		 /**< @brief Enable interrupt on OBF, from mouse */
#define KBC_CMDB_KBD_INT_EN     BIT(0)		 /**< @brief Enable interrupt on OBF, from keyboard */


///////////////////////////////
//  USEFUL MAKE/BREAK CODES  //
///////////////////////////////

#define KBD_ESC_BREAKCODE   0x81

 /** @} end of i8042 */

#endif /* _LCOM_I8024_H_ */
