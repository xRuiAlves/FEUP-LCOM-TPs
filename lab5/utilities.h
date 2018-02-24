#ifndef _LCOM_UTILITIES_H_
#define _LCOM_UTILITIES_H_

/* Usefull typedefs */
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned char u_char;


/* Usefull MACROS */
#define absolute_value(n) ( ((n)<0) ? (-(n)) : (n) )            // Absolute value of a number
#define swap_values(S,a,b) {S temp=a; a=b; b=temp;}             // Swap two generic values


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
unsigned int convert_byte_to_BCD(u_char byte);


#endif /* _LCOM_UTILITIES_H_ */
