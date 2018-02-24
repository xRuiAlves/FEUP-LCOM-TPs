#ifndef _KBD_H_
#define _KBD_H_

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "stdio.h"

/** @defgroup kbd kbd
 * @{
 * Functions for manipulating the kbc controller for the keyboard
 */

/* Symbolic constants */
#define KBD_SUCCESS             0
#define KBD_ERROR_OCCURED       1
#define KBD_MAX_INBUFF_TRIES    2       /**< @brief 2 tries correspong to about ~40 miliseconds */


/* Keyboard Keys Makecodes */
#define ESC_KEY  0x01
#define W_KEY    0x11
#define A_KEY    0x1e
#define S_KEY    0x1f
#define D_KEY    0x20

/* Keyboard Keys Related Macros */
#define BREAKCODE_BIT   BIT(7)
#define MAKECODE(code)  (code)
#define BREAKCODE(code) ((code) | BREAKCODE_BIT)



/* Functions used in kbd.c */

/**
 * @brief Subscribes and enables Keyboard interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int kbd_subscribe_int();


/**
 * @brief Disables and unsubscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_int();


/**
 * @brief Writes a command with no arguments to the KBC command register
 *
 * @param cmd Command-Byte with no arguments to write in the command register, 0x64
 *
 * @return Returns 0 on sucess on writing the command and non-zero otherwise
 */
int kbd_write_cmd(unsigned char cmd);


/**
 * @brief Writes a command with an argument to the KBC command register
 *
 * @param cmd Command-Byte that takes an argument to write in the command register, 0x64
 * @param arg Command-Byte's argument, to write in the input buffer, 0x60
 *
 * @return Returns 0 on sucess on writing the command and the argument, and non-zero otherwise
 */
int kbd_write_cmd_witharg(unsigned char cmd, unsigned char arg);


/**
 * @brief Reads the KBC status register
 *
 * @param statByte Pointer to the byte that will contain the status register content after reading
 *
 * @return Returns 0 on sucess on reading the status register, and non-zero otherwise
 */
int kbd_read_statbyte(unsigned char* statByte);


/**
 * @brief Reads the KBC output buffer
 *
 * @param dataByte Pointer to the byte that will contain the output buffer content after reading
 *
 * @return Returns 0 on sucess on reading the output buffer content, and non-zero otherwise
 */
int kbd_read_outputbuffer(unsigned char* dataByte);


/**
 * @brief Flushes the output buffer, if it is full
 *
 * @return Returns 0 on success on flushing and non-zero otherwise
 */
int kbc_flush_outputbuffer();

/**
 * @brief Reads the kbc's command byte
 *
 * @param commandByte   Pointer to the byte that will contain the command byte after reading
 *
 * @return Return 0 on sucessful reading and non-zero otherwise
 */
int kbd_read_kbc_commandByte(unsigned char * commandByte);

/**
 * @brief Gets the code byte
 *
 * @return Returns codeByte static variable
 */
unsigned char kbc_get_codebyte();


/**
 * @brief Keyboard interrupt handler. Extern from assembly_kbd.S
 *
 * @return Returns 0 on success after reading the break/make code, and non-zero otherwise
 */
extern int kbd_int_handler();   // Extern from assembly_kbd.S

/**
 * @brief Handles the player's input from the keyboard, raising events
 *
 * @return void
 */
void handle_keyboard();

/** @} end of kbd */

#endif
