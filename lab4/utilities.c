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


unsigned int absoluteValue(int value){
    if (value >= 0)
        return value;
    else
        return (-value);
}
