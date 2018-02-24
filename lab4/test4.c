#include "test4.h"
#include "i8042.h"
#include "i8254.h"
#include "ps2.h"
#include "kbd.h"
#include "timer.h"
#include "mouseSM.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "stdio.h"

/* Extern Variables from timer.c */
extern unsigned int timerIntCounter;    // For counting time in mouse_test_async()

/* Extern Variables from ps2.c */
extern unsigned char packet[3];
extern unsigned char packetCount;

/* Extern State Machine com mouseSM.c */
extern struct StateMachine state_machine;
extern int mouseEventOccured;          // Flag to check if a mouse event has occured (1 if event occured, 0 if not)


int mouse_test_packet(unsigned short cnt){
    unsigned short printCounter = 0;    // Counts the number of times a packet is printed
    unsigned char acknowledgementByte;


    // Subscribe the Mouse interrupts and get the mouse's bit order in the interruption mask OR get -1 in case of failure
    int ps2MaskBit = ps2_subscribe_int();

	if (ps2MaskBit == PS2_INT_SUBS_ERROR)		// ps2_subscribe_int() failed
		return PS2_INT_SUBS_ERROR;


    // Enable data reporting in stream mode
    do{
        acknowledgementByte = ps2_enable_dataReporting();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to enable data reporting until the command is acknowleged by the mouse


    // Read the mouse packets and print the desired number of packets
	int r;
	int ipc_status;
	message msg;

    while(printCounter < cnt){
        /* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & BIT(ps2MaskBit)) { /* Mouse interrupts */
                        if(ps2_int_handler() != PS2_SUCCESS)    // Call the interrupt handler and check if it ran successfully
                            continue;

                        // The interrupt handler was successfull, the packet byte that was read is now in ps2::packetByte ; Parse it
                        if (ps2_parse_packetByte() == PS2_COMPLETE_PACKET){
                            ps2_print_packet(); // The packet is complete ; print it
                            printCounter ++;
                        }
                        else {  // The packet is not complete yet / packet reading was not synchronized ; Continue the packet reading
                            continue;
                        }

					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			 /* no standard messages expected: do nothing */
		}
    }


    // Disable data reporting in stream mode
    do{
        acknowledgementByte = ps2_disable_dataReporting();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to disable data reporting until the command is acknowleged by the mouse


    // Unsubscribe the mouse interrupts and return success / failure
    if (ps2_unsubscribe_int() != PS2_SUCCESS)
        return PS2_INT_UNSUBS_ERROR;

    printf("\n\n  test4::mouse_test_packet() sucessfully terminated\n\n");

    return PS2_SUCCESS;
}




int mouse_test_async(unsigned short idle_time){

    unsigned char acknowledgementByte;


    // Subscribe the Mouse interrupts and get the mouse's bit order in the interruption mask OR get -1 in case of failure
    int ps2MaskBit = ps2_subscribe_int();

	if (ps2MaskBit == PS2_INT_SUBS_ERROR)		// ps2_subscribe_int() failed
		return PS2_INT_SUBS_ERROR;


    // Subscribe the Timer interrupts and get the Timer's bit order in the interruption mask OR get -1 in case of failure
	int timerMaskBit = timer_subscribe_int();

    if(timerMaskBit == -1)		// timer_subscribe_int() failed
    	return TIMER_ERROR_OCCURED;


    // Enable data reporting in stream mode
    do{
        acknowledgementByte = ps2_enable_dataReporting();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to enable data reporting until the command is acknowleged by the mouse


    // Reads the mouse packets and print packets before idle time is reached
	int r;
	int ipc_status;
	message msg;

    while(timerIntCounter < (idle_time * 60)) {     // Loops for a number of seconds equal to idle_time
        /* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
                    if (msg.NOTIFY_ARG & BIT(timerMaskBit)){ /* Timer Interrupt */
                        // Call the timer interruption handler, thus incrementing the timerIntCounter
                        timer_int_handler();
                    }
					if (msg.NOTIFY_ARG & BIT(ps2MaskBit)) { /* Mouse Interrupt */
                        if(ps2_int_handler() != PS2_SUCCESS)   // Call the interrupt handler and check if it ran successfully
                            continue;

                        // The interrupt handler was successfull, the packet byte that was read is now in ps2::packetByte ; Parse it
                        if (ps2_parse_packetByte() == PS2_COMPLETE_PACKET){
                            ps2_print_packet(); // The packet is complete ; print it
                            timerIntCounter = 0;    // Reset the idle time counter!
                        }

                        else {  // The packet is not complete yet / packet reading was not synchronized ; Continue the packet reading
                            /* Do nothing */
                        }
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			 /* no standard messages expected: do nothing */
		}
    }


    // Disable data reporting in stream mode
    do{
        acknowledgementByte = ps2_disable_dataReporting();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;

    } while (acknowledgementByte != PS2_ACK);   // Try to disable data repoting until the command is acknowleged by the mouse


    // Unsubscribe the mouse interrupts and return success / failure
    if (ps2_unsubscribe_int() != PS2_SUCCESS)
        return PS2_INT_UNSUBS_ERROR;


    // Unsubscribe the Timer interrupts and deal with possible failure
	if(timer_unsubscribe_int() != TIMER_SUCCESS)
		return TIMER_ERROR_OCCURED;

    printf("\n\n  test4::mouse_test_async() sucessfully terminated\n\n");

    return PS2_SUCCESS;
}





int mouse_test_remote(unsigned long period, unsigned short cnt){
    unsigned char acknowledgementByte;   // To read acknoledgement bytes returned from mouse commands

    // Read the initial command byte from the kbc, and save it to write it back in the end
    unsigned char initialCommandByte;

    // Enable kbc insterrupts in exclusive mode to prevent minix's keyboard IH from stealing the command byte from the output buffer
    if(kbd_subscribe_int() == -1)
        return KBD_ERROR_OCCURED;

    // Read the command byte itself
    if (kbd_read_kbc_commandByte(&initialCommandByte) != KBD_SUCCESS)
        return KBD_ERROR_OCCURED;

    // Get the "new" command byte by un-setting bit 1, thus disabling minix's interrupt handler for mouse
    unsigned char newCommandByte = initialCommandByte & 0xFD;   // Logic AND operator with mask 1111 1101 , which keeps all bits equal, less bit 1 (minix mouse IH), which becomes 0

    // Write the new command byte (disabling)
    if(kbd_write_cmd_witharg(KBC_WRITE_COMMAND, newCommandByte) != KBD_SUCCESS)
        return KBD_ERROR_OCCURED;

    // Unsubscribe the interrupt that were previously subscribed to prevet minix keyboard IH from stealing content written / to be read
    if(kbd_unsubscribe_int()!= KBD_SUCCESS)
        return KBD_ERROR_OCCURED;


    /*  ------------------------------------------------------------------------------------------------------------------------------------------
        In order to both displacement and mouse buttons state being sent by the mouse in remote mode, we figured out that had to run test_packet()
        or test_async() before running test_remote(). After some more digging, we figured that in order for the mouse to report displacement and
        buttons we had to ENABLE DATA REPORTING and DISABLE DATA REPORTING at least once before going into remote mode. This solved the problem,
        allthough we don't understand why. The following two operations Enable and Disable data reporting because of this exact reason. */

    // Enable Data Reporting
    do{
        acknowledgementByte = ps2_enable_dataReporting();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to enable data repoting until the command is acknowleged by the mouse

    // Disable Data Reporting
    do{
        acknowledgementByte = ps2_disable_dataReporting();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to disable data repoting until the command is acknowleged by the mouse

    /* Now that data reporting has been enabled -> disabled at least once, proceed to remote mode and print the disered number of packets, with
       the specified interval between packets
        ------------------------------------------------------------------------------------------------------------------------------------------- */


    // Set the mouse to remote mode
    do{
        acknowledgementByte = ps2_set_remoteMode();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to set remote mode until the command is acknowleged by the mouse


    // Print the desired number of packets with the specified interval between packets
    unsigned short printedPacketCounter = 0;

    while (printedPacketCounter < cnt){
        // Request a packet from the mouse
        do{
            acknowledgementByte = ps2_request_packet();
            if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
                return acknowledgementByte;
        } while (acknowledgementByte != PS2_ACK);   // Try to request a packet until the command is acknowleged by the mouse

        // Read the 3 packet bytes and print the packet!
        do{
            if(ps2_remote_read_packetByte(&packet[packetCount]) != PS2_SUCCESS){
                return PS2_READ_OUTBUFFER_ERROR;
            }
            else
                packetCount++;
        } while (packetCount < 3);

        packetCount = 0; // Reset the packet count

        ps2_print_packet();  // Print the packet

        printedPacketCounter++;

        // Wait 'period' miliseconds (specified in parameter) until the next packet printing
        tickdelay(micros_to_ticks(period*1000));
    }


    // Set the mouse back to stream mode
    do{
        acknowledgementByte = ps2_set_streamMode();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to set stream mode until the command is acknowleged by the mouse


    // Make sure that data reporting in disabled!
    do{
        acknowledgementByte = ps2_disable_dataReporting();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to disable data repoting until the command is acknowleged by the mouse


    // Re-write the initial kbc's command byte, thus re-enabling minix's mouse and keyboard interruption handlers
    if(kbd_write_cmd_witharg(KBC_WRITE_COMMAND, initialCommandByte) != KBD_SUCCESS)
        return KBD_ERROR_OCCURED;

    printf("\n\n  test4::mouse_test_remote() sucessfully terminated\n\n");

    return PS2_SUCCESS;
}




int mouse_test_gesture(short length){

    // Check if the parameter is valid -> Length can not be negative nor null.
    if (length <= 0)
        return INVALID_PARAMETER;

    state_machine.state = INIT;         // Initialize the State Machine. The initial State is INIT. It has no initial Event, because no event has happened yet
    unsigned char acknowledgementByte;  // To process the acknowledgement bytes


    // Subscribe the Mouse interrupts and get the mouse's bit order in the interruption mask OR get -1 in case of failure
    int ps2MaskBit = ps2_subscribe_int();

	if (ps2MaskBit == PS2_INT_SUBS_ERROR)		// ps2_subscribe_int() failed
		return PS2_INT_SUBS_ERROR;


    // Enable data reporting in stream mode
    do{
        acknowledgementByte = ps2_enable_dataReporting();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to enable data reporting until the command is acknowleged by the mouse



    /* Loops until the final state is reached, that is, until the line drawn */
	int r;
	int ipc_status;
	message msg;

    while(state_machine.state != FINAL){
        /* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & BIT(ps2MaskBit)) { /* Mouse interrupts */
                        if(ps2_int_handler() != PS2_SUCCESS)    // Call the interrupt handler and check if it ran successfully
                            continue;

                        // The interrupt handler was successfull, the packet byte that was read is now in ps2::packetByte ; Parse it
                        if (ps2_parse_packetByte() == PS2_COMPLETE_PACKET){
                            ps2_print_packet();             // Print the packet
                            mouse_event_trigger(length);    // Trigger any events that occured
                        }
                        else {  // The packet is not complete yet / packet reading was not synchronized ; Continue the packet reading
                            continue;
                        }
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			 /* no standard messages expected: do nothing */
		}


        if (mouseEventOccured){     /* If some mouse event occured, handle it! */
            handle_mouse_event();
        }
    }


    // Disable data reporting in stream mode
    do{
        acknowledgementByte = ps2_disable_dataReporting();
        if ((acknowledgementByte!= PS2_ACK) && (acknowledgementByte != PS2_NACK))  // Error due to sys_inb failure or due to second consecutive invalid byte
            return acknowledgementByte;
    } while (acknowledgementByte != PS2_ACK);   // Try to disable data reporting until the command is acknowleged by the mouse


    // Unsubscribe the mouse interrupts and return success / failure
    if (ps2_unsubscribe_int() != PS2_SUCCESS)
        return PS2_INT_UNSUBS_ERROR;

    printf("\n\n  test4::mouse_test_gesture() sucessfully terminated\n\n");

    return PS2_SUCCESS;
}
