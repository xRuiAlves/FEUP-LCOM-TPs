#include "StateMachine.h"
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "Snake.h"
#include "Apple.h"
#include "Mouse.h"
#include "ps2.h"
#include "rtc.h"
#include "portaserie.h"
#include "spProtocol.h"
#include "stdlib.h"
#include "Game.h"
#include "SnakeyNix.h"
#include "video_gr.h"

extern unsigned int endTheGame;

static void w_key_pressed_event_handler();
static void a_key_pressed_event_handler();
static void s_key_pressed_event_handler();
static void d_key_pressed_event_handler();
static void esc_key_pressed_event_handler();
static void timer_tick_event_handler();
static void snake_move_event_handler();
static void snake_self_collision_event_handler();
static void snake_apple_collision_event_handler();
static void mouse_movement_event_handler();
static void mouse_lb_pressed_event_handler();
static void mouse_lb_released_event_handler();
static void up_button_clicked_event_handler();
static void down_button_clicked_event_handler();
static void left_button_clicked_event_handler();
static void right_button_clicked_event_handler();
static void rtc_alarm_ring_event_handler();
static void player_1_joined_game_event_handler();
static void player_2_joined_game_event_handler();
static void player_1_left_game_event_handler();

static void (*event_handlers[])() = {w_key_pressed_event_handler,
                                     a_key_pressed_event_handler,
                                     s_key_pressed_event_handler,
                                     d_key_pressed_event_handler,
                                     esc_key_pressed_event_handler,
                                     timer_tick_event_handler,
                                     snake_move_event_handler,
                                     snake_self_collision_event_handler,
                                     snake_apple_collision_event_handler,
                                     mouse_movement_event_handler,
                                     mouse_lb_pressed_event_handler,
                                     mouse_lb_released_event_handler,
                                     up_button_clicked_event_handler,
                                     down_button_clicked_event_handler,
                                     left_button_clicked_event_handler,
                                     right_button_clicked_event_handler,
                                     rtc_alarm_ring_event_handler,
                                     player_1_joined_game_event_handler,
                                     player_2_joined_game_event_handler,
                                     player_1_left_game_event_handler };


int snake_can_move = 1;   /* Auxiliary flag to check if the snake can move ; This flag is usefull to avoid the player changing direction
                             twice in the same frame. For example: If the player was moving RIGHT , he can't now change to DOWN/UP and
                             then change to LEFT , which would be illegal! */

///////////////////////////////////////
///////////////////////////////////////
/*    StateMachine Implementation    */
///////////////////////////////////////
///////////////////////////////////////


///////////////////////////////////
//    Attributes  ::  Private    //
///////////////////////////////////

struct StateMachine_t{
    // Events Buffer
    Event events_buffer[BUFFER_MAX_SIZE];
    unsigned char current_buffer_size;

    // State
    State current_state;
};


///////////////////////////////
//    Methods  ::  Public    //
///////////////////////////////


StateMachine* initialize_state_machine(){

    // Allocate space for the "Object"
    StateMachine *sm = (StateMachine *) malloc ( sizeof(StateMachine) );

    // Check if the mem. allocation was successful
    if( sm == NULL )
        return SM_ALLOC_FAILED;

    // Initialize the various state machine's Attributes
    sm->current_buffer_size = 0;
    sm->current_state = MAIN_MENU;

    // Return the newly created state machine's address
    return sm;

}

void terminate_state_machine(StateMachine* sm){

    // Check if the state machine is existant
    if ( sm == NULL )
        return;

    // Free the memory allocated for the state machine
    free(sm);

    // The state machine is now un-existant
    sm = NULL;

}

void raise_event(StateMachine* sm , Event evt){
    // Add the event to the event buffer
    sm->events_buffer[sm->current_buffer_size] = evt;

    // Increment the current buffer size
    sm->current_buffer_size ++;
}

void handle_events(StateMachine* sm){
    // If the are no events to process , return , no further verification are needed
    if (sm->current_buffer_size == 0)
        return;

    unsigned char i;

    // Handle all the events in the event buffer, calling the appropriate handler
    for(i = 0 ; i < sm->current_buffer_size ; i++){
        (*event_handlers[sm->events_buffer[i]])();
    }

    // Empty the events buffer
    sm->current_buffer_size = 0;
}

static void w_key_pressed_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // Make the snake start to move UP, if it can move!
        if (snake_can_move){
            direction dir = get_snake_direction(get_game_snake(getSnakeyNix()));

            if ((dir != UP) && (dir != DOWN)){
                set_snake_direction(get_game_snake(getSnakeyNix()) , UP);
            }
            snake_can_move = 0;    // Snake is now unable to move (until the next frame!)
        }
    }
}

static void a_key_pressed_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // Make the snake start to move LEFT, if it can move!
        if (snake_can_move){
            direction dir = get_snake_direction(get_game_snake(getSnakeyNix()));

            if ((dir != LEFT) && (dir != RIGHT)){
                set_snake_direction(get_game_snake(getSnakeyNix()) , LEFT);
            }
            snake_can_move = 0;    // Snake is now unable to move (until the next frame!)
        }
    }
}

static void s_key_pressed_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // Make the snake start to move DOWN, if it can move!
        if (snake_can_move){
            direction dir = get_snake_direction(get_game_snake(getSnakeyNix()));

            if ((dir != UP) && (dir != DOWN)){
                set_snake_direction(get_game_snake(getSnakeyNix()) , DOWN);
            }
            snake_can_move = 0;    // Snake is now unable to move (until the next frame!)
        }
    }
}

static void d_key_pressed_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // Make the snake start to move RIGHT, if it can move!
        if (snake_can_move){
            direction dir = get_snake_direction(get_game_snake(getSnakeyNix()));

            if ((dir != LEFT) && (dir != RIGHT)){
                set_snake_direction(get_game_snake(getSnakeyNix()) , RIGHT);
            }
            snake_can_move = 0;    // Snake is now unable to move (until the next frame!)
        }
    }
}

static void esc_key_pressed_event_handler(){
    // User pressed ESC. End the game.
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // The user wishes to leave the game ; user lost
        get_game_state_machine(getSnakeyNix())->current_state = LOSING_SCREEN;

        // Set an alarm to ring in 10 seconds
        rtc_set_alarm(10);

        // Notify Player 2
        uart_write_message(PLAYER_1_LEFT);
        uart_get_acknowledgement_byte();
    }
    else if (current_state == CREDITS){
        // Go back to the main menu
        get_game_state_machine(getSnakeyNix())->current_state = MAIN_MENU;
    }
    else{
        endTheGame = 1;
    }
}

static void timer_tick_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // Draw the game (player 1 prespective) and flip the buffers!
        draw_player1_game(getSnakeyNix());
        flipBuffer();
    }
    else if (current_state == MAIN_MENU){
        // Draw the menu and flip the buffers!
        draw_game_menu(getSnakeyNix());
        flipBuffer();
    }
    else if (current_state == PLAYER_2_PLAYING){
        // Draw the game (player 2 prespective) and flip the buffers!
        draw_player2_game(getSnakeyNix());
        flipBuffer();
    }
    else if (current_state == LOSING_SCREEN){
        // Draw the losing and flip the buffers!
        draw_game_losing_screen(getSnakeyNix());
        flipBuffer();
    }
    else if (current_state == WAITING_PLAYER_1){
        // Draw the losing and flip the buffers!
        draw_game_p1_waiting_screen(getSnakeyNix());
        flipBuffer();
    }
    else if (current_state == PLAYER_1_LEFT){
        // Draw the losing and flip the buffers!
        draw_game_p1_left_screen(getSnakeyNix());
        flipBuffer();
    }
    else if (current_state == CREDITS){
        // Draw the losing and flip the buffers!
        draw_game_credits_screen(getSnakeyNix());
        flipBuffer();
    }
}

static void snake_move_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // The snake is now free to move again
        snake_can_move = 1;
    }
}

static void snake_self_collision_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // Snake's head collided with its own body! The user has lost the game
        get_game_state_machine(getSnakeyNix())->current_state = LOSING_SCREEN;

        // Set an alarm to ring in 10 seconds
        rtc_set_alarm(10);

        // Notify Player 2
        uart_write_message(PLAYER_1_LEFT);
        uart_get_acknowledgement_byte();
    }
}

static void snake_apple_collision_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // Collision occured, apple is not active anymore!
        set_apple_status( get_game_apple(getSnakeyNix()) , APPLE_NOT_SET);

        // Grow the snake and increment the score!
        grow_snake( get_game_snake(getSnakeyNix()) );
        increase_game_score(getSnakeyNix());
    }
}

static void mouse_movement_event_handler(){
    u_char m_packet_byte1 = get_mouse_packet_byte1();
    u_char m_packet_byte2 = get_mouse_packet_byte2();
    u_char m_packet_byte3 = get_mouse_packet_byte3();

    // Check for overflow
    if ((m_packet_byte1 & PS2_PACKET_XOVF) || (m_packet_byte1 & PS2_PACKET_YOVF))
        return;

    // Get the movement's sign
    u_char x_sign = (m_packet_byte1 & PS2_PACKET_XSIGN) == PS2_PACKET_XSIGN;
    u_char y_sign = (m_packet_byte1 & PS2_PACKET_YSIGN) == PS2_PACKET_YSIGN;

    // Compute the the "quantaty" of movement
    short delta_x = convertBinaryToDecimal(m_packet_byte2 , x_sign);
    short delta_y = convertBinaryToDecimal(m_packet_byte3 , y_sign);

    // Move the mouse itself
    set_mouse_x_pos( get_game_mouse(getSnakeyNix()) , get_mouse_x_pos(get_game_mouse(getSnakeyNix())) + delta_x );
    set_mouse_y_pos( get_game_mouse(getSnakeyNix()) , get_mouse_y_pos(get_game_mouse(getSnakeyNix())) - delta_y );

    // Make sure the mouse stays in borders
    assert_mouse_pos( get_game_mouse(getSnakeyNix()) );



    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == MAIN_MENU){
        // Check if the player is hovering the "Player 1" button
        if (check_button_mouse_collision( get_game_player1_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_player1_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_player1_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);

        // Check if the player is hovering the "Player 2" button
        if (check_button_mouse_collision( get_game_player2_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_player2_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_player2_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);

        // Check if the player is hovering the "Exit" button
        if (check_button_mouse_collision( get_game_exit_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_exit_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_exit_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);

        // Check if the player is hovering the "Credits" button
        if (check_button_mouse_collision( get_game_credits_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_credits_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_credits_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);
    }

    else if (current_state == PLAYER_2_PLAYING){
        // Check if the player is hovering the "Up" button
        if (check_button_mouse_collision( get_game_up_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_up_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_up_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);

        // Check if the player is hovering the "Down" button
        if (check_button_mouse_collision( get_game_down_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_down_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_down_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);

        // Check if the player is hovering the "Left" button
        if (check_button_mouse_collision( get_game_left_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_left_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_left_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);

        // Check if the player is hovering the "Right" button
        if (check_button_mouse_collision( get_game_right_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_right_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_right_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);

        // Check if the player is hovering the "Exit" button
        if (check_button_mouse_collision( get_game_small_exit_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_small_exit_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_small_exit_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);
    }

    else if (current_state == LOSING_SCREEN){
        // Check if the player is hovering the "Exit" button
        if (check_button_mouse_collision( get_game_small_exit_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            set_button_status( get_game_small_exit_button(getSnakeyNix()) , BUTTON_HOVERED_STATUS);
        else
            set_button_status( get_game_small_exit_button(getSnakeyNix()) , BUTTON_DEFAULT_STATUS);
    }
}

static void mouse_lb_pressed_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (get_mouse_lb_status(get_game_mouse(getSnakeyNix())) == MOUSE_LB_IS_PRESSED) // Mouse left button was already clicked.
        return;
    else    // Mouse left button has been clicked!
        set_mouse_lb_status(get_game_mouse(getSnakeyNix()) , MOUSE_LB_IS_PRESSED);

    if (current_state == PLAYER_1_PLAYING){
        // If the player isn't clicking inside the snake's body AND if the apple is not set already, spawn the apple where the player clicked!
        if ( (!check_snake_mouse_click_collision(get_game_snake(getSnakeyNix()),get_game_mouse(getSnakeyNix()))) && (!get_apple_status(get_game_apple(getSnakeyNix()))) ) {
            short mouse_x_pos = get_mouse_x_pos( get_game_mouse(getSnakeyNix()) );
            short mouse_y_pos = get_mouse_y_pos( get_game_mouse(getSnakeyNix()) );

            // Subtracting Position%SNAKE_BITMAP_SIZE to keep the apple in the imaginary 20-pixel-sized grid to keep everything alligned
            set_apple_x_pos(get_game_apple(getSnakeyNix()) , mouse_x_pos+(MOUSE_BITMAP_SIZE/2) - (mouse_x_pos+(MOUSE_BITMAP_SIZE/2))%SNAKE_BITMAP_SIZE);
            set_apple_y_pos(get_game_apple(getSnakeyNix()) , mouse_y_pos+(MOUSE_BITMAP_SIZE/2) - (mouse_y_pos+(MOUSE_BITMAP_SIZE/2))%SNAKE_BITMAP_SIZE);
            set_apple_status(get_game_apple(getSnakeyNix()) , APPLE_IS_SET);
        }
    }

    else if (current_state == MAIN_MENU){
        // Check if the player clicked the "Player 1" button
        if (check_button_mouse_collision( get_game_player1_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) )){
            // Notify player 2 that player 1 joined the game
            uart_write_message(PLAYER_1_JOINED);
            uart_get_acknowledgement_byte();

            get_game_state_machine(getSnakeyNix())->current_state = PLAYER_1_PLAYING;
        }

        // Check if the player clicked the "Player 2" button
        else if (check_button_mouse_collision( get_game_player2_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) )){
            // Notify player 1 that player 2 joined the game
            u_char acknowledgement_byte;
            u_char tryCounter = 0;
            while(1){
                uart_write_message(PLAYER_2_JOINED);

                acknowledgement_byte = uart_get_acknowledgement_byte();
                if (acknowledgement_byte == SPP_ACK_BYTE)       // Message acknowledged!
                    break;

                else{   // Message not Acknowledged ;  Wait some time and try again
                    tryCounter ++;
                }

                if (tryCounter == SPP_MAX_NUM_TRIES)
                    break;  // Max number of time reached!
                else
                    tickdelay(micros_to_ticks(SPP_WAIT_TIME));  // Wait some time
            }

            // Check if the message was Acknowledged
            if (acknowledgement_byte == SPP_ACK_BYTE)
                get_game_state_machine(getSnakeyNix())->current_state = PLAYER_2_PLAYING;
            else
                get_game_state_machine(getSnakeyNix())->current_state = WAITING_PLAYER_1;
        }

        // Check if the player clicked the "Exit" button
        else if (check_button_mouse_collision( get_game_exit_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) )){
            endTheGame = 1;
        }

        // Check if the player clicked the "Credits" button
        else if (check_button_mouse_collision( get_game_credits_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) )){
            get_game_state_machine(getSnakeyNix())->current_state = CREDITS;
        }
    }

    else if (current_state == PLAYER_2_PLAYING){
        u_char tryCounter = 0;

        // Check if the player clicked the "Up" button and send the message via serial port
        if (check_button_mouse_collision( get_game_up_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) )){
            while(1){
                uart_write_message(UP_ARROW);

                if (uart_get_acknowledgement_byte() == SPP_ACK_BYTE)    // Message acknowledged!
                    break;
                else{    // Message not acknowledged ; re-send it
                    tryCounter ++;
                }

                if (tryCounter == SPP_MAX_NUM_TRIES)
                    break;  // Max number of time reached!
                else
                    tickdelay(micros_to_ticks(SPP_WAIT_TIME));  // Wait some time
            }
        }

        // Check if the player clicked the "Down" button and send the message via serial port
        else if (check_button_mouse_collision( get_game_down_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) )){
            while(1){
                uart_write_message(DOWN_ARROW);

                if (uart_get_acknowledgement_byte() == SPP_ACK_BYTE)    // Message acknowledged!
                    break;
                else{    // Message not acknowledged ; re-send it
                    tryCounter ++;
                }

                if (tryCounter == SPP_MAX_NUM_TRIES)
                    break;  // Max number of time reached!
                else
                    tickdelay(micros_to_ticks(SPP_WAIT_TIME));  // Wait some time
            }
        }

        // Check if the player clicked the "Left" button and send the message via serial port
        else if (check_button_mouse_collision( get_game_left_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) )){
            while(1){
                uart_write_message(LEFT_ARROW);

                if (uart_get_acknowledgement_byte() == SPP_ACK_BYTE)    // Message acknowledged!
                    break;
                else{    // Message not acknowledged ; re-send it
                    tryCounter ++;
                }

                if (tryCounter == SPP_MAX_NUM_TRIES)
                    break;  // Max number of time reached!
                else
                    tickdelay(micros_to_ticks(SPP_WAIT_TIME));  // Wait some time
            }
        }

        // Check if the player clicked the "Right" button and send the message via serial port
        else if (check_button_mouse_collision( get_game_right_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) )){
            while(1){
                uart_write_message(RIGHT_ARROW);

                if (uart_get_acknowledgement_byte() == SPP_ACK_BYTE)    // Message acknowledged!
                    break;
                else{    // Message not acknowledged ; re-send it
                    tryCounter ++;
                }

                if (tryCounter == SPP_MAX_NUM_TRIES)
                    break;  // Max number of time reached!
                else
                    tickdelay(micros_to_ticks(SPP_WAIT_TIME));  // Wait some time
            }
        }

        // Check if the player clicked the "Exit" button
        else if (check_button_mouse_collision( get_game_small_exit_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            endTheGame = 1;
    }

    else if (current_state == LOSING_SCREEN){
        // Check if the player clicked the "Exit" button
        if (check_button_mouse_collision( get_game_small_exit_button(getSnakeyNix()) , get_game_mouse(getSnakeyNix()) ))
            endTheGame = 1;
    }
}

static void mouse_lb_released_event_handler(){
    if (get_mouse_lb_status(get_game_mouse(getSnakeyNix())) == MOUSE_LB_IS_NOT_PRESSED) // Mouse left button was already not pressed.
        return;
    else    // Mouse left button is not pressed anymore!
        set_mouse_lb_status(get_game_mouse(getSnakeyNix()) , MOUSE_LB_IS_NOT_PRESSED);
}

static void up_button_clicked_event_handler(){
    // The event was acknowledged
    uart_write_character(SPP_ACK_BYTE);

    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING)
        move_apple_up(get_game_apple(getSnakeyNix()));
}

static void down_button_clicked_event_handler(){
    // The event was acknowledged
    uart_write_character(SPP_ACK_BYTE);

    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING)
        move_apple_down(get_game_apple(getSnakeyNix()));
}

static void left_button_clicked_event_handler(){
    // The event was acknowledged
    uart_write_character(SPP_ACK_BYTE);

    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING)
        move_apple_left(get_game_apple(getSnakeyNix()));
}

static void right_button_clicked_event_handler(){
    // The event was acknowledged
    uart_write_character(SPP_ACK_BYTE);

    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING)
        move_apple_right(get_game_apple(getSnakeyNix()));
}

static void rtc_alarm_ring_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == LOSING_SCREEN)
        endTheGame = 1;
}

static void player_1_joined_game_event_handler(){
    // The event was acknowledged
    uart_write_character(SPP_ACK_BYTE);

    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == WAITING_PLAYER_1){
        get_game_state_machine(getSnakeyNix())->current_state = PLAYER_2_PLAYING;
    }
}

static void player_2_joined_game_event_handler(){
    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_1_PLAYING){
        // The event was acknowledged
        uart_write_character(SPP_ACK_BYTE);
    }
}

static void player_1_left_game_event_handler(){
    // The event was acknowledged
    uart_write_character(SPP_ACK_BYTE);

    State current_state = get_game_state_machine(getSnakeyNix())->current_state;

    if (current_state == PLAYER_2_PLAYING){
        get_game_state_machine(getSnakeyNix())->current_state = PLAYER_1_LEFT;
    }
}
