#ifndef RTC_H_
#define RTC_H_

#include "utilities.h"

/** @defgroup rtc rtc
 * @{
 * Functions for manipulating the rtc
 */

#define BIT(n)              (0x01<<(n))

#define RTC_ADDRESS_REG     0x70
#define RTC_DATA_REG        0x71
#define RTC_IRQ             8        /**< @brief rtc IRQ line */

// Registers
#define RTC_SECONDS_ADRESS         0x00
#define RTC_SECONDS_ALARM_ADRESS   0x01
#define RTC_MINUTES_ADRESS         0x02
#define RTC_MINUTES_ALARM_ADRESS   0x03
#define RTC_HOURS_ADRESS           0x04
#define RTC_HOURS_ALARM_ADRESS     0x05
#define RTC_DAY_OF_WEEK_ADRESS     0x06
#define RTC_DATE_OF_MONTH_ADRESS   0x07
#define RTC_MONTH_ADRESS           0x08
#define RTC_YEAR_ADRESS            0x09
#define REGISTER_A                 0x0A
#define REGISTER_B                 0x0B
#define REGISTER_C                 0x0C
#define REGISTER_D                 0x0D

/* Register A */
#define UIP_FLAG                    BIT(7)   /**< @brief for verifying if the actualization of the rtc is happening at the moment */

/* Register B */
#define SET                         BIT(7)
#define PERIODIC_INTERRUPT_ENABLE   BIT(6)   /**< @brief To enable/disable the periodic interrupts */
#define ALARM_INTERRUPT_ENABLE      BIT(5)   /**< @brief To enable/disable the alarm interrupts */
#define UPDATE_INTERRUPT_ENABLE     BIT(4)   /**< @brief To enable/disable the update interrupts */

/* Register C */
#define IRQ_LINE_ACTIVE             BIT(7)
#define PERIODIC_INTERRUPT_PENDING  BIT(6)   /**< @brief To check if there is a periodic interrupt pending */
#define ALARM_INTERRUPT_PENDING     BIT(5)   /**< @brief To check if there is an alarm interrupt pending */
#define UPDATE_INTERRUPT_PENDING    BIT(4)   /**< @brief To check if there is an update interrupt pending */

// Symbolic constants
#define REGISTER_DONT_CARE_VALUE  0xFF       /**< @brief Don't care value for alarm registers */
#define RTC_UNSUBSCRIBE_FAILED    1
#define RTC_WRITING_FAILED        2
#define RTC_READING_FAILED       -3
#define RTC_SUBSCRIBE_FAILED     -2
#define RTC_SET_ALARM_FAILED     -1

/**
 * @brief Subscribes and enables the rtc interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int rtc_subscribe_int();

/**
* @brief Disables and unsubscribes rtc interrupts
*
* @return Return 0 upon success and non-zero otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief Enables the alarm interruptions by setting to 1 the bit 5 of the register B of the RTC
 *
 * @return Returns OK upon success and non-zero otherwise
 */
int rtc_enable_alarm_int();

/**
 * @brief Disables the alarm interruptions by setting to 0 the bit 5 of the register B of the RTC
 *
 * @return Returns OK upon success and non-zero otherwise
 */
int rtc_disable_alarm_int();

/**
 * @brief Enables the periodic interruptions by setting to 1 the bit 6 of the register B of the RTC
 *
 * @return Returns OK upon success and non-zero otherwise
 */
int rtc_enable_periodic_int();

/**
 * @brief Disables the periodic interruptions by setting to 0 the bit 6 of the register B of the RTC
 *
 * @return Returns OK upon success and non-zero otherwise
 */
int rtc_disable_periodic_int();

/**
 * @brief Enables the update interruptions by setting to 1 the bit 4 of the register B of the RTC
 *
 * @return Returns OK upon success and non-zero otherwise
 */
int rtc_enable_update_int();

/**
 * @brief Disables the update interruptions by setting to 0 the bit 4 of the register B of the RTC
 *
 * @return Returns OK upon success and non-zero otherwise
 */
int rtc_disable_update_int();

/**
 * @brief Writes the data passed by parameter on the register passed by parameter
 *
 * @param reg - the number of the register to write in
 * @param data - the data to write in the 'reg' register
 *
 * @return int - RTC_WRITING_FAILED(2) or OK(0)
 */
int rtc_write_reg(u_char reg, u_char data);

/**
 * @brief Reads the content of the register reg and puts it in the data variable
 *
 * @param reg - the number of the register to write in
 * @param data - the address of the variable in which to write the data of the chosen register
 *
 * @return RTC_WRITING_FAILED(2), RTC_READING_FAILED(3) or OK
 */
int rtc_read_reg(u_char reg, u_char * data);

/**
 * @brief Returns the current year
 *
 * @return int - year , or RTC_READING_FAILED(-2) if failure
 */
int rtc_read_year();

/**
 * @brief Returns the current month
 *
 * @return int - month , or RTC_READING_FAILED(-2) if failure
 */
int rtc_read_month();

/**
 * @brief Returns the current day
 *
 * @return int - day , or RTC_READING_FAILED(-2) if failure
 */
int rtc_read_day();

/**
 * @brief Returns the current hour
 *
 * @return int - hour , or RTC_READING_FAILED(-2) if failure
 */
int rtc_read_hour();

/**
 * @brief Returns the current minute
 *
 * @return int - minute , or RTC_READING_FAILED(-2) if failure
 */
int rtc_read_minute();

/**
 * @brief Returns the current second
 *
 * @return int - second , or RTC_READING_FAILED(-2) if failure
 */
int rtc_read_second();

/**
 * @brief Writes in the RTC_SECONDS_ALARM register the value 'value' passed by parameter. Accepts the parameter REGISTER_DONT_CARE_VALUE (0xFF), in case the value of this parameter in particular is not relevant.
 *
 * @param value - the value of the seconds on which to raise an interrupt
 *
 * @return Returns OK upon success and non-zero otherwise
 */
 int rtc_set_seconds_alarm(char value);

 /**
  * @brief Writes in the RTC_MINUTES_ALARM register the value 'value' passed by parameter. Accepts the parameter REGISTER_DONT_CARE_VALUE (0xFF), in case the value of this parameter in particular is not relevant.
  *
  * @param value - the value of the minutes on which to raise an interrupt
  *
  * @return Returns OK upon success and non-zero otherwise
  */
 int rtc_set_minutes_alarm(char value);

 /**
  * @brief Writes in the RTC_HOURS_ALARM register the value 'value' passed by parameter. Accepts the parameter REGISTER_DONT_CARE_VALUE (0xFF), in case the value of this parameter in particular is not relevant.
  *
  * @param value - the value of the hours on which to raise an interrupt
  *
  * @return Returns OK upon success and non-zero otherwise
  */
 int rtc_set_hours_alarm(char value);

 /**
  * @brief Programs the RTC to raise an Alarm Interruption after a delay of 'seconds' number of seconds.
  *
  * @param num_seconds - the interval of seconds to raise the requested interrupt. Note that 'seconds' is suposed to be a value between 0 and 60, if not, it will be turned into one on the calculations.
  *
  * @return Returns OK upon success and non-zero otherwise
  */
  int rtc_set_alarm(u_char num_seconds);

  /**
   * @brief Handles the interrupts from the rtc, raising events
   *
   * @return void
   */
  void handle_rtc();


/** @} end of rtc */

#endif /* RTC_H_ */
