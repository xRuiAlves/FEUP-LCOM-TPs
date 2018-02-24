#ifndef _GAME_H_
#define _GAME_H_

#include "Mouse.h"
#include "Apple.h"
#include "Snake.h"
#include "Button.h"
#include "Bitmap.h"
#include "StateMachine.h"

/** @defgroup Game Game
 * @{
 * Functions for controling the game
 */

//////////////////////////////////////////////
//  Symbolic Constants for Error detection  //
//////////////////////////////////////////////

#define GAME_INITIALIZATION_FAILED    NULL
#define TIMER_INT_SUBSCRIBE_FAILED    1
#define KBD_INT_SUBSCRIBE_FAILED      2
#define MOUSE_INT_SUBSCRIBE_FAILED    3
#define UART_INT_SUBSCRIBE_FAILED     4
#define RTC_INT_SUBSCRIBE_FAILED      5
#define UART_CONFIGURATION_FAILED     6
#define RTC_CONFIGURATION_FAILED      7
#define MOUSE_OBJ_CREATION_FAILED     8
#define SNAKE_OBJ_CREATION_FAILED     9
#define APPLE_OBJ_CREATION_FAILED     10
#define BUTTON_OBJ_CREATION_FAILED    11
#define SM_OBJ_CREATION_FAILED        12
#define SNAKE_BMP_NOT_FOUND           13
#define BACKGROUND_BMP_NOT_FOUND      14
#define MOUSE_BMP_NOT_FOUND           15
#define APPLE_BMP_NOT_FOUND           16
#define BUTTON_BMP_NOT_FOUND          17
#define LOSING_SCREEN_BMP_NOT_FOUND   18
#define P1_LEFT_BMP_NOT_FOUND         19
#define P1_WAITING_BMP_NOT_FOUND      20
#define MENU_SCREEN_BMP_NOT_FOUND     21
#define CREDITS_SCREEN_BMP_NOT_FOUND  22


///////////////////////////////////////
//  Game-Related Symbolic Constants  //
///////////////////////////////////////

#define GAME_UPDATE_RATE         2     // Frame Rate = 60/(update rate) = 60/2 = 30 FPS
#define SNAKE_INITIAL_SIZE       5
#define SNAKE_INITIAL_X_POS      100
#define SNAKE_INITIAL_Y_POS      100
#define MOUSE_INITIAL_X_POS      400
#define MOUSE_INITIAL_Y_POS      300
#define APPLE_INITIAL_X_POS      200
#define APPLE_INITIAL_Y_POS      200
#define SCORE_X_POS              14
#define SCORE_Y_POS              570
#define FINAL_SCORE_Y_POS        410
#define DATE_SIZE                19
#define DATE_X_POS               446
#define DATE_Y_POS               570
#define PLAYER1_BUTTON_X_POS     27
#define PLAYER1_BUTTON_Y_POS     280
#define PLAYER2_BUTTON_X_POS     414
#define PLAYER2_BUTTON_Y_POS     280
#define EXIT_BUTTON_X_POS        27
#define EXIT_BUTTON_Y_POS        440
#define CREDITS_BUTTON_X_POS     414
#define CREDITS_BUTTON_Y_POS     440
#define UP_BUTTON_X_POS          329
#define UP_BUTTON_Y_POS          148
#define DOWN_BUTTON_X_POS        329
#define DOWN_BUTTON_Y_POS        310
#define LEFT_BUTTON_X_POS        167
#define LEFT_BUTTON_Y_POS        310
#define RIGHT_BUTTON_X_POS       491
#define RIGHT_BUTTON_Y_POS       310
#define SMALL_EXIT_BUTTON_X_POS  679
#define SMALL_EXIT_BUTTON_Y_POS  539
#define LOSING_SCREEN_CENTERING_OFFSET  6       // For correctly centering the score in the losing screen

///////////////////////////////////////////////////
//  CONSTANTS FOR COMMUNICATING BETWEEN PLAYERS  //
///////////////////////////////////////////////////

#define UP_ARROW         1
#define DOWN_ARROW       2
#define LEFT_ARROW       3
#define RIGHT_ARROW      4
#define PLAYER_1_JOINED  5
#define PLAYER_2_JOINED  6
#define PLAYER_1_LEFT    7


///////////////////////////////////////////
//  Graphics-Related Symbolic Constants  //
///////////////////////////////////////////

#define MOUSE_BMP_PATH                "/home/SnakeyNix/mouse.bmp"
#define SNAKE_BMP_PATH                "/home/SnakeyNix/snake.bmp"
#define APPLE_BMP_PATH                "/home/SnakeyNix/apple.bmp"
#define BCG_BMP_PATH                  "/home/SnakeyNix/background.bmp"
#define MENU_SCREEN_BMP_PATH          "/home/SnakeyNix/menuScreen.bmp"
#define CREDITS_SCREEN_BMP_PATH       "/home/SnakeyNix/creditsScreen.bmp"
#define GAME_OVER_BMP_PATH            "/home/SnakeyNix/losingScreen.bmp"
#define P1_LEFT_BMP_PATH              "/home/SnakeyNix/p1leftScreen.bmp"
#define P1_WAITING_BMP_PATH           "/home/SnakeyNix/p1waitingScreen.bmp"
#define PLAYER1_BUTTON_BMP_1_PATH     "/home/SnakeyNix/player1DefaultButton.bmp"
#define PLAYER1_BUTTON_BMP_2_PATH     "/home/SnakeyNix/player1HoveredButton.bmp"
#define PLAYER2_BUTTON_BMP_1_PATH     "/home/SnakeyNix/player2DefaultButton.bmp"
#define PLAYER2_BUTTON_BMP_2_PATH     "/home/SnakeyNix/player2HoveredButton.bmp"
#define EXIT_BUTTON_BMP_1_PATH        "/home/SnakeyNix/exitDefaultButton.bmp"
#define EXIT_BUTTON_BMP_2_PATH        "/home/SnakeyNix/exitHoveredButton.bmp"
#define CREDITS_BUTTON_BMP_1_PATH     "/home/SnakeyNix/creditsDefaultButton.bmp"
#define CREDITS_BUTTON_BMP_2_PATH     "/home/SnakeyNix/creditsHoveredButton.bmp"
#define UP_BUTTON_BMP_1_PATH          "/home/SnakeyNix/upDefaultButton.bmp"
#define UP_BUTTON_BMP_2_PATH          "/home/SnakeyNix/upHoveredButton.bmp"
#define DOWN_BUTTON_BMP_1_PATH        "/home/SnakeyNix/downDefaultButton.bmp"
#define DOWN_BUTTON_BMP_2_PATH        "/home/SnakeyNix/downHoveredButton.bmp"
#define LEFT_BUTTON_BMP_1_PATH        "/home/SnakeyNix/leftDefaultButton.bmp"
#define LEFT_BUTTON_BMP_2_PATH        "/home/SnakeyNix/leftHoveredButton.bmp"
#define RIGHT_BUTTON_BMP_1_PATH       "/home/SnakeyNix/rightDefaultButton.bmp"
#define RIGHT_BUTTON_BMP_2_PATH       "/home/SnakeyNix/rightHoveredButton.bmp"
#define SMALL_EXIT_BUTTON_BMP_1_PATH  "/home/SnakeyNix/smallExitDefaultButton.bmp"
#define SMALL_EXIT_BUTTON_BMP_2_PATH  "/home/SnakeyNix/smallExitHoveredButton.bmp"



////////////////////
//  Game "Class"  //
////////////////////

struct Game_Class;
typedef struct Game_Class Game;


////////////////////
//  Game Methods  //
////////////////////

/**
 * @brief Creates the Game "object" , initializes all the devices and bitmaps and subscribes the interruptions
 *
 * @param interrupts_subscription_success - the address of a variable to be filled with a value corresponding to a given device or bitmap if their initialization or interruption subscription fails, 0 if OK
 *
 * @return Game* - the address of the Game "object" just created
 *
 */
Game* initialize_game(int* interrupts_subscription_success);

/**
 * @brief Terminates the game by unsubscribing all the interrutions and deleting all "objects", thus freeing the memory occupied by it
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void end_game(Game* game);

/**
 * @brief Manages all interruptions and calls the correct interruption handlers
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void update_game(Game* game);

/**
 * @brief Draws on the screen all the elements of the game (Player 1 view)
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void draw_player1_game(Game* game);

/**
 * @brief Draws on the screen all the elements of the game (Player 2 view)
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void draw_player2_game(Game* game);

/**
 * @brief Draws on the screen all the elements of the menu
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void draw_game_menu(Game* game);

/**
 * @brief Draws on the screen the "losing screen"
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void draw_game_losing_screen(Game* game);

/**
 * @brief Draws on the screen the "player 1 left screen"
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void draw_game_p1_left_screen(Game* game);

/**
 * @brief Draws on the screen the "waiting for player 1 screen"
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void draw_game_p1_waiting_screen(Game* game);

/**
 * @brief Draws on the screen the "credits screen"
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void draw_game_credits_screen(Game* game);

/**
 * @brief Increases the game's score attribute by 10
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void increase_game_score(Game* game);

/**
 * @brief Draws on the screen the final game score (score attribute)
 *
 * @param game - a Game pointer
 *
 * @return void
 *
 */
void draw_final_score(Game* game);

/**
 * @brief Returns the address of the game's snake attribute
 *
 * @param game - a Game pointer
 *
 * @return Snake* - returns the game's snake attribute
 *
 */
Snake* get_game_snake(Game* game);

/**
 * @brief Returns the address of the game's apple attribute
 *
 * @param game - a Game pointer
 *
 * @return Apple* - returns the game's apple attribute
 *
 */
Apple* get_game_apple(Game* game);

/**
 * @brief Returns the address of the game's mouse attribute
 *
 * @param game - a Game pointer
 *
 * @return Mouse* - returns the game's mouse attribute
 *
 */
Mouse* get_game_mouse(Game* game);

/**
 * @brief Returns the address of the game's state_machine attribute
 *
 * @param game - a Game pointer
 *
 * @return StateMachine* - returns the game's StateMachine attribute
 *
 */
StateMachine* get_game_state_machine(Game* game);

/**
 * @brief Returns the address of the game's player1_button attribute
 *
 * @param game - a Game pointer
 *
 * @return Button* - returns the game's player1_button attribute
 *
 */
Button* get_game_player1_button(Game* game);

/**
 * @brief Returns the address of the game's player2_button attribute
 *
 * @param game - a Game pointer
 *
 * @return Button* - returns the game's player2_button attribute
 *
 */
Button* get_game_player2_button(Game* game);

/**
 * @brief Returns the address of the game's exit_button attribute
 *
 * @param game - a Game pointer
 *
 * @return Button* - returns the game's exit_button attribute
 *
 */
Button* get_game_exit_button(Game* game);

/**
 * @brief Returns the address of the game's credits_button attribute
 *
 * @param game - a Game pointer
 *
 * @return Button* - returns the game's credits_button attribute
 *
 */
Button* get_game_credits_button(Game* game);

/**
 * @brief Returns the address of the game's up_button attribute
 *
 * @param game - a Game pointer
 *
 * @return Button* - returns the game's up_button attribute
 *
 */
Button* get_game_up_button(Game* game);

/**
 * @brief Returns the address of the game's down_button attribute
 *
 * @param game - a Game pointer
 *
 * @return Button* - returns the game's down_button attribute
 *
 */
Button* get_game_down_button(Game* game);

/**
 * @brief Returns the address of the game's left_button attribute
 *
 * @param game - a Game pointer
 *
 * @return Button* - returns the game's left_button attribute
 *
 */
Button* get_game_left_button(Game* game);

/**
 * @brief Returns the address of the game's right_button attribute
 *
 * @param game - a Game pointer
 *
 * @return Button* - returns the game's right_button attribute
 *
 */
Button* get_game_right_button(Game* game);

/**
 * @brief Returns the address of the game's small_exit_button attribute
 *
 * @param game - a Game pointer
 *
 * @return Button* - returns the game's small_exit_button attribute
 *
 */
Button* get_game_small_exit_button(Game* game);

 /** @} end of Game */

#endif /* _GAME_H_ */
