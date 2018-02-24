#include "utilities.h"
#include <minix/driver.h>
#include "rtc.h"
#include "uart.h"
#include "portaserie.h"
#include "stdio.h"


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


u_int convert_BCD_to_decimal(u_char byte){
    u_char lowerDigit = (byte & 0x0F);
    u_char higherDigit = (byte & 0xF0) >> 4;

    return (lowerDigit + higherDigit*10);
}

u_char convert_decimal_to_BCD(u_int decimal_val){
    u_char lowerDigit = decimal_val%10;
    u_char higherDigit = (decimal_val/10)%10;

    u_char result = (higherDigit << 4) + lowerDigit;

    return result;
}

void print_date(){

    u_char year, month, day, hour, minute, second;

    day = rtc_read_day();
    printf("%x/", day);

    month = rtc_read_month();
    printf("%x/", month);

    year = rtc_read_year();
    printf("20%x", year);

    hour = rtc_read_hour();
    printf("  %x:", hour);

    minute = rtc_read_minute();
    printf("%02x:", minute);

    second = rtc_read_second();
    printf("%02x\n", second);

}

char* get_date(){

    u_char day = rtc_read_day();
    u_char month = rtc_read_month();
    u_char year = rtc_read_year();
    u_char hour = rtc_read_hour();
    u_char minute = rtc_read_minute();
    u_char second = rtc_read_second();

    static char date[20];

    sprintf(date, "%02x/%02x/20%02x %02x:%02x:%02x", day, month, year, hour, minute, second);

    return date;
}

int configure_uart_SnakeyNix(){
    // Line Control Register Byte - 8 bits per character, with 2 stop bits and even parity verification
    u_char line_control_register_byte = (UART_LCR_8BPC | UART_LCR_2_STOPBIT | UART_LCR_EVEN_PARITY);

    // Interrupt Enable Register Byte - Received Data Available Interupts and Error Interrupts
    u_char interrupt_enable_register_byte = ( UART_IER_RCVD_DATA_INT | UART_IER_ERROR_INT );

    // Write the Interrupt Enable Register Byte
    if (uart_write_IER(interrupt_enable_register_byte) != OK)
        return UART_CONFIG_FAILED;

    // Write the Line Control Register Byte
    if (uart_write_LCR(line_control_register_byte) != OK)
        return UART_CONFIG_FAILED;

    // Set the correct bit-rate
    if (uart_set_bit_rate(9600) != OK)
        return UART_CONFIG_FAILED;

    // Flush the buffer to make sure it is clean
    uart_flush_RBR();

    return OK;
}

int configure_rtc_SnakeyNix(){
    // Enable Alarm Interrupts
    if (rtc_enable_alarm_int() != OK)
        return RTC_CONFIG_FAILED;

    // Disable Periodic Interrupts
    if (rtc_disable_periodic_int() != OK)
        return RTC_CONFIG_FAILED;

    // Disable Update Interrupts
    if (rtc_disable_update_int() != OK)
        return RTC_CONFIG_FAILED;

    return OK;
}
