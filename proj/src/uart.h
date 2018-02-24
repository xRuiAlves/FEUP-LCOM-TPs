#ifndef _UART_H_
#define _UART_H_

/** @defgroup uart uart
 * @{
 *
 * Constants for programming the Universal Asynchronous Receiver/ Transmitter (UART)
 */

#define BIT(n)              (0x01<<(n))

#define UART_COM1_IRQ            4        /**< @brief UART COM1 IRQ Line  */
#define UART_COM2_IRQ            3        /**< @brief UART COM2 IRQ Line  */

#define UART_DL_BITRATE_DIVISOR  115200    /**< Divisor latch value must be set to 115200/bit_rate to set the bit_rate */

//////////////////////////
//  I/O port addresses  //
//////////////////////////

#define UART_COM1_BASE_ADDR    0x3F8 /**< @brief UART's COM1 base address register */
#define UART_COM2_BASE_ADDR    0x2F8 /**< @brief UART's COM2 base address register */

///////////////////////////////////////////////////
//  UART's accessible 8-bit Registers (offsets)  //
///////////////////////////////////////////////////

#define UART_RBR  0    /**< @brief Receiver Buffer Register */
#define UART_THR  0    /**< @brief Transmitter Holding Register */
#define UART_IER  1    /**< @brief Interrupt Enable Register */
#define UART_IIR  2    /**< @brief Interrupt Identification Register */
#define UART_FCR  2    /**< @brief FIFO Control Register */
#define UART_LCR  3    /**< @brief Line Control Register */
#define UART_MCR  4    /**< @brief Modem Control Register */
#define UART_LSR  5    /**< @brief Line Status Register */
#define UART_MSR  6    /**< @brief Modem Status Register */
#define UART_SR   7    /**< @brief Scratchpad Register */

#define UART_DLL  0    /**< @brief Divisor Latch LSB */
#define UART_DLM  1    /**< @brief Divisor Latch MSB */


///////////////////////////////////
//  Line Control Register (LCR)  //
///////////////////////////////////

/* Number of Bits per Char */
#define UART_LCR_5BPC   0                           /**< @brief 5 bits per char */
#define UART_LCR_6BPC   BIT(0)                      /**< @brief 6 bits per char */
#define UART_LCR_7BPC   BIT(1)                      /**< @brief 7 bits per char */
#define UART_LCR_8BPC   (BIT(0) | BIT(1))           /**< @brief 8 bits per char */

/* Number of Stop Bits */
#define UART_LCR_1_STOPBIT  0                       /**< @brief 1 stop bit */
#define UART_LCR_2_STOPBIT  BIT(2)                  /**< @brief 2 stop bits (1 and 1/2 when 5 bits char) */

/* Parity Control */
#define UART_LCR_NO_PARITY    0                     /**< @brief No parity */
#define UART_LCR_ODD_PARITY   BIT(3)                /**< @brief Odd parity */
#define UART_LCR_EVEN_PARITY  (BIT(3) | BIT(4))     /**< @brief Even parity */

/* Break Control */
#define UART_LCR_BREAK_CTRL   BIT(6)                /**< @brief Break control: sets serial output to 0 (low) */

/* Divisor Latch Access */
#define UART_LCR_DLAB   BIT(7)                      /**< @brief 0 - Divisor Latch Access / 1 - RBR (read) or THR (write) */


//////////////////////////////////
//  Line Status Register (LSR)  //
//////////////////////////////////

#define UART_LSR_RCV_DATA    BIT(0)    /**< @brief Set to 1 when there is data for receiving */
#define UART_LSR_OVR_ERROR   BIT(1)    /**< @brief Set to 1 when a characters received is overwritten by another one */
#define UART_LSR_PAR_ERROR   BIT(2)    /**< @brief Set to 1 when a character with a parity error is received */
#define UART_LSR_FRM_ERROR   BIT(3)    /**< @brief Set to 1 when a received character does not have a valid Stop bit */
#define UART_LSR_BRK_INT     BIT(4)    /**< @brief Set to 1 when the serial data input line is held in the low level for longer than a full “word” transmission */
#define UART_LSR_THRE        BIT(5)    /**< @brief When set, means that the UART is ready to accept a new character for transmitting */
#define UART_LSR_TER         BIT(6)    /**< @brief When set, means that both the THR and the Transmitter Shift Register are both empty */
#define UART_LSR_FIFO_ERROR  BIT(7)    /**< @brief Set to 1 when there is at least one parity error or framing error or break indication in the FIFO Reset to 0 when the LSR is read, if there are no subsequent errors in the FIFO */


///////////////////////////////////////
// Interrupt Enabler Register (IER)  //
///////////////////////////////////////

#define UART_IER_RCVD_DATA_INT   BIT(0)   /**< @brief Enables the Received Data Available Interrupt */
#define UART_IER_THRE_INT        BIT(1)   /**< @brief Enables the Transmitter Holding Register Empty Interrupt */
#define UART_IER_ERROR_INT       BIT(2)   /**< @brief Enables the Receiver Line Status Interrupt This event is generated when there is a change in the state of bits 1 to 4, i.e. the error bits, of the LSR */
#define UART_IER_MODEM_STAT_INT  BIT(3)   /**< @brief Enables the MODEM Status Interrupt */


//////////////////////////////////////////////
// Interrupt Identification Register (IIR)  //
//////////////////////////////////////////////

#define UART_IIR_NO_INT          BIT(0)              /**< @brief If set, no interrupt is pending */
#define UART_IIR_RLS             (BIT(1) | BIT(2))   /**< @brief Receiver Line Status */
#define UART_IIR_RDA             BIT(2)              /**< @brief Reived Data Available */
#define UART_IIR_FIFO_C_TIMEOUT  (BIT(2) | BIT(3))   /**< @brief Character Timeout (FIFO) */
#define UART_IIR_THRE            BIT(1)              /**< @brief Transmitter Holding Register Empty */
#define UART_IIR_MODEM_STATUS    0                   /**< @brief Modem Status */
#define UART_INT_ID_MASK         (BIT(1) | BIT(2) | BIT(3))

/** @} end of uart */

#endif /* _UART_H_ */
