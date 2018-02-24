#include "timer.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/com.h>
#include "stdio.h"
#include "i8254.h"
#include "Game.h"
#include "SnakeyNix.h"

static int timerHookId = 0;
unsigned int timerIntCounter = 0;


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

void handle_timer(){
	timer_int_handler();

	// Timer has ticked!
	raise_event(get_game_state_machine(getSnakeyNix()) , TIMER_TICKED);


	if (timer_get_int_counter() % GAME_UPDATE_RATE == 0){   // Check if is time to update the game's status
        move_snake(get_game_snake(getSnakeyNix()));

		// Snake moved
		raise_event(get_game_state_machine(getSnakeyNix()) , SNAKE_MOVED);

        // Check Snake self collision
        if (check_snake_self_collision( get_game_snake(getSnakeyNix()) ))
			raise_event(get_game_state_machine(getSnakeyNix()) , SNAKE_SELF_COLLIDED);

        // Check Snake-Apple collision
        if (check_snake_head_apple_colision( get_game_snake(getSnakeyNix()) , get_game_apple(getSnakeyNix()) ))
			raise_event(get_game_state_machine(getSnakeyNix()) , SNAKE_APPLE_COLLIDED);
	}
}
