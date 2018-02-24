#ifndef _LCOM_UTILITIES_H_
#define _LCOM_UTILITIES_H_

/** @defgroup utilities utilities
 * @{
 * Functions for general purposes, useful funcionalities
 */

/* Usefull typedefs */
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned char u_char;


/* Usefull MACROS */
#define absolute_value(n) ( ((n)<0) ? (-(n)) : (n) )            // Absolute value of a number
#define swap_values(S,a,b) {S temp=a; a=b; b=temp;}             // Swap two generic values


/* Usefull Symbolic Constants */
#define UART_CONFIG_FAILED  1
#define RTC_CONFIG_FAILED   2


/**
 * @brief Converts any binary in 2's complement to decimal
 *
 * @param delta - number to convert mantissa
 * @param sign - the sign of the number we are to convertBinaryToDecimal
 *
 * @return Returns the converted number (in decimal), may range from -256 to 255
 */
int convertBinaryToDecimal(unsigned char delta, unsigned char sign);


/**
 * @brief rounds a value
 *
 * @param val Value to round
 *
 * @return Rounded value
 */
int round(double val);


/**
 * @brief Converts a byte to BCD
 *
 * @param byte - Byte to be converted to decimal using BCD
 *
 * @return Returns the decimal interpretation of the byte in BCD
 */
 u_int convert_BCD_to_decimal(u_char byte);

 /**
  * @brief Converts a decimal value to BCD
  *
  * @param decimal_val - decimal value to be converted to BCD
  *
  * @return Returns the BCD interpretation of the decimal value
  */
 u_char convert_decimal_to_BCD(u_int decimal_val);

/**
 * @brief Prints the date in the format "DD/MM/YYYY HH:MM:SS"
 *
 * @return void
 */
void print_date();

/**
 * @brief Gets the actual time and date
 *
 * @return Returns a "string" (char*) with the time and date
 */
char* get_date();

/**
 * @brief Configures the UART for SnakeyNix.
 *
 * @return Return 0 on sucess, and non-zero otherwise
 */
int configure_uart_SnakeyNix();

/**
 * @brief Configures the RTC for SnakeyNix.
 *
 * @return Return 0 on sucess, and non-zero otherwise
 */
int configure_rtc_SnakeyNix();




/** @} end of utilities */

#endif /* _LCOM_UTILITIES_H_ */
