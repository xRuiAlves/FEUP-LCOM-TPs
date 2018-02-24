#include "utilities.h"


int convertBinaryToDecimal(unsigned char delta, unsigned char sign){

    // Return may range from -256 to 255

    if (sign == 0){         // Positive number
        return delta;
    }
    else{                   // Negative number
        if(delta == 0)
            return -256;
        else
            return -((delta^0xFF)+1);    // 2's complement conversion
    }
}


int round(double val){
    // Get decimal value
    char decimal_val = (int)(val*10)%10;

    if (decimal_val < 0)  decimal_val = -decimal_val;

    if(decimal_val < 5)
        return ( (int)val );
    else{
        if (val>0)
            return ( (int)val + 1 );
        else
            return ( (int)val - 1 );
    }
}


unsigned int convert_byte_to_BCD(u_char byte){
    u_char lowerDigit = (byte & 0x0F);
    u_char higherDigit = (byte & 0xF0) >> 4;

    return (lowerDigit + higherDigit*10);
}
