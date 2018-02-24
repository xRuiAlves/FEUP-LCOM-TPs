#ifndef _LCOM_UTILITIES_H_
#define _LCOM_UTILITIES_H_

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
 * @brief Get the absolute value of a number
 *
 * @param value Value of which we want to obtain the absolute value
 *
 * @return Returns the absolute value of 'value' parameter
 */
unsigned int absoluteValue(int value);





#endif /* _LCOM_UTILITIES_H_ */
