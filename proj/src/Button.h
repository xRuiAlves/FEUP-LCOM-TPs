#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "utilities.h"
#include "Bitmap.h"
#include "Mouse.h"

/** @defgroup Button Button
 * @{
 * Functions for manipulating the "objects" of the "Class" Button
 */

//////////////////////////
//  Symbolic Constants  //
//////////////////////////

#define BUTTON_ALLOC_FAILED    NULL
#define BUTTON_DEFAULT_STATUS  0
#define BUTTON_HOVERED_STATUS  1
#define NO_COLLISION_FOUND     0
#define COLLISION_FOUND        1


//////////////////////
//  Button "Class"  //
//////////////////////

struct Button_Class;
typedef struct Button_Class Button;


////////////////////
//  Game Methods  //
////////////////////

/**
 * @brief Creates a Button "object", that will be represented on the screen by default_bmp at the position (x_pos, y_pos), or by hovered_bmp if the mouse is within its coordinates
 *
 * @param x_pos - the x position on the screen where the button will be created
 * @param y_pos - the y position on the screen where the button will be created
 * @param default_bmp - the bitmap that represents the button "object" in its normal state
 * @param hovered_bmp - the bitmap that represents the button "object" when the mouse is above it
 *
 * @return Button* - the address of the button that was just created
 *
 */
Button* create_button(short x_pos , short y_pos , Bitmap* default_bmp , Bitmap* hovered_bmp);

/**
 * @brief Deletes the Button "object" passed by parameter, thus freeing the memory occupied by it
 *
 * @param button - a Button pointer
 *
 * @return void
 *
 */
void delete_button(Button* button);

/**
 * @brief Draws on the screen the button passed by parameter
 *
 * @param button - a Button pointer
 *
 * @return void
 *
 */
void draw_button(Button* button);

/**
 * @brief Returns the current value of the status attribute of the button passed by parameter
 *
 * @param button - a Button pointer
 *
 * @return int - the current button's status (status attribute)
 *
 */
int get_button_status(Button* button);

/**
 * @brief Returns the height of the button passed by parameter
 *
 * @param button - a Button pointer
 *
 * @return short - the button's height
 *
 */
short get_button_height(Button* button);

/**
 * @brief Returns the width of the button passed by parameter
 *
 * @param button - a Button pointer
 *
 * @return short - the button's width
 *
 */
short get_button_width(Button* button);

/**
* @brief Sets the button's status (status attribute) to new_status value passed by parameter
*
* @param button - a Button pointer
* @param new_status
*
* @return void
*
*/
void set_button_status(Button* button , int new_status);

/**
 * @brief Checks the mouse and button colision, i.e. if the mouse is on the button or outside it
 *
 * @param button - a Button pointer
 * @param mouse - a mouse pointer
 *
 * @return int - Returns COLLISION_FOUND(1) or NO_COLLISION_FOUND(0)
 */
int check_button_mouse_collision(Button* button , Mouse* mouse);

 /** @} end of Button */

#endif /* _BUTTON_H_ */
