#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define BIT(n)              (0x01<<(n))

#define KBD_IRQ             1		/* KBD IRQ Line */

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

#define KBC_REENABLE_KEYBOARD   0x47        /* Argument for KBC_WRITE_COMMAND, to reenable the keyboard in Minix */

//////////////////////
// KBC Command-Byte //
//////////////////////

#define KBC_CMDB_MOUSE_DIS      BIT(5)		/* 1: Disable Mouse */
#define KBC_CMDB_KBD_DIS        BIT(4)		/* 1: Disable Keyboard Interface */
#define KBC_CMDB_MOUSE_INT_EN   BIT(1)		/* Enable interrupt on OBF, from mouse */
#define KBC_CMDB_KBD_INT_EN     BIT(0)		/* Enable interrupt on OBF, from keyboard */

#endif /* _LCOM_I8024_H_ */
