#include <minix/syslib.h>
#include <minix/drivers.h>
#include "stdio.h"
#include "test3.h"
#include "kbd.h"
#include "timer.h"
#include "i8042.h"
#include "i8254.h"

/* Externs from kbd.c*/
extern unsigned char codeByte;
extern unsigned int sysInbCounter;

/* Externs from timer.c*/
extern unsigned int timerIntCounter;

/* Externs from assemblyKbd.S */
extern int kbd_a_int_handler();

int kbd_test_scan(unsigned short ass) {

	// Subscribe the Keyboard interrupts and get the keyboard's bit order in the interruption mask OR get -1 in case of failure
	int kbdMaskBit = kbd_subscribe_int();

	if (kbdMaskBit == -1)		// kbd_subscribe_int() failed
		return KBD_ERROR_OCCURED;


	// Read the keyboard for break and makecodes, until escape key is released (Breakcode 0x81)
	int r;
	int ipc_status;
	message msg;
	int extraByteExistance = 0;		// 1 if the previously read byte was 0xE0/0xE1 (it's a 2 Byte Code) and 0 otherwise
	unsigned char extraByte;		// Extra byte in case a 2-byte code is read

	while(codeByte != KBD_ESC_BREAKCODE){
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & BIT(kbdMaskBit)) { /* subscribed interrupt */
						if(ass == 0){								// Ass = 0, call the C interrupt handler
							if(kbd_c_int_handler() != KBD_SUCCESS)	// The interruption handler was not sucessfull to attend the interruption, due to parity error or timeout or due to sys_inb failures
								continue;
						}
						else {										// Ass != 0, call the Assembly interrupt handler
							if(kbd_a_int_handler() != KBD_SUCCESS)	// The interruption handler was not sucessfull to attend the interruption, due to parity error or timeout error
								continue;
						}

						// If the kbd_c_int_handler() was sucessfull, the make/break code is now in codeByte global variable
						if((codeByte == 0xE0) || (codeByte == 0xE1)){	// It's a 2 Byte code! Light up the flag and continue to the next loop
							extraByte = codeByte;	// Save the extra byte
							extraByteExistance = 1;
							continue;
						}

						// Check if it is a break or a make code
						if ((extraByteExistance == 0) && !(codeByte & BIT(7)))		// 1 Byte Makecode
							kbd_print_code(0,codeByte,0);					// The third parameter does not matter, since it's a one byte code
						else if ((extraByteExistance == 0) && (codeByte & BIT(7)))	// 1 Byte Breakcode
							kbd_print_code(1,codeByte,0);					// The third parameter does not matter, since it's a one byte code
						else if ((extraByteExistance == 1) && !(codeByte & BIT(7)))	// 2 Byte Makecode
							kbd_print_code(2,codeByte,extraByte);
						else if ((extraByteExistance == 1) && (codeByte & BIT(7)))	// 2 Byte Breakcode
							kbd_print_code(3,codeByte,extraByte);

						// The extraByteExistance flag is now not set
						extraByteExistance = 0;
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			 /* no standard messages expected: do nothing */
		}
	}

	#ifdef LAB3
	// Print the number of sys_inb calls
	if (ass == 0)	// Only prints the number of sys_inb() calls when we are using the C interrupt handler
		printf("\n\nNumber of sys_inb calls: %d\n\n", sysInbCounter);
	#endif

	// Unsubscribe the Keyboard interrupts and check if it was successfull
	return kbd_unsubscribe_int();
}





int kbd_test_poll() {
	unsigned char stat;		// Variable for reading the kbc status Register
	unsigned char data;		// Variable for reading the output buffer
	int extraByteExistance = 0;		// 1 if the previously read byte was 0xE0/0xE1 (it's a 2 Byte Code) and 0 otherwise
	unsigned char extraByte;		// Extra byte in case a 2-byte code is read


	// Loop until escape key is released
	while(codeByte != KBD_ESC_BREAKCODE) {
		// Read the status register
		if (kbd_read_statbyte(&stat) != KBD_SUCCESS)				// Verify sys_inb success/failure
			return KBD_ERROR_OCCURED;

		// Check if Output buffer is full
		if( stat & KBC_STATUS_OBF ) {
			// Read the output buffer content
			if ( kbd_read_outputbuffer(&data) != KBD_SUCCESS)		// Verify sys_inb success/failure
				return KBD_ERROR_OCCURED;
			// Check if there wasn't parity error / timeout error
			if ( (stat & (KBC_STATUS_PARITY | KBC_STATUS_TIMEOUT)) == 0 ){
				codeByte = data;    // Get the Makecode / BreakCode

				if((codeByte == 0xE0) || (codeByte == 0xE1)){	// It's a 2 Byte code! Light up the flag and continue to the next loop
					extraByte = codeByte;	// Save the extra byte
					extraByteExistance = 1;
					continue;
				}

				// Check if it is a break or a make code
				if ((extraByteExistance == 0) && !(codeByte & BIT(7)))		// 1 Byte Makecode
					kbd_print_code(0,codeByte,0);					// The third parameter does not matter, since it's a one byte code
				else if ((extraByteExistance == 0) && (codeByte & BIT(7)))	// 1 Byte Breakcode
					kbd_print_code(1,codeByte,0);					// The third parameter does not matter, since it's a one byte code
				else if ((extraByteExistance == 1) && !(codeByte & BIT(7)))	// 2 Byte Makecode
					kbd_print_code(2,codeByte,extraByte);
				else if ((extraByteExistance == 1) && (codeByte & BIT(7)))	// 2 Byte Breakcode
					kbd_print_code(3,codeByte,extraByte);

				// The extraByteExistance flag is now not set
				extraByteExistance = 0;
			}
			else		// There was parity error / timeout error
				return KBD_ERROR_OCCURED;
			}
			tickdelay(micros_to_ticks(KBD_DELAY));		// wait 20 ms
	}

	#ifdef LAB3
	// Print the number of sys_inb calls
	printf("\n\nNumber of sys_inb calls: %d\n\n", sysInbCounter);
	#endif

	// Write the command to re-enable the keyboard in Minix, and return if it was sucessfull or not
	return kbd_write_cmd_witharg(KBC_WRITE_COMMAND, KBC_REENABLE_KEYBOARD);
}





int kbd_test_timed_scan(unsigned short n) {

	// Subscribe both interrupts (Keyboard and Timer)

	// Subscribe the Keyboard interrupts and get the keyboard's bit order in the interruption mask OR get -1 in case of failure
	int kbdMaskBit = kbd_subscribe_int();

	if (kbdMaskBit == -1)		// kbd_subscribe_int() failed
		return KBD_ERROR_OCCURED;

	// Subscribe the Timer interrupts and get the Timer's bit order in the interruption mask OR get -1 in case of failure
	int timerMaskBit = timer_subscribe_int();

	if(timerMaskBit == -1)		// timer_subscribe_int() failed
		return TIMER_ERROR_OCCURED;


	/* Read the keyboard for break and makecodes, until escape key is released (Breakcode 0x81) OR until
	   no keys are pressed for n seconds (n*60 timer interrupts, if timer freq is 60) */
	int r;
	int ipc_status;
	message msg;
	int extraByteExistance = 0;		// 1 if the previously read byte was 0xE0/0xE1 (it's a 2 Byte Code) and 0 otherwise
	unsigned char extraByte;		// Extra byte in case a 2-byte code is read

	while( (codeByte != KBD_ESC_BREAKCODE) && (timerIntCounter < (60*n)) ){
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & BIT(kbdMaskBit)) {  /* Keyboard Interrupt */
						if(kbd_c_int_handler() != KBD_SUCCESS)	// The interruption handler was not sucessfull to attend the interruption, due to parity error or timeout or due to sys_inb failures
							continue;

						// There was a keyboard interrupt ( a key was pressed ) , reset the timerIntCounter
						timerIntCounter = 0;

						// If the kbd_c_int_handler() was sucessfull, the make/break code is now in codeByte global variable
						if((codeByte == 0xE0) || (codeByte == 0xE1)){	// It's a 2 Byte code! Light up the flag and continue to the next loop
							extraByte = codeByte;	// Save the extra byte
							extraByteExistance = 1;
							continue;
						}

						// Check if it is a break or a make code
						if ((extraByteExistance == 0) && !(codeByte & BIT(7)))		// 1 Byte Makecode
							kbd_print_code(0,codeByte,0);					// The third parameter does not matter, since it's a one byte code
						else if ((extraByteExistance == 0) && (codeByte & BIT(7)))	// 1 Byte Breakcode
							kbd_print_code(1,codeByte,0);					// The third parameter does not matter, since it's a one byte code
						else if ((extraByteExistance == 1) && !(codeByte & BIT(7)))	// 2 Byte Makecode
							kbd_print_code(2,codeByte,extraByte);
						else if ((extraByteExistance == 1) && (codeByte & BIT(7)))	// 2 Byte Breakcode
							kbd_print_code(3,codeByte,extraByte);

						// The extraByteExistance flag is now not set
						extraByteExistance = 0;
					}
					else if (msg.NOTIFY_ARG & BIT(timerMaskBit)){ /* Timer Interrupt */
						// Call the timer interruption handler, thus incrementing the timerIntCounter
						timer_int_handler();
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			 /* no standard messages expected: do nothing */
		}
	}


	// Unsubscribe the previously subscribed interrupts (Timer and Keyboard)

	// Unsubscribe the Timer interrupts and deal with possible failure
	if(timer_unsubscribe_int() != TIMER_SUCCESS)
		return TIMER_ERROR_OCCURED;

	// Unsubscribe the Keyoard interrupts and return success/failure
	return kbd_unsubscribe_int();
}
