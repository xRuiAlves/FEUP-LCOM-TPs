#ifndef _APPLE_H_
#define _APPLE_H_

#include "utilities.h"
#include "Bitmap.h"

/** @defgroup Apple Apple
 * @{
 * Functions for manipulating the "objects" of the "Class" Apple
 */

//////////////////////////
//  Symbolic Constants  //
//////////////////////////

#define APPLE_ALLOC_FAILED    NULL
#define APPLE_IS_SET          1
#define APPLE_NOT_SET         0
#define APPLE_SIZE            20


/////////////////////
//  Apple "Class"  //
/////////////////////

struct Apple_Class;
typedef struct Apple_Class Apple;


/////////////////////
//  Apple Methods  //
/////////////////////

/**
 * @brief Creates an Apple "object", that will be represented on the screen by apple_bmp at the position (x_pos, y_pos)
 *
 * @param x_pos - the x position on the screen where the apple will be created
 * @param y_pos - the y position on the screen where the apple will be created
 * @param apple_bmp - the bitmap that represents the apple "object"
 *
 * @return Apple* - the address of the apple that was just created
 */
Apple* create_apple(short x_pos , short y_pos , Bitmap* apple_bmp);

/**
 * @brief Deletes the Apple "object" passed by parameter, thus freeing the memory occupied by it
 *
 * @param apple - an Apple pointer
 *
 * @return void
 */
void delete_apple(Apple* apple);

/**
 * @brief Draws on the screen the apple passed by parameter
 *
 * @param apple - an Apple pointer
 *
 * @return void
 */
void draw_apple(Apple* apple);

/**
 * @brief Sets the apple's x_pos attribute to x_pos value passed by parameter
 *
 * @param apple - an Apple pointer
 * @param x_pos
 *
 * @return void
 */
void set_apple_x_pos(Apple* apple , short x_pos);

/**
 * @brief Sets the apple's y_pos attribute to y_pos value passed by parameter
 *
 * @param apple - an Apple pointer
 * @param y_pos
 *
 * @return void
 */
void set_apple_y_pos(Apple* apple , short y_pos);

/**
 * @brief Sets the apple's status (is_active attribute) to new_status value passed by parameter
 *
 * @param apple - an Apple pointer
 * @param new_status
 *
 * @return void
 */
void set_apple_status(Apple* apple , int new_status);

/**
 * @brief Returns the current value of the x_pos attribute of the apple passed by parameter
 *
 * @param apple - an Apple pointer
 *
 * @return short - the current apple's x_position
 */
short get_apple_x_pos(Apple* apple);

/**
 * @brief Returns the current value of the y_pos attribute of the apple passed by parameter
 *
 * @param apple - an Apple pointer
 *
 * @return short - the current apple's y_position
 */
short get_apple_y_pos(Apple* apple);

/**
 * @brief Returns the current value of the is_active attribute of the apple passed by parameter
 *
 * @param apple - an Apple pointer
 *
 * @return short - the current apple's status (is_active attribute)
 */
int get_apple_status(Apple* apple);

/**
 * @brief Moves the apple to the right, keeping it inside screen borders
 *
 * @return void
 */
void move_apple_right(Apple* apple);

/**
 * @brief Moves the apple to the left, keeping it inside screen borders
 *
 * @return void
 */
void move_apple_left(Apple* apple);

/**
 * @brief Moves the apple down, keeping it inside screen borders
 *
 * @return void
 */
void move_apple_down(Apple* apple);

/**
 * @brief Moves the apple up, keeping it inside screen borders
 *
 * @return void
 */
void move_apple_up(Apple* apple);

 /** @} end of Apple */

#endif /* _APPLE_H_ */
