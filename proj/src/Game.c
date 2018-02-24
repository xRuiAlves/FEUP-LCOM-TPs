#include "Game.h"
#include "math.h"
#include "video_gr.h"
#include "timer.h"
#include "kbd.h"
#include "ps2.h"
#include "rtc.h"
#include "font.h"
#include "portaserie.h"

/* Static Functions Prototypes */
static void draw_score();
static void draw_date();


/////////////////////////////////////
/////////////////////////////////////
/*    Game Class Implementation    */
/////////////////////////////////////
/////////////////////////////////////



///////////////////////////////////
//    Attributes  ::  Private    //
///////////////////////////////////


struct Game_Class{
    // Game's attributes
    u_int score;

    // Subscribed interruptions lines in the interruption mask
    int timer_irq_line;
    int mouse_irq_line;
    int uart_irq_line;
    int rtc_irq_line;
    int kbd_irq_line;

    // Game's objects
    Mouse* mouse;
    Snake* snake;
    Apple* apple;
    Button* player1_button;
    Button* player2_button;
    Button* exit_button;
    Button* credits_button;
    Button* up_button;
    Button* down_button;
    Button* left_button;
    Button* right_button;
    Button* small_exit_button;

    // Game's graphics
    Bitmap* background;
    Bitmap* menu_screen;
    Bitmap* credits_screen;
    Bitmap* game_over_screen;
    Bitmap* p1_left_screen;
    Bitmap* p1_waiting_screen;

    // Game's state machine
    StateMachine* state_machine;
};


static void draw_score(Game* game){
    char score[10];
    sprintf(score, "%d", game->score);
    draw_string(score , (game->score==0 ? 1 : floor(log10(abs(game->score))) + 1) , SCORE_X_POS , SCORE_Y_POS , SMALL , getAuxiliaryBuffer()); // floor(log10(abs(game->score))) + 1 calculates the length of the string
}


static void draw_date(){
    draw_string(get_date() , DATE_SIZE , DATE_X_POS , DATE_Y_POS , SMALL , getAuxiliaryBuffer());
}


///////////////////////////////
//    Methods  ::  Public    //
///////////////////////////////


Game* initialize_game(int* interrupts_subscription_success){

    // Allocate space for the "Object"
    Game *game = (Game *) malloc ( sizeof(Game) );

    // Check if the mem. allocation was successful
    if( game == NULL )
        return GAME_INITIALIZATION_FAILED;


    // Load the necessary bitmaps
    Bitmap* mouse_bitmap = loadBitmap(MOUSE_BMP_PATH);
    Bitmap* snake_bitmap = loadBitmap(SNAKE_BMP_PATH);
    Bitmap* apple_bitmap = loadBitmap(APPLE_BMP_PATH);
    Bitmap* background_bitmap = loadBitmap(BCG_BMP_PATH);
    Bitmap* menu_screen_bitmap = loadBitmap(MENU_SCREEN_BMP_PATH);
    Bitmap* credits_screen_bitmap = loadBitmap(CREDITS_SCREEN_BMP_PATH);
    Bitmap* game_over_screen_bitmap = loadBitmap(GAME_OVER_BMP_PATH);
    Bitmap* p1_left_screen_bitmap = loadBitmap(P1_LEFT_BMP_PATH);
    Bitmap* p1_waiting_screen_bitmap = loadBitmap(P1_WAITING_BMP_PATH);
    Bitmap* player1_default_button = loadBitmap(PLAYER1_BUTTON_BMP_1_PATH);
    Bitmap* player1_hovered_button = loadBitmap(PLAYER1_BUTTON_BMP_2_PATH);
    Bitmap* player2_default_button = loadBitmap(PLAYER2_BUTTON_BMP_1_PATH);
    Bitmap* player2_hovered_button = loadBitmap(PLAYER2_BUTTON_BMP_2_PATH);
    Bitmap* exit_default_button = loadBitmap(EXIT_BUTTON_BMP_1_PATH);
    Bitmap* exit_hovered_button = loadBitmap(EXIT_BUTTON_BMP_2_PATH);
    Bitmap* credits_default_button = loadBitmap(CREDITS_BUTTON_BMP_1_PATH);
    Bitmap* credits_hovered_button = loadBitmap(CREDITS_BUTTON_BMP_2_PATH);
    Bitmap* up_default_button_bitmap = loadBitmap(UP_BUTTON_BMP_1_PATH);
    Bitmap* up_hovered_button_bitmap = loadBitmap(UP_BUTTON_BMP_2_PATH);
    Bitmap* down_default_button_bitmap = loadBitmap(DOWN_BUTTON_BMP_1_PATH);
    Bitmap* down_hovered_button_bitmap = loadBitmap(DOWN_BUTTON_BMP_2_PATH);
    Bitmap* left_default_button_bitmap = loadBitmap(LEFT_BUTTON_BMP_1_PATH);
    Bitmap* left_hovered_button_bitmap = loadBitmap(LEFT_BUTTON_BMP_2_PATH);
    Bitmap* right_default_button_bitmap = loadBitmap(RIGHT_BUTTON_BMP_1_PATH);
    Bitmap* right_hovered_button_bitmap = loadBitmap(RIGHT_BUTTON_BMP_2_PATH);
    Bitmap* small_exit_default_button_bitmap = loadBitmap(SMALL_EXIT_BUTTON_BMP_1_PATH);
    Bitmap* small_exit_hovered_button_bitmap = loadBitmap(SMALL_EXIT_BUTTON_BMP_2_PATH);

    // Check if all the bitmaps where successfully loaded
    if (mouse_bitmap == NULL){
        *interrupts_subscription_success = MOUSE_BMP_NOT_FOUND;
        return NULL;
    }
    else if (snake_bitmap == NULL){
        *interrupts_subscription_success = SNAKE_BMP_NOT_FOUND;
        return NULL;
    }
    else if (apple_bitmap == NULL){
        *interrupts_subscription_success = APPLE_BMP_NOT_FOUND;
        return NULL;
    }
    else if (background_bitmap == NULL){
        *interrupts_subscription_success = BACKGROUND_BMP_NOT_FOUND;
        return NULL;
    }
    else if (menu_screen_bitmap == NULL){
        *interrupts_subscription_success = MENU_SCREEN_BMP_NOT_FOUND;
        return NULL;
    }
    else if (credits_screen_bitmap == NULL){
        *interrupts_subscription_success = CREDITS_SCREEN_BMP_NOT_FOUND;
        return NULL;
    }
    else if (player1_default_button==NULL || player1_hovered_button==NULL || player2_default_button==NULL || player2_hovered_button==NULL ||
             exit_default_button==NULL || exit_hovered_button==NULL || up_default_button_bitmap==NULL || up_hovered_button_bitmap==NULL ||
             down_default_button_bitmap==NULL || down_hovered_button_bitmap==NULL || left_default_button_bitmap==NULL || left_hovered_button_bitmap==NULL ||
             right_default_button_bitmap==NULL || right_hovered_button_bitmap==NULL || small_exit_default_button_bitmap==NULL ||
             small_exit_hovered_button_bitmap==NULL || credits_default_button==NULL || credits_hovered_button==NULL){

        *interrupts_subscription_success = BUTTON_BMP_NOT_FOUND;
        return NULL;
    }
    else if (game_over_screen_bitmap == NULL){
        *interrupts_subscription_success = LOSING_SCREEN_BMP_NOT_FOUND;
        return NULL;
    }
    else if (p1_left_screen_bitmap == NULL){
        *interrupts_subscription_success = P1_LEFT_BMP_NOT_FOUND;
        return NULL;
    }
    else if (p1_waiting_screen_bitmap == NULL){
        *interrupts_subscription_success = P1_WAITING_BMP_NOT_FOUND;
        return NULL;
    }


    // Initialize game's attributes
    game->p1_left_screen = p1_left_screen_bitmap;
    game->p1_waiting_screen = p1_waiting_screen_bitmap;
    game->background = background_bitmap;
    game->menu_screen = menu_screen_bitmap;
    game->credits_screen = credits_screen_bitmap;
    game->game_over_screen = game_over_screen_bitmap;
    game->score = 0;


    // Initialize font
    initialize_font();


    // Subscribe Timer Interrupts and check for success
    game->timer_irq_line = timer_subscribe_int();

    if (game->timer_irq_line < 0){
        *interrupts_subscription_success = TIMER_INT_SUBSCRIBE_FAILED;
        return NULL;
    }


    // Subscribe Keyboard Interrupts and check for success
    game->kbd_irq_line = kbd_subscribe_int();

    if (game->kbd_irq_line < 0){
        *interrupts_subscription_success = KBD_INT_SUBSCRIBE_FAILED;
        return NULL;
    }


    // Subscribe Mouse Interrupts and check for success
    game->mouse_irq_line = ps2_subscribe_int();

    if (game->mouse_irq_line < 0){
        *interrupts_subscription_success = MOUSE_INT_SUBSCRIBE_FAILED;
        return NULL;
    }

    ps2_enable_dataReporting();     // Enable Mouse's data reporting (in stream mode)


    // Configure the UART and check for success
    if (configure_uart_SnakeyNix() != OK){
        *interrupts_subscription_success = UART_CONFIGURATION_FAILED;
        return NULL;
    }


    // Subscribe UART Interrupts and check for success
    game->uart_irq_line = uart_subscribe_int();

    if (game->uart_irq_line < 0){
        *interrupts_subscription_success = UART_INT_SUBSCRIBE_FAILED;
        return NULL;
    }


    // Configure the RTC and check for success
    if (configure_rtc_SnakeyNix() != OK){
        *interrupts_subscription_success = RTC_CONFIGURATION_FAILED;
        return NULL;
    }


    // Subscribe RTC Interrupts and check for success
    game->rtc_irq_line = rtc_subscribe_int();

    if (game->rtc_irq_line < 0){
        *interrupts_subscription_success = RTC_INT_SUBSCRIBE_FAILED;
        return NULL;
    }


    // Create button objets for the game and check if it was successfully created
    game->player1_button = create_button(PLAYER1_BUTTON_X_POS , PLAYER1_BUTTON_Y_POS , player1_default_button , player1_hovered_button);
    game->player2_button = create_button(PLAYER2_BUTTON_X_POS , PLAYER2_BUTTON_Y_POS , player2_default_button , player2_hovered_button);
    game->exit_button = create_button(EXIT_BUTTON_X_POS , EXIT_BUTTON_Y_POS , exit_default_button , exit_hovered_button);
    game->credits_button = create_button(CREDITS_BUTTON_X_POS , CREDITS_BUTTON_Y_POS , credits_default_button , credits_hovered_button);
    game->up_button = create_button(UP_BUTTON_X_POS , UP_BUTTON_Y_POS , up_default_button_bitmap , up_hovered_button_bitmap);
    game->down_button = create_button(DOWN_BUTTON_X_POS , DOWN_BUTTON_Y_POS , down_default_button_bitmap , down_hovered_button_bitmap);
    game->left_button = create_button(LEFT_BUTTON_X_POS , LEFT_BUTTON_Y_POS , left_default_button_bitmap , left_hovered_button_bitmap);
    game->right_button = create_button(RIGHT_BUTTON_X_POS , RIGHT_BUTTON_Y_POS , right_default_button_bitmap , right_hovered_button_bitmap);
    game->small_exit_button = create_button(SMALL_EXIT_BUTTON_X_POS , SMALL_EXIT_BUTTON_Y_POS , small_exit_default_button_bitmap , small_exit_hovered_button_bitmap);

    if (game->player1_button==NULL || game->player2_button==NULL || game->exit_button==NULL || game->up_button==NULL ||
        game->down_button==NULL || game->left_button==NULL || game->right_button==NULL || game->small_exit_button==NULL ){

        *interrupts_subscription_success = BUTTON_OBJ_CREATION_FAILED;
        return NULL;
    }


    // Create a mouse object for the game and check if it was successfully created
    game->mouse = create_mouse(MOUSE_INITIAL_X_POS , MOUSE_INITIAL_Y_POS , mouse_bitmap);

    if (game->mouse == NULL){
        *interrupts_subscription_success = MOUSE_OBJ_CREATION_FAILED;
        return NULL;
    }


    // Create a snake object for the game and check if it was successfully created
    game->snake = create_snake(SNAKE_INITIAL_SIZE , SNAKE_INITIAL_X_POS , SNAKE_INITIAL_Y_POS , RIGHT , snake_bitmap);

    if (game->snake == NULL){
        *interrupts_subscription_success = SNAKE_OBJ_CREATION_FAILED;
        return NULL;
    }


    // Create an apple object for the game and check if it was successfully created
    game->apple = create_apple(APPLE_INITIAL_X_POS , APPLE_INITIAL_Y_POS , apple_bitmap);

    if (game->apple == NULL){
        *interrupts_subscription_success = APPLE_OBJ_CREATION_FAILED;
        return NULL;
    }


    // Create a State Machine object for the game and check if it was successfully created
    game->state_machine = initialize_state_machine();

    if (game->state_machine == NULL){
        *interrupts_subscription_success = SM_OBJ_CREATION_FAILED;
        return NULL;
    }


    // Return the newly created game's address
    *interrupts_subscription_success = OK;
    return game;

}

void end_game(Game* game){

    // Check if the game is existant
    if ( game == NULL )
        return;

    // Free the memory allocated for the font bitmaps
    terminate_font();

    // Free the memory allocated for the Game's objects
    delete_mouse(game->mouse);
    delete_snake(game->snake);
    delete_apple(game->apple);
    delete_button(game->player1_button);
    delete_button(game->player2_button);
    delete_button(game->exit_button);
    delete_button(game->credits_button);
    delete_button(game->up_button);
    delete_button(game->down_button);
    delete_button(game->left_button);
    delete_button(game->right_button);
    delete_button(game->small_exit_button);

    // Terminate the state machine
    terminate_state_machine(game->state_machine);

    // Fre the memory allocated for game's bitmaps (other bitmaps are freed in their own classes)
    deleteBitmap(game->background);
    deleteBitmap(game->p1_left_screen);
    deleteBitmap(game->p1_waiting_screen);
    deleteBitmap(game->game_over_screen);

    // Unsubscribe the subscribed interrupts
    timer_unsubscribe_int();
    kbd_unsubscribe_int();
    ps2_disable_dataReporting();    // Disable the data reporting (in stream mode)
    ps2_unsubscribe_int();
    uart_unsubscribe_int();
    rtc_unsubscribe_int();

    // Free the memory allocated for the game
    free(game);

    // The game is now un-existant
    game = NULL;

}


void update_game(Game* game){
    int ipc_status;
    message msg;

    if (driver_receive(ANY, &msg, &ipc_status) != 0 ) // Not an interruption / invalid data, return
       return;

    // Check if a notification was received
    if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
        // Check if the message source is hardware related
        case HARDWARE:
            // Keyboard Interrupt
            if (msg.NOTIFY_ARG & BIT(game->kbd_irq_line)){
                handle_keyboard();
            }

            // Mouse Interrupt
            if (msg.NOTIFY_ARG & BIT(game->mouse_irq_line)){
                handle_mouse();
            }

            // UART Interrupt
            if (msg.NOTIFY_ARG & BIT(game->uart_irq_line)){
                handle_uart();
            }

            // RTC Interrupt
            if (msg.NOTIFY_ARG & BIT(game->rtc_irq_line)){
                handle_rtc();
            }

            // Timer Interrupt
            if (msg.NOTIFY_ARG & BIT(game->timer_irq_line)){
                handle_timer();
            }



            break;

        default:    // Not a hardware related notification, ignore it
            break;
        }
    }

    handle_events(game->state_machine);
}


void draw_player1_game(Game* game){
    // Draw the background
    drawBitmap(game->background , 0 , 0 , getAuxiliaryBuffer());

    // Draw the game objects
    draw_apple(game->apple);
    draw_snake(game->snake);

    // Draw the score
    draw_score(game);

    // Draw the date
    draw_date();

    // Draw the mouse
    draw_mouse(game->mouse);

}

void draw_player2_game(Game* game){
    // Draw the background
    drawBitmap(game->background , 0 , 0 , getAuxiliaryBuffer());

    // Draw the buttons
    draw_button(game->up_button);
    draw_button(game->down_button);
    draw_button(game->left_button);
    draw_button(game->right_button);
    draw_button(game->small_exit_button);

    // Draw the mouse
    draw_mouse(game->mouse);
}


void draw_game_menu(Game* game){
    // Draw the background
    drawBitmap(game->menu_screen , 0 , 0 , getAuxiliaryBuffer());

    // Draw the buttons
    draw_button(game->player1_button);
    draw_button(game->player2_button);
    draw_button(game->exit_button);
    draw_button(game->credits_button);

    // Draw the mouse
    draw_mouse(game->mouse);
}


void draw_game_losing_screen(Game* game){
    // Draw the losing screen
    drawBitmap(game->game_over_screen , 0 , 0 , getAuxiliaryBuffer());

    // Draw the exit button
    draw_button(game->small_exit_button);

    // Draw the score
    draw_final_score(game);

    // Draw the mouse
    draw_mouse(game->mouse);
}


void draw_game_p1_left_screen(Game* game){
    // Draw the losing screen
    drawBitmap(game->p1_left_screen , 0 , 0 , getAuxiliaryBuffer());
}


void draw_game_p1_waiting_screen(Game* game){
    // Draw the losing screen
    drawBitmap(game->p1_waiting_screen , 0 , 0 , getAuxiliaryBuffer());
}

void draw_game_credits_screen(Game* game){
    // Draw the credits screen
    drawBitmap(game->credits_screen , 0 , 0 , getAuxiliaryBuffer());
}


void draw_final_score(Game* game){
    char score[10];
    sprintf(score, "%d", game->score);

    // Compute the correct X coordinate to make the score centered
    u_char score_size = (game->score==0 ? 1 : floor(log10(abs(game->score))) + 1);   // floor(log10(abs(game->score))) + 1 calculates the length of the string
    u_short x_pos = getHorResolution()/2 + LOSING_SCREEN_CENTERING_OFFSET - score_size*BIG_DIGIT_SIZE/2 - (score_size-1)*DIGIT_SEPARATION_SIZE/2;

    draw_string(score , score_size , x_pos , FINAL_SCORE_Y_POS , BIG , getAuxiliaryBuffer());
}


Snake* get_game_snake(Game* game){
    return game->snake;
}


Apple* get_game_apple(Game* game){
    return game->apple;
}


Mouse* get_game_mouse(Game* game){
    return game->mouse;
}


StateMachine* get_game_state_machine(Game* game){
    return game->state_machine;
}


Button* get_game_player1_button(Game* game){
    return game->player1_button;
}


Button* get_game_player2_button(Game* game){
    return game->player2_button;
}


Button* get_game_exit_button(Game* game){
    return game->exit_button;
}


Button* get_game_credits_button(Game* game){
    return game->credits_button;
}


Button* get_game_up_button(Game* game){
    return game->up_button;
}


Button* get_game_down_button(Game* game){
    return game->down_button;
}


Button* get_game_left_button(Game* game){
    return game->left_button;
}


Button* get_game_right_button(Game* game){
    return game->right_button;
}


Button* get_game_small_exit_button(Game* game){
    return game->small_exit_button;
}


void increase_game_score(Game* game){
    game->score += 10;
}
