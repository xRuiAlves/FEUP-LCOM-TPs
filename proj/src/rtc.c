#include "rtc.h"
#include "stdio.h"
#include <minix/syslib.h>
#include <minix/driver.h>
#include "Game.h"
#include "SnakeyNix.h"

static int rtcHookId = 8;


/*
   @brief Writes a register adress to 0x70 (RTC_ADDRESS_REG) so we can read/write from/in the desired register

   @param reg - the adress of the register in which we want to write/read

   @return OK(0) in case of success or RTC_WRITING_FAILED(2) in case of failure
 */
static int rtc_write_cmd(u_char reg){

    // Writing in 0x70
    if(sys_outb(RTC_ADDRESS_REG, reg) != OK)   // Check if the Writingwas successful
        return RTC_WRITING_FAILED;  // Return failure if the writing wasn't successful

    else
        return OK;     // Return success if the writing was successful

}


int rtc_subscribe_int(){

    int rtcMaskBit = rtcHookId;   // The bit we are using for RTC interrupts

    // Subscribe the RTC interrupts
    if(sys_irqsetpolicy(RTC_IRQ , IRQ_REENABLE , &rtcHookId) != OK)
        return RTC_SUBSCRIBE_FAILED;    // Error occured, return RTC_SUBSCRIBE_FAILED

    // Enable RTC interrupts
    if(sys_irqenable(&rtcHookId) != OK)
        return RTC_SUBSCRIBE_FAILED;
    else
        return rtcMaskBit; // No error occured, return the mask bit for the subscribed interrupt
}

int rtc_enable_alarm_int(){

    u_char data;

    // Reading the data of Register B
    if(rtc_read_reg(REGISTER_B, &data) != OK){
        return RTC_READING_FAILED;
    }

    data = data | ALARM_INTERRUPT_ENABLE;   // To set bit 5 to 1

    // Writes the new data on Register B
    return rtc_write_reg(REGISTER_B, data);
}

int rtc_disable_alarm_int(){

    u_char data;

    // Reading the data of Register B
    if(rtc_read_reg(REGISTER_B, &data) != OK){
        return RTC_READING_FAILED;
    }

    u_char mask = (u_char) ~ALARM_INTERRUPT_ENABLE;
    data = (data & mask);   // Set bit 5 to 0

    // Writes the new data on Register B
    return rtc_write_reg(REGISTER_B, data);
}

int rtc_enable_periodic_int(){

    u_char data;

    // Reading the data of Register B
    if(rtc_read_reg(REGISTER_B, &data) != OK){
        return RTC_READING_FAILED;
    }

    data = data | PERIODIC_INTERRUPT_ENABLE;   // To set bit 6 to 1

    // Writes the new data on Register B
    return rtc_write_reg(REGISTER_B, data);
}

int rtc_disable_periodic_int(){
    u_char data;

    // Reading the data of Register B
    if(rtc_read_reg(REGISTER_B, &data) != OK){
        return RTC_READING_FAILED;
    }

    u_char mask = (u_char) ~PERIODIC_INTERRUPT_ENABLE;
    data = (data & mask);   // Set bit 6 to 0

    // Writes the new data on Register B
    return rtc_write_reg(REGISTER_B, data);
}

int rtc_enable_update_int(){

    u_char data;

    // Reading the data of Register B
    if(rtc_read_reg(REGISTER_B, &data) != OK){
        return RTC_READING_FAILED;
    }

    data = data | UPDATE_INTERRUPT_ENABLE;   // To set bit 4 to 1

    // Writes the new data on Register B
    return rtc_write_reg(REGISTER_B, data);
}

int rtc_disable_update_int(){
    u_char data;

    // Reading the data of Register B
    if(rtc_read_reg(REGISTER_B, &data) != OK){
        return RTC_READING_FAILED;
    }

    u_char mask = (u_char) ~UPDATE_INTERRUPT_ENABLE;
    data = (data & mask);   // Set bit 4 to 0

    // Writes the new data on Register B
    return rtc_write_reg(REGISTER_B, data);
}

int rtc_unsubscribe_int(){

    // Disable RTC interrupts
    if(sys_irqdisable(&rtcHookId) != OK)
        return RTC_UNSUBSCRIBE_FAILED;

    // Unsubscribe RTC interrupts and return failure/success
    return (sys_irqrmpolicy(&rtcHookId));
}


int rtc_write_reg(u_char reg, u_char data){

    // Write in RTC_ADDRESS_REG (0x70) the adress of the register we want to write
    if(rtc_write_cmd(reg) != OK)
        return RTC_WRITING_FAILED;  // Return failure if writing in 0x70 fails

    else{
        return (sys_outb(RTC_DATA_REG, data));
    }

}


int rtc_read_reg(u_char reg, u_char * data){

    u_long regInfo;

    // Write in RTC_ADDRESS_REG (0x70) the adress of the register we want to read
    if(rtc_write_cmd(reg) != OK)
        return RTC_WRITING_FAILED;  // Return failure if writing in 0x70 fails

    else{
        // Read the content of the desired register by writing itś adress to 0x71
        if(sys_inb(RTC_DATA_REG, &regInfo) != OK)
            return RTC_READING_FAILED;

        else{
            *data = (u_char)regInfo;
            return OK;     // Return success upon a successful reading!
        }
    }

}


int rtc_read_year(){

    u_char year;

    // Read the year register
    if(rtc_read_reg(RTC_YEAR_ADRESS, &year) != OK){
        return RTC_READING_FAILED;
    }
    else{
        return year;  // Return the year
    }
}


int rtc_read_month(){

    u_char month;

    // Read the month register
    if(rtc_read_reg(RTC_MONTH_ADRESS, &month) != OK){
        return RTC_READING_FAILED;
    }
    else{
        return month;  // Return the month
    }
}


int rtc_read_day(){

    u_char day;

    // Read the day register
    if(rtc_read_reg(RTC_DATE_OF_MONTH_ADRESS, &day) != OK){
        return RTC_READING_FAILED;
    }
    else{
        return day;  // Return the day
    }
}


int rtc_read_hour(){

    u_char hour;

    // Read the hours register
    if(rtc_read_reg(RTC_HOURS_ADRESS, &hour) != OK){
        return RTC_READING_FAILED;
    }
    else{
        return hour;  // Return the hour
    }
}


int rtc_read_minute(){

    u_char minute;

    // Read the minutes register
    if(rtc_read_reg(RTC_MINUTES_ADRESS, &minute) != OK){
        return RTC_READING_FAILED;
    }
    else{
        return minute;  // Return the minutes
    }
}


int rtc_read_second(){

    u_char second;

    // Read the seconds register
    if(rtc_read_reg(RTC_SECONDS_ADRESS, &second) != OK){
        return RTC_READING_FAILED;
    }
    else{
        return second;  // Return the seconds
    }
}


int rtc_set_seconds_alarm(char value){
    return rtc_write_reg(RTC_SECONDS_ALARM_ADRESS, value);  // Writes in the adress of the seconds alarm register the value passed by parameter
}


int rtc_set_minutes_alarm(char value){
return rtc_write_reg(RTC_MINUTES_ALARM_ADRESS, value);   // Writes in the adress of the minutes alarm register the value passed by parameter
}


int rtc_set_hours_alarm(char value){
 return rtc_write_reg(RTC_HOURS_ALARM_ADRESS, value);   // Writes in the adress of the hours alarm register the value passed by parameter
}


int rtc_set_alarm(u_char num_seconds){

    int second = convert_BCD_to_decimal(rtc_read_second());  // Converts to decimal so we can add num_seconds

    if(rtc_set_seconds_alarm(convert_decimal_to_BCD((second+num_seconds)%60)) != OK){   // Converts to BCD again to write in the register
        return RTC_WRITING_FAILED;
    }
    if(rtc_set_minutes_alarm(REGISTER_DONT_CARE_VALUE) != OK){
        return RTC_WRITING_FAILED;
    }
    if(rtc_set_hours_alarm(REGISTER_DONT_CARE_VALUE) != OK){
        return RTC_WRITING_FAILED;
    }

    return OK;
}


void handle_rtc(){
    u_char reg_c;

    // Read Register C to find out the source of the Interrupt
    if(rtc_read_reg(REGISTER_C, &reg_c) != OK)
        return;

    // Alarm interupt occured
    if (reg_c & ALARM_INTERRUPT_PENDING)
        raise_event(get_game_state_machine(getSnakeyNix()) , RTC_ALARM_RINGED);

}
