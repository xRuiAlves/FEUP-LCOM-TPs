#ifndef _PORTA_SERIE_H_
#define _PORTA_SERIE_H_

#include "utilities.h"

/** @defgroup portaserie portaserie
 * @{
 * Functions for manipulating the serial port
 */

#define UART_READ_LSR_FAILED            1   /**< @brief Failed to read from LSR */
#define UART_READ_RBR_FAILED            2   /**< @brief Failed to read from RBR */
#define UART_READ_IER_FAILED            3   /**< @brief Failed to read from IER */
#define UART_READ_IIR_FAILED            4   /**< @brief Failed to read from IIR */
#define UART_READ_LCR_FAILED            5   /**< @brief Failed to read from LCR */
#define UART_READ_DLL_FAILED            6   /**< @brief Failed to read from DLL */
#define UART_READ_DLM_FAILED            7   /**< @brief Failed to read from DLM */
#define UART_WRITE_LCR_FAILED          -1   /**< @brief Failed to write to LCR */
#define UART_WRITE_IER_FAILED          -2   /**< @brief Failed to write to IER */
#define UART_WRITE_DLL_FAILED          -3   /**< @brief Failed to write to DLL */
#define UART_WRITE_DLM_FAILED          -4   /**< @brief Failed to write to DLM */
#define UART_WRITE_THR_FAILED          -5   /**< @brief Failed to write to THR */
#define UART_ENABLE_DL_ACCESS_FAILED   -6   /**< @brief Failed to enable divisor latch access */
#define UART_DISABLE_DL_ACCESS_FAILED  -7   /**< @brief Failed to disable divisor latch access */
#define UART_SET_BIT_RATE_FAILED       -8   /**< @brief Failed to set uart's bit-rate */
#define UART_THR_NOT_EMPTY             -9   /**< @brief Transmitter Holding Register not empty */
#define UART_INT_SUBS_FAILED           -10  /**< @brief Failed to subscribe uart's interrupts */
#define UART_INT_UNSUBS_FAILED         -11  /**< @brief Failed to un-subscribe uart's interrupts */
#define UART_WRITE_CHAR_FAILED         -12  /**< @brief Failed to write a character */
#define UART_WRITE_MESSAGE_FAILED      -13  /**< @brief Failed to write a message */

#define UART_BIT_RATE_LSB(bit_rate) (  (bit_rate) & 0x00FF)
#define UART_BIT_RATE_MSB(bit_rate) ( ((bit_rate) & 0xFF00) >> 8 )



/**
 * @brief Subscribes and enables the uart interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int uart_subscribe_int();

/**
* @brief Disables and unsubscribes uart interrupts
*
* @return Return 0 upon success and non-zero otherwise
 */
int uart_unsubscribe_int();

/**
 * @brief Reads the Line Status Register (LSR)
 *
 * @param status - Pointer to the byte that will contain the Line Status Register content after reading
 *
 * @return Returns 0 on sucess on reading, and non-zero otherwise
 */
int uart_read_LSR(u_char* status);

/**
 * @brief Reads the Receiver Buffer Register (RBR)
 *
 * @param content - Pointer to the byte that will contain the Receiver Buffer Register content after reading
 *
 * @return Returns 0 on sucess on reading, and non-zero otherwise
 */
int uart_read_RBR(u_char* content);

/**
 * @brief Reads the Interrupt Enable Register (IER)
 *
 * @param content - Pointer to the byte that will contain the Interrupt Enable Register content after reading
 *
 * @return Returns 0 on sucess on reading, and non-zero otherwise
 */
int uart_read_IER(u_char* content);

/**
 * @brief Reads the Interrupt Identification Register (IIR)
 *
 * @param content - Pointer to the byte that will contain the Interrupt Identification Register content after reading
 *
 * @return Returns 0 on sucess on reading, and non-zero otherwise
 */
int uart_read_IIR(u_char* content);

/**
 * @brief Reads the Line Control Register (LCR)
 *
 * @param content - Pointer to the byte that will contain the Line Control Register content after reading
 *
 * @return Returns 0 on sucess on reading, and non-zero otherwise
 */
int uart_read_LCR(u_char* content);

/**
 * @brief Reads the Divisor Latch Least Significant Byte Register (DLL)
 *
 * @param content - Pointer to the byte that will contain the Divisor Latch Least Significant Byte Register content after reading
 *
 * @return Returns 0 on sucess on reading, and non-zero otherwise
 */
int uart_read_DLL(u_char* content);

/**
 * @brief Reads the Divisor Latch Most Significant Byte Register (DLM)
 *
 * @param content - Pointer to the byte that will contain the Divisor Latch Most Significant Byte Register content after reading
 *
 * @return Returns 0 on sucess on reading, and non-zero otherwise
 */
int uart_read_DLM(u_char* content);

/**
 * @brief Writes a byte in the Transmitter Holding Register (THR)
 *
 * @param data - Byte to write in the Transmitter Holding Register
 *
 * @return Returns 0 on sucess on writing, and non-zero otherwise
 */
int uart_write_THR(u_char data);

/**
 * @brief Writes a byte in the Line Control Register (LCR)
 *
 * @param data - Byte to write in the Line Control Register
 *
 * @return Returns 0 on sucess on writing, and non-zero otherwise
 */
int uart_write_LCR(u_char data);

/**
 * @brief Writes a byte in the Interrupt Enable Register (IER)
 *
 * @param data - Byte to write in the Interrupt Enable Register
 *
 * @return Returns 0 on sucess on writing, and non-zero otherwise
 */
int uart_write_IER(u_char data);

/**
 * @brief Writes a byte in the Divisor Latch Least Significant Byte Register (DLL)
 *
 * @param data - Byte to write in the Divisor Latch Least Significant Byte Register
 *
 * @return Returns 0 on sucess on writing, and non-zero otherwise
 */
int uart_write_DLL(u_char data);

/**
 * @brief Writes a byte in the Divisor Latch Most Significant Byte Register (DLM)
 *
 * @param data - Byte to write in the Divisor Latch Most Significant Byte Register
 *
 * @return Returns 0 on sucess on writing, and non-zero otherwise
 */
int uart_write_DLM(u_char data);

/**
 * @brief Enables access to the divisor latch registers, by setting LCR's bit 7 to 1
 *
 * @return Returns 0 on sucess and non-zero otherwise
 */
int uart_enable_divisor_latch_access();

/**
 * @brief Enables access to the divisor latch registers, by setting LCR's bit 7 to 0
 *
 * @return Returns 0 on sucess and non-zero otherwise
 */
int uart_disable_divisor_latch_access();

/**
 * @brief Sets the uart bit-rate
 *
 * @param bit_rate - New UART's bit-rate
 *
 * @return Returns 0 on sucess and non-zero otherwise
 */
int uart_set_bit_rate(u_short bit_rate);

/**
 * @brief Flushes the content of the Receiver Buffer Register (RBR) , it is is full
 */
void uart_flush_RBR();

/**
 * @brief Writes a character in the Transmitter Holding Register (After polling LSR to check if it is empty)
 *
 * @param character - Character to send
 *
 * @return Returns 0 on sucess and non-zero otherwise
 */
int uart_write_character(u_char character);

/**
 * @brief Writes a full message (Header Byte + Content + Trailer Byte)
 *
 * @param message_content - Message Content to Send
 *
 * @return Returns 0 on sucess and non-zero otherwise
 */
int uart_write_message(u_char message_content);

/**
 * @brief Reads an acknowledgement byte from the Receiver Buffer Register
 *
 * @return Returns SPP_ACK_BYTE(0xFA) on acknowledgement , SPP_NACK_BYTE(0xFE) on no-acknoledgement and SPP_ERROR_BYTE(0xFC) on error
 */
u_char uart_get_acknowledgement_byte();

/**
 * @brief Passes a message character to the right place in the message array.
 *
 * @details It checks, based on the current message status, if the correct byte is received (Header , Content or Trailer) and saves it in the correct place in the array, notifying the function caller if there was any error and if the message is complete/incomplete.
 *
 * @return Returns 0 when message is complete, 1 if the message is not complete yet (message building in progress) or 2 or 3 if there's a corrupted header or trailer byte.
 */
int uart_parse_message();


/**
 * @brief Handles the interrupts from the uart, raising events
 *
 * @return void
 */
void handle_uart();

 /** @} end of portaserie */

#endif /* _PORTA_SERIE_H_ */
