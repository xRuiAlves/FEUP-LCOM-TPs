#include "test5.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/sysutil.h>
#include "stdio.h"
#include "video_gr.h"
#include "video.h"
#include "i8042.h"
#include "i8254.h"
#include "utilities.h"
#include "kbd.h"
#include "video_test.h"
#include "vbe.h"
#include "Sprite.h"
#include "timer.h"


/* Externs from kbd.c*/
extern unsigned char codeByte;

/* Externs from timer.c */
extern unsigned int timerIntCounter;



int video_test_init(unsigned short mode, unsigned short delay) {

    // Run vg_init() with the specified mode and save the VM addresss
    void *v_mem = vg_init(mode);

    // Check for success
    if (v_mem == NULL){
    	printf("test5::video_test_init() failed after uncessful call of vg_init()\n");
    	return VG_INIT_FAILED;
    }

	// Wait 'delay' seconds
	tickdelay(micros_to_ticks(delay * MICROS_TO_SECONDS));

	// Change back to default Minix 3 text mode and check for success / failure
	if (vg_exit() != OK){
    	printf("test5::video_test_init() failed after uncessful call of vg_exit()\n");
    	return VG_EXIT_FAILED;
    }


    // Function ran successfully
    return OK;
}




int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	// Subscribe keyboard interrupts and check for success
	int kbd_mask_bit = kbd_subscribe_int();

	if (kbd_mask_bit < 0)
		return KBD_SUBS_INT_FAILED;


	// Change to video mode 0x105 and check for success
	void *v_mem = vg_init(VBE_MODE_1024x768_256C);

	if (v_mem == NULL){
		printf("test5::video_test_square() failed after uncessful call of vg_init()\n");
		return VG_INIT_FAILED;
	}


	// Draw the square
	vg_draw_square(x,y,size,color);
	// Call video_dump_fb() as requested in the handout
	video_dump_fb();


	// Loop until the ESC key is pressed (Break Code 0x81)
	int r;
	int ipc_status;
	message msg;

	while(codeByte != KBD_ESC_BREAKCODE){
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & BIT(kbd_mask_bit)) { /* Keyboard interrupt */
						if(kbd_c_int_handler() != KBD_SUCCESS)	// The interruption handler was not sucessfull to attend the interruption, due to parity error or timeout or due to sys_inb failures
							continue;
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			 /* no standard messages expected: do nothing */
		}
	}

	// Unsubscribe keyboard interrupts and check for success
	if (kbd_unsubscribe_int() != OK)
		return KBD_UNSUBS_INT_FAILED;


	// Change back to text mode and check for success
	if (vg_exit() != OK)
		return VG_EXIT_FAILED;


    // The function ran successfully
	return OK;
}




int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {

	// Subscribe keyboard interrupts and check for success
	int kbd_mask_bit = kbd_subscribe_int();

	if (kbd_mask_bit < 0)
		return KBD_SUBS_INT_FAILED;


	// Change to video mode 0x105 and check for success
	void *v_mem = vg_init(VBE_MODE_1024x768_256C);

	if (v_mem == NULL){
		printf("test5::video_test_line() failed after uncessful call of vg_init()\n");
		return VG_INIT_FAILED;
	}


	// Draw the line
	vg_draw_line(xi, yi, xf, yf, color);
	// Call video_dump_fb() as requested in the handout
	video_dump_fb();


	// Loop until the ESC key is pressed (Break Code 0x81)
	int r;
	int ipc_status;
	message msg;

	while(codeByte != KBD_ESC_BREAKCODE){
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & BIT(kbd_mask_bit)) { /* Keyboard interrupt */
						if(kbd_c_int_handler() != KBD_SUCCESS)	// The interruption handler was not sucessfull to attend the interruption, due to parity error or timeout or due to sys_inb failures
							continue;
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			 /* no standard messages expected: do nothing */
		}
	}

	// Unsubscribe keyboard interrupts and check for success
	if (kbd_unsubscribe_int() != OK)
		return KBD_UNSUBS_INT_FAILED;


	// Change back to text mode and check for success
	if (vg_exit() != OK)
		return VG_EXIT_FAILED;


	// The function ran successfully
	return OK;
}




int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

    // Subscribe keyboard interrupts and check for success
	int kbd_mask_bit = kbd_subscribe_int();

	if (kbd_mask_bit < 0)
		return KBD_SUBS_INT_FAILED;


	// Change to video mode 0x105 and check for success
	void *v_mem = vg_init(VBE_MODE_1024x768_256C);

	if (v_mem == NULL){
		printf("test5::test_xpm() failed after uncessful call of vg_init()\n");
		return VG_INIT_FAILED;
	}


    // Create the sprite with the desired coordinates and initial speed equal to zero
    Sprite* sprite = create_sprite(xpm, xi, yi, 0 ,0);

    // Check if the sprite's creating was successful
    if (sprite == NULL)
        return CREATING_SPRITE_FAILED;


    // Draw the sprite
    vg_draw_sprite(sprite);
    // Call video_dump_fb() as requested in the handout
    video_dump_fb();


    // Loop until the ESC key is pressed (Break Code 0x81)
    int r;
    int ipc_status;
    message msg;

    while(codeByte != KBD_ESC_BREAKCODE){
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.NOTIFY_ARG & BIT(kbd_mask_bit)) { /* Keyboard interrupt */
                        if(kbd_c_int_handler() != KBD_SUCCESS)	// The interruption handler was not sucessfull to attend the interruption, due to parity error or timeout or due to sys_inb failures
                            continue;
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received a standard message, not a notification */
             /* no standard messages expected: do nothing */
        }
    }


    // Unsubscribe keyboard interrupts and check for success
    if (kbd_unsubscribe_int() != OK)
        return KBD_UNSUBS_INT_FAILED;


    // Change back to text mode and check for success
    if (vg_exit() != OK)
        return VG_EXIT_FAILED;


    // The function ran successfully
	return OK;
}




int test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {
    int distance_remaining;

    // Compute the number of interrupts between each image rendering
    u_int num_ints = 60/f;


    // Check if the frame rate isn't too high
    if (f > MAX_FRAME_RATE)
        return FRAME_RATE_OUT_OF_RANGE;


    // Subscribe keyboard interrupts and check for success
	int kbd_mask_bit = kbd_subscribe_int();

	if (kbd_mask_bit < 0)
		return KBD_SUBS_INT_FAILED;


    // Subscribe timer interrupts and check for success
    int timerMaskBit = timer_subscribe_int();

    if(timerMaskBit < 0)
        return TIMER_ERROR_OCCURED;



	// Change to video mode 0x105 and check for success
	void *v_mem = vg_init(VBE_MODE_1024x768_256C);

	if (v_mem == NULL){
		printf("test5::test_move() failed after unsucessful call of vg_init()\n");
		return VG_INIT_FAILED;
	}


    // Create the sprite with the desired coordinates initial coordinates
    Sprite* sprite = create_sprite(xpm, xi, yi, 0 ,0);

    // Check if the sprite's creating was successful
    if (sprite == NULL)
        return CREATING_SPRITE_FAILED;


    // Draw the 'initial' sprite
    vg_draw_sprite(sprite);
    video_dump_fb();        // Call video_dump_fb after rendering every image, as requested in the lab's handout


    // Case in which the speed is positive : It should be understood as the displacement in pixels between consecutive frames
    if (s >= 0){
        // Set the correct speed to the sprite
        if (xi == xf){     // Case in which the X does not change
            distance_remaining = absolute_value(yf - yi);
            if (yf > yi)  // It should move down, with positive speed!
                sprite->yspeed = s;
            else          // It should move up, with negative speed!
                sprite->yspeed = -s;
        }
        else{             // Case in which the Y does not change
            distance_remaining = absolute_value(xf - xi);
            if (xf > xi)  // It should move right, with positive speed!
                sprite->xspeed = s;
            else          // It should move left, with negative speed!
                sprite->xspeed = -s;
        }

        // If speed is zero, no distance is remaining
        if (s==0)   distance_remaining = 0;

        // Loop until the ESC key is pressed (Break Code 0x81)
        int r;
        int ipc_status;
        message msg;

        while(codeByte != KBD_ESC_BREAKCODE){
            /* Get a request message. */
            if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
                printf("driver_receive failed with: %d", r);
                continue;
            }
            if (is_ipc_notify(ipc_status)) { /* received notification */
                switch (_ENDPOINT_P(msg.m_source)) {
                    case HARDWARE: /* hardware interrupt notification */
                        if (msg.NOTIFY_ARG & BIT(kbd_mask_bit)) { /* Keyboard interrupt */
                            if(kbd_c_int_handler() != KBD_SUCCESS)	// The interruption handler was not sucessfull to attend the interruption, due to parity error or timeout or due to sys_inb failures
                                continue;
                        }
                        if (msg.NOTIFY_ARG & BIT(timerMaskBit)){ /* Timer Interrupt */
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

            // Render new image
            if ((timerIntCounter%num_ints == 0) && (distance_remaining > 0)){
                //Update the sprite's position
                sprite->x += sprite->xspeed;
                sprite->y += sprite->yspeed;
                distance_remaining -= absolute_value(sprite->xspeed + sprite->yspeed);    // One of the speeds is zero!

                // Check if the sprite isn't trying to move past the final point! (This may happen due to the speed being != 1)
                if (distance_remaining < 0){
                    sprite->x = xf;
                    sprite->y = yf;
                }

                // Draw the sprite
                vg_clear_screen();
                vg_draw_sprite(sprite);
                video_dump_fb();        // Call video_dump_fb after rendering every image, as requested in the lab's handout
            }

        }

    }

    // Case in which the speed is negative : It should be understood as the number of frames required for a displacement of one pixel
    else{
        u_int num_of_passed_frames = 0;     // Number of frames that have passed
        int hasBeenDrawn;   // Auxiliary flag to make sure multiple images are not rendered in the same frame

        // Set the correct speed to the sprite
        if (xi == xf){     // Case in which the X does not change
            distance_remaining = absolute_value(yf - yi);
            if (yf > yi)  // It should move down, with positive speed!
                sprite->yspeed = 1;
            else          // It should move up, with negative speed!
                sprite->yspeed = -1;
        }
        else{             // Case in which the Y does not change
            distance_remaining = absolute_value(xf - xi);
            if (xf > xi)  // It should move right, with positive speed!
                sprite->xspeed = 1;
            else          // It should move left, with negative speed!
                sprite->xspeed = -1;
        }

        // Loop until the ESC key is pressed (Break Code 0x81)
        int r;
        int ipc_status;
        message msg;

        while(codeByte != KBD_ESC_BREAKCODE){
            /* Get a request message. */
            if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
                printf("driver_receive failed with: %d", r);
                continue;
            }
            if (is_ipc_notify(ipc_status)) { /* received notification */
                switch (_ENDPOINT_P(msg.m_source)) {
                    case HARDWARE: /* hardware interrupt notification */
                        if (msg.NOTIFY_ARG & BIT(kbd_mask_bit)) { /* Keyboard interrupt */
                            if(kbd_c_int_handler() != KBD_SUCCESS)	// The interruption handler was not sucessfull to attend the interruption, due to parity error or timeout or due to sys_inb failures
                                continue;
                        }
                        if (msg.NOTIFY_ARG & BIT(timerMaskBit)){ /* Timer Interrupt */
                            // Call the timer interruption handler, thus incrementing the timerIntCounter
                            timer_int_handler();

                            if (timerIntCounter%num_ints == 0){ // Another frame has passed
                                num_of_passed_frames++;
                                hasBeenDrawn = 0;       // Un-set the flag
                            }
                        }
                        break;
                    default:
                        break; /* no other notifications expected: do nothing */
                }
            } else { /* received a standard message, not a notification */
                 /* no standard messages expected: do nothing */
            }

            // Render new image, if the specified number of frames specified in 'speed' have passed!
            if ((num_of_passed_frames%absolute_value(s)==0) && (distance_remaining > 0) && (!hasBeenDrawn)){
                num_of_passed_frames = 0;   // Reset the number of passed frames

                //Update the sprite's position
                sprite->x += sprite->xspeed;
                sprite->y += sprite->yspeed;
                distance_remaining -= 1;    // One less step to go!

                // Draw the sprite
                vg_clear_screen();
                vg_draw_sprite(sprite);
                video_dump_fb();        // Call video_dump_fb after rendering every image, as requested in the lab's handout

                hasBeenDrawn = 1;   // Set the flag!
            }
        }

    }

    // Unsubscribe keyboard interrupts and check for success
    if (kbd_unsubscribe_int() != OK)
        return KBD_UNSUBS_INT_FAILED;


    // Unsubscribe the timer interrupts and check for success
    if(timer_unsubscribe_int() != OK)
		return TIMER_ERROR_OCCURED;


    // Change back to text mode and check for success
    if (vg_exit() != OK)
        return VG_EXIT_FAILED;


    // The function ran successfully
	return OK;
}




int test_controller() {

	// Get the VBE controller information
    vbe_ctrl_info_t vctrl_info;
    if (vbe_get_ctrl_info(&vctrl_info) != OK){    // Check if no error occured
        printf("test5::test_controller -> failed to get controller information! \n");
        return GET_CTRL_INFO_FAILED;
    }


	// Print the VBE controller information
	vbe_print_ctrl_info(&vctrl_info);


	// Function was successfully
	return OK;
}
