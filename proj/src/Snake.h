#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "utilities.h"
#include "Bitmap.h"
#include "Apple.h"
#include "Mouse.h"

/** @defgroup Snake Snake
 * @{
 * Functions for manipulating the "objects" of the "Class" Snake
 */

//////////////////////////
//  Symbolic Constants  //
//////////////////////////

#define SNAKE_ALLOC_FAILED    NULL
#define SNAKE_SPEED           20
#define SNAKE_BITMAP_SIZE     20

#define COLLISION_FOUND       1
#define NO_COLLISION_FOUND    0


/////////////////////
//  Snake "Class"  //
/////////////////////

struct Snake_Class;
typedef struct Snake_Class Snake;

typedef enum { RIGHT=0 , UP , LEFT , DOWN } direction;     // types of directions


/////////////////////
//  Snake Methods  //
/////////////////////

/**
 * @brief Creates an Snake "object", that will be represented on the screen by node_bmp at the position (x_pos, y_pos)
 *
 * @param initial_size - the snake's initial size
 * @param x_pos - the x position on the screen where the snake will be created
 * @param y_pos - the y position on the screen where the snake will be created
 * @param dir - the initial direction of the snake's movement
 * @param node_bmp - the bitmap that represents each one of the snake's nodes
 *
 * @return Snake* - the address of the snake that was just created
 */
Snake* create_snake(u_short initial_size , short x_pos , short y_pos , direction dir , Bitmap* node_bmp);

/**
 * @brief Deletes the Snake "object" passed by parameter, thus freeing the memory occupied by it
 *
 * @param snake - a Snake pointer
 *
 * @return void
 *
 */
void delete_snake(Snake* snake);

/**
 * @brief "Moves" the snake by erasing its tail and creating a new node in front of its head
 *
 * @param snake - a Snake pointer
 *
 * @return void
 */
void move_snake(Snake* snake);

/**
 * @brief Draws on the screen the snake passed by parameter
 *
 * @param snake - a Snake pointer
 *
 * @return void
 *
 */
void draw_snake(Snake* snake);

/**
 * @brief Sets the snake's current_direction attribute to new_direction
 *
 * @param snake - a Snake pointer
 * @param new_direction - the snake's new direction
 *
 * @return void
 */
void set_snake_direction(Snake* snake , direction new_direction);

/**
 * @brief Returns the current value of the current_direction attribute of the snake passed by parameter
 *
 * @param snake - a Snake pointer
 *
 * @return direction - the current snake's direction
 *
 */
direction get_snake_direction(Snake* snake);

/**
 * @brief Increments by one the snake's size attribute
 *
 * @param snake - a Snake pointer
 *
 * @return void
 *
 */
void grow_snake(Snake* snake);

/**
 * @brief Goes through all the snake's nodes and checks if there is a colision with the snake's head
 *
 * @param snake - a Snake pointer
 *
 * @return int - COLLISION_FOUND(1) or NO_COLLISION_FOUND(0)
 *
 */
int check_snake_self_collision(Snake* snake);

/**
 * @brief Checks if there is a colision between the snake's head and an apple
 *
 * @param snake - a Snake pointer
 * @param apple - an Apple pointer
 *
 * @return int - COLLISION_FOUND(1) or NO_COLLISION_FOUND(0)
 *
 */
int check_snake_head_apple_colision(Snake* snake, Apple* apple);

/**
 * @brief Checks if there is a colision between any of the snake's nodes and the mouse click position
 *
 * @param snake - a Snake pointer
 * @param mouse - a Mouse pointer
 *
 * @return int - COLLISION_FOUND(1) or NO_COLLISION_FOUND(0)
 *
 */
int check_snake_mouse_click_collision(Snake* snake , Mouse* mouse);

 /** @} end of Snake */

#endif  /* _SNAKE_H_ */
