#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include "stdio.h"
#include "i8254.h"
#include "timer.h"

int timerHookId = 0;
unsigned int timerIntCounter = 0;

int timer_set_frequency(unsigned char timer, unsigned long freq) {

	if (timer<0 || timer>2)		// timer out of range.
		return TIMER_ERROR_OCCURED;

	// check if freq value is nor too low nor too high.
	if (freq < 19)				// the timer LSB+MSB can only store numbers from 0 up to 65535 ; if freq is less than 19, TIMER_FREQ/freq is bigger than 65535 (2^16-1)
		return TIMER_ERROR_OCCURED;


	if (freq > TIMER_FREQ)	// TIMER_FREQ/TIMER_FREQ = 1, any value for freq higher than TIMER_FREQ will cause the result to be 0
		return TIMER_ERROR_OCCURED;

	// get the timer configuration
	unsigned char config;

	if(timer_get_conf(timer, &config) != TIMER_SUCCESS)
		return TIMER_ERROR_OCCURED;

	// save the counting mode and BCD
	unsigned char countingModeMask = ( BIT(3) | BIT(2) | BIT(1) );
	unsigned char bcdMask = BIT(0);

	unsigned char countingMode = (config & countingModeMask);
	unsigned char bcd = (config & bcdMask);


	// write the control word and change the timer
	unsigned char selectedTimer;

	switch(timer){
	case 0:
		selectedTimer = TIMER_0;
		break;
	case 1:
		selectedTimer = TIMER_1;
		break;
	case 2:
		selectedTimer = TIMER_2;
		break;
	}

	// write the control word
	unsigned long controlWord = (selectedTimer | TIMER_LSB_MSB | countingMode | bcd);
	if(sys_outb(TIMER_CTRL, controlWord) != TIMER_SUCCESS)
		return TIMER_ERROR_OCCURED;


	// write the desired frequency in the timer's LSB and MSB
	unsigned long timerFrequency = TIMER_FREQ / freq;

	unsigned long timerLsbMask = 0x00ff;		//0x00ff -> least significant byte
	unsigned long timerMsbMask = 0xff00;		//0xff00 -> most significant byte

	unsigned long timerLSB = timerFrequency & timerLsbMask;
	unsigned long timerMSB = (timerFrequency & timerMsbMask) >> 8;

	if(sys_outb(selectedTimer, timerLSB) != TIMER_SUCCESS)		// write the LSB
		return TIMER_ERROR_OCCURED;

	if(sys_outb(selectedTimer, timerMSB) != TIMER_SUCCESS)		// write the MSB
		return TIMER_ERROR_OCCURED;

	// success! no errors occured!
	return TIMER_SUCCESS;
}

int timer_subscribe_int(void ) {
	int timerMaskBit = timerHookId; 	// Bit we are using for timer interrupts

	// Subcribe timer interrupt
	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timerHookId) != TIMER_SUCCESS)
		return -1;		// Error occured, return negative value

	// Enable timer interrupts
	if(sys_irqenable(&timerHookId) != TIMER_SUCCESS)
		return -1;
	else
		return timerMaskBit;	// No error occured, return the mask bit for the subscribed interrupt
}

int timer_unsubscribe_int() {
	// Disable timer interrupts
	if(sys_irqdisable(&timerHookId) != TIMER_SUCCESS)
		return TIMER_ERROR_OCCURED;

	// Unsubscribe timer interrupt and return failure/success
	return sys_irqrmpolicy(&timerHookId);
}

void timer_int_handler() {
	timerIntCounter++;		// Increment the global timer interruption counter
}

int timer_get_conf(unsigned char timer, unsigned char *st) {

	if (timer < 0 || timer > 2)	// timer out of range
		return TIMER_ERROR_OCCURED;

	// write-back command for getting timer status
	unsigned long readBackCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | ~TIMER_RB_STATUS_ | TIMER_RB_SEL(timer);
	if(sys_outb(TIMER_CTRL, readBackCommand) != TIMER_SUCCESS)
		return TIMER_ERROR_OCCURED;

	// read the timer status
	unsigned long timerStatus;
	unsigned char selectedTimer;

	switch(timer){
	case 0:
		selectedTimer = TIMER_0;
		break;
	case 1:
		selectedTimer = TIMER_1;
		break;
	case 2:
		selectedTimer = TIMER_2;
		break;
	}

	if(sys_inb(selectedTimer, &timerStatus) != TIMER_SUCCESS)
		return TIMER_ERROR_OCCURED;

	// change content of st and return SUCCESS
	*st = (unsigned char)timerStatus;
	return TIMER_SUCCESS;
}

int timer_display_conf(unsigned char conf) {

	// make the masks for configuration analising
	unsigned char counterInitializationMask = ( BIT(5) | BIT(4) );
	unsigned char modeMask = ( BIT(3) | BIT(2) | BIT(1) );
	unsigned char bcdMask = BIT(0);


	// analize the configuration
	unsigned char counterInitialization = (conf & counterInitializationMask);
	unsigned char mode = (conf & modeMask);
	unsigned char bcd = (conf & bcdMask);


	// Counter Initialization
	printf("\nCOUNTER INITIALIZATION:\n");
	if(counterInitialization == TIMER_LSB){
		printf("Initialize Counter LSB only\n\n");
	}
	else if(counterInitialization == TIMER_MSB){
		printf("Initialize Counter MSB only\n\n");
	}
	else if(counterInitialization == TIMER_LSB_MSB){
		printf("Initialize Counter LSB first and Counter MSB afterwards\n\n");
	}


	// Operating Mode
	printf("OPERATING MODE:\n");
	if(mode == 0){							// mode 0
		printf("Mode 0 - Interrupt on terminal count\n\n");
	}
	else if(mode == BIT(0)){				// mode 1
		printf("Mode 1 - Hardware Retriggerable One-Shot\n\n");
	}
	else if(mode == TIMER_RATE_GEN){		// mode 2
		printf("Mode 2 - Rate Generator\n\n");
	}
	else if(mode == TIMER_SQR_WAVE){		// mode 3
		printf("Mode 3 - Square Wave Mode\n\n");
	}
	else if(mode == BIT(2)){				// mode 4
		printf("Mode 4 - Software Triggered Strobe\n\n");
	}
	else if(mode == (BIT(0) | BIT(2))){		// mode 5
		printf("Mode 5 - Software Triggered Strobe (Retriggerable)\n\n");
	}


	// BCD
	printf("BCD:\n");
	if(bcd==TIMER_BCD){
		printf("Count in BCD\n\n");
	}
	else if(bcd==TIMER_BIN){
		printf("Count in Binary\n\n");
	}


	// display was a success!
	return TIMER_SUCCESS;
}

int timer_test_time_base(unsigned long freq) {
	// Change timer 0 frequency and return SUCESS / ERROR_OCCURED
	return timer_set_frequency(0,freq);
}

int timer_test_int(unsigned long time) {
	// Subscribe timer interruptions and save the mask bit for the subscribed interrupt
	int timerMaskBit = timer_subscribe_int();

	if(timerMaskBit < 0)		// function returned negative value, error occured!
		return TIMER_ERROR_OCCURED;


	// Count the timer interruptions and print a message every second
	int r;
	int ipc_status;
	message msg;

	while(timerIntCounter < (time*60)){
		/* Get a request message. */
	    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
	       printf("driver_receive failed with: %d", r);
	       continue;
	    }
	    if (is_ipc_notify(ipc_status)) { /* received notification */
	       switch (_ENDPOINT_P(msg.m_source)) {
	           case HARDWARE: /* hardware interrupt notification */
	               if (msg.NOTIFY_ARG & BIT(timerMaskBit)) { /* subscribed interrupt */
	                   timer_int_handler();
	               }
	               break;
	           default:
	               break; /* no other notifications expected: do nothing */
	       }
	   } else { /* received a standard message, not a notification */
	       /* no standard messages expected: do nothing */
	   }
	   if(timerIntCounter%60 == 0)	// Print a numbered message every second!
		   printf("\nMensagem no. %d", timerIntCounter/60);
	}

	// Give some space from the printed messages
	printf("\n\n");


	// Unsubscribe the timer interruption and return success / failure
	return timer_unsubscribe_int();
}

int timer_test_config(unsigned char timer) {
	if (timer<0 || timer>2)	// timer out of range
		return TIMER_ERROR_OCCURED;

	unsigned char st;		// timer status

	// get timer configuration and check for errors
	if(timer_get_conf(timer, &st) != TIMER_SUCCESS)
		return TIMER_ERROR_OCCURED;

	// display the timer's configuration in a user-friendly way and return SUCCESS / ERROR_OCCURED
	if(timer_display_conf(st) != TIMER_SUCCESS)
		return TIMER_ERROR_OCCURED;


	// success!!
	return TIMER_SUCCESS;

}
