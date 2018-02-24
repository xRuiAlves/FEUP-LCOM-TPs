#ifndef _KBD_H_
#define _KBD_H_

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "stdio.h"

/* Function for counting sys_inb() calls */
#ifdef LAB3
int sys_inb_cnt(port_t port, unsigned long *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif


/* Symbolic constants */
#define KBD_SUCCESS             0
#define KBD_ERROR_OCCURED       1
#define KBD_MAX_INBUFF_TRIES    2       // 2 tries correspong to about ~40 miliseconds


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
 * @brief Keyboard interrupt handler
 *
 * @return Returns 0 on success after reading the break/make code, and non-zero otherwise
 */
int kbd_c_int_handler();


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
 * @brief Prints the make/break code on the screen
 *
 * @details Depending on if it is a 1-Byte or a 2-Byte Make/Break code, the function will ignore or not the extraByte parameter,
 * in order to print the code in an organized way. The typeOfCode argument specifies what kind of code it is going to print,
 * be it a 1/2 Byte Make/Break code. code represents the code itself.
 *
 * @param typeOfCode  Type of code to print, 1 (1-Byte Makecode), 2 (1-Byte Breakcode), 3 (2-Byte Makecode) or 4 (2-Byte Breakcode)
 * @param code  The Break/Make code byte to write on the screen
 * @param extraByte The extra byte when the code to print is a 2-Byte code. If the typeOfCode is 0 or 1 (1-Byte codes), this parameter is ignored.
 */
void kbd_print_code(short typeOfCode, unsigned char code, unsigned char extraByte);

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


#endif
