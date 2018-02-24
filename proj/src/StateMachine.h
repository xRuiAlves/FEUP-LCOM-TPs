#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

/** @defgroup StateMachine StateMachine
 * @{
 * Functions for manipulating the game's State Machine
 */

#define BUFFER_MAX_SIZE         50
#define SM_ALLOC_FAILED         NULL


struct StateMachine_t;
typedef struct StateMachine_t StateMachine;


/* Possible States for the State Machine */
typedef enum { MAIN_MENU = 0 , CREDITS , PLAYER_1_PLAYING , PLAYER_1_LEFT , WAITING_PLAYER_1 , PLAYER_2_PLAYING , LOSING_SCREEN } State;

/* Possible Events for the State Machine */
typedef enum { W_KEY_PRESSED = 0 ,
               A_KEY_PRESSED ,
               S_KEY_PRESSED ,
               D_KEY_PRESSED ,
               ESC_KEY_PRESSED ,
               TIMER_TICKED ,
               SNAKE_MOVED ,
               SNAKE_SELF_COLLIDED ,
               SNAKE_APPLE_COLLIDED ,
               MOUSE_MOVED ,
               MOUSE_LB_PRESSED ,
               MOUSE_LB_RELEASED ,
               UP_BUTTON_CLICKED ,
               DOWN_BUTTON_CLICKED ,
               LEFT_BUTTON_CLICKED ,
               RIGHT_BUTTON_CLICKED ,
               RTC_ALARM_RINGED ,
               PLAYER_1_JOINED_GAME ,
               PLAYER_2_JOINED_GAME ,
               PLAYER_1_LEFT_GAME } Event;


/**
 * @brief Allocates memory for the StateMachine "object" and initializes the machine with its initial state(MAIN_MENU) and with its bufferSize(current_buffer_size Attribute= 0)
 *
 * @return StateMachine* - the addressof the StateMachine that was just created
 *
 */
StateMachine* initialize_state_machine();

/**
 * @brief  Deletes the StateMachine "object" passed by parameter, thus freeing the memory occupied by it
 *
 * @param sm - a StateMachine pointer
 *
 * @return void
 *
 */
void terminate_state_machine(StateMachine* sm);

/**
 * @brief Adds the Event evt to the StateMachine's events_buffer
 *
 * @param sm - a StateMachine pointer
 * @param evt - an Event
 *
 * @return void
 *
 */
void raise_event(StateMachine* sm , Event evt);

/**
 * @brief Goes through all the Events in the events_buffer and calls the appropriate event handler for each, thus emptying the buffer
 *
 * @param sm - a StateMachine pointer
 *
 * @return void
 *
 */
void handle_events(StateMachine* sm);

/** @} end of StateMachine */

#endif /* _STATE_MACHINE_H_ */
