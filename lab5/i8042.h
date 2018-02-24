#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define BIT(n)              (0x01<<(n))

#define KBD_IRQ             1		/* KBD IRQ Line  */
#define PS2_IRQ             12      /* PS/2 IRQ line */

#define KBD_DELAY           20000   /* Delay in micro-seconds (20 mili-seconds or 1/50 seconds) */

///////////////////////
// I/O Port Adresses //
///////////////////////

#define KBC_STATUS_REG      0x64		/* KBC Status Register */
#define KBC_CMD_REG         0x64		/* KBC Command Register */
#define KBC_IN_BUF          0x60		/* KBC Input Buffer Register. Can also be 0x64 */
#define KBC_OUT_BUF         0x60		/* KBC Output Buffer Register */


/////////////////////
// Status Register //
/////////////////////

#define KBC_STATUS_PARITY   BIT(7)          /* Parity error - invalid data */
#define KBC_STATUS_TIMEOUT  BIT(6)          /* Timeout error - invalid data */
#define KBC_STATUS_AUX      BIT(5)          /* Mouse data */
#define KBC_STATUS_INH      BIT(4)          /* Inhibit flag: 0 is keyboard is inhibited */
#define KBC_STATUS_A2       BIT(3)		      /* A2 input line: 0 data byte,
				                                                1 command byte */
#define KBC_STATUS_SYS      BIT(2)		      /* System Flag: 0 is system in power-on reset,
				                                              1 if system already initialized */
#define KBC_STATUS_IBF      BIT(1)		      /* Input buffer full, don't write command or arguments */
#define KBC_STATUS_OBF      BIT(0)		      /* Output buffer full - data available for reading */


//////////////////
// KBC Commands //
//////////////////

#define KBC_READ_COMMAND        0x20		/* Returns Command Byte */
#define KBC_WRITE_COMMAND       0x60		/* Take Arg: Command Byte */
#define KBC_SELF_TEST           0xAA		/* Returns 0x55 if OK
				                                       0xFC if error */
#define KBC_CHECK_KBD_INTERF    0xAB		/* Returns 0 if OK */
#define KBC_DISABLE_KBD_INTERF  0xAD		/* Disables the Keyboard interface */
#define KBC_ENABLE_INTERF       0xAE		/* Enables the Keyboard interface */
#define KBC_DISABLE_MOUSE       0xA7        /* Disables the mouse */
#define KBD_ENABLE_MOUSE        0xA8        /* Enables the mouse */
#define KBD_CHECK_MOUSE_IF      0xA9        /* Checks mouse interface, returning 0 is OK */
#define KBD_WRITE_BYTE_MOUSE    0xD4        /* Takes Arg: Byte to write to mouse */

#define KBC_REENABLE_KEYBOARD   0x47        /* Argument for KBC_WRITE_COMMAND, to reenable the keyboard in Minix */


////////////////////////
//    PS/2 COMMANDS   //
////////////////////////

#define PS2_CMD_RESET               0xFF    /* Mouse Reset */
#define PS2_CMD_RESEND              0xFE    /* For serial communications errors */
#define PS2_CMD_SET_DEFAULTS        0xF6    /* Set default values */
#define PS2_CMD_DISABLE_DATA_REP    0xF5    /* In stream mode, should be sent before any other command */
#define PS2_CMD_ENABLE_DATA_REP     0xF4    /* In stream mode only */
#define PS2_CMD_SET_SRATE           0xF3    /* Sets state sampling rate */
#define PS2_CMD_SET_REMOTE_MODE     0xF0    /* Send data on request only */
#define PS2_CMD_READ_DATA           0xEB    /* Send data packet request */
#define PS2_CMD_SET_STREAM_MODE     0xEA    /* Send data on events */
#define PS2_CMD_STATUS_REQUEST      0xE9    /* Get mouse configuration (3 bytes) */
#define PS2_CMD_SET_RESOLUTION      0xE8    /* */
#define PS2_CMD_SET_ACCEL_MODE      0xE7    /* Acceleration mode */
#define PS2_CMD_SET_LINEAR_MODE     0xE6    /* Linear mode */


/////////////////////////////////
//  PS/2 ACKNOWLEDGMENT BYTES  //
/////////////////////////////////

#define PS2_ACK      0xFA   /* Acknowledged -> Everything is OK */
#define PS2_NACK     0xFE   /* Not Acknowledged -> Invalid Byte (May be because of a serial communication error) */
#define PS2_ERROR    0xFC   /* Error -> Second Consecutive Invalid Byte */


//////////////////////////
//  PS/2 PACKET STATES  //
//////////////////////////

#define PS2_COMPLETE_PACKET      0
#define PS2_INCOMPLETE_PACKET    1
#define PS2_UNSYNCHED_PACKET     2


/////////////////////////////////////////////
//  PS/2 PACKET FIRST BYTE INTERPRETATION  //
/////////////////////////////////////////////

#define PS2_PACKET_MOUSELB       BIT(0)
#define PS2_PACKET_MOUSERB       BIT(1)
#define PS2_PACKET_MOUSEMB       BIT(2)
#define PS2_PACKET_XSIGN         BIT(4)
#define PS2_PACKET_YSIGN         BIT(5)
#define PS2_PACKET_XOVF          BIT(6)
#define PS2_PACKET_YOVF          BIT(7)


//////////////////////
// KBC Command-Byte //
//////////////////////

#define KBC_CMDB_MOUSE_DIS      BIT(5)		/* 1: Disable Mouse */
#define KBC_CMDB_KBD_DIS        BIT(4)		/* 1: Disable Keyboard Interface */
#define KBC_CMDB_MOUSE_INT_EN   BIT(1)		/* Enable interrupt on OBF, from mouse */
#define KBC_CMDB_KBD_INT_EN     BIT(0)		/* Enable interrupt on OBF, from keyboard */


///////////////////////////////
//  USEFUL MAKE/BREAK CODES  //
///////////////////////////////

#define KBD_ESC_BREAKCODE   0x81

#endif /* _LCOM_I8024_H_ */
