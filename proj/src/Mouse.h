#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "utilities.h"
#include "Bitmap.h"

/** @defgroup Mouse Mouse
 * @{
 * Functions for manipulating the "objects" of the "Class" Mouse
 */

//////////////////////////
//  Symbolic Constants  //
//////////////////////////

#define MOUSE_ALLOC_FAILED        NULL
#define MOUSE_BITMAP_SIZE         8
#define MOUSE_LB_IS_PRESSED       1
#define MOUSE_LB_IS_NOT_PRESSED   0


/////////////////////
//  Mouse "Class"  //
/////////////////////

struct Mouse_Class;
typedef struct Mouse_Class Mouse;


/////////////////////
//  Mouse Methods  //
/////////////////////

/**
 * @brief Creates an Mouse "object", that will be represented on the screen by mouse_bitmap at the position (mouse_x_coord, mouse_y_coord)
 *
 * @param mouse_x_coord - the value which x_pos attribute of the mouse will be initialized
 * @param mouse_y_coord - the value which y_pos attribute of the mouse will be initialized
 * @param mouse_bitmap - the bitmap that will be representing the mouse and with which the attribute bitmap will be filled
 *
 * @return Mouse* - the address of the mouse "object" that was just created
 */
Mouse* create_mouse(short mouse_x_coord , short mouse_y_coord , Bitmap* mouse_bitmap);

/**
 * @brief Deletes the Mouse "object" passed by parameter, thus freeing the memory occupied by it
 *
 * @param mouse - a Mouse pointer
 *
 * @return void
 *
 */
void delete_mouse(Mouse* mouse);

/**
 * @brief Checks if the x and y coordinates of the mouse are within the screen's boundaries and corrects them if needed
 *
 * @param mouse - a Mouse pointer
 *
 * @return void
 */
void assert_mouse_pos(Mouse* mouse);

/**
 * @brief Draws on the screen the mouse passed by parameter
 *
 * @param mouse - a Mouse pointer
 *
 * @return void
 *
 */
void draw_mouse(Mouse* mouse);

/**
 * @brief Sets the mouse's x_pos attribute to new_mouse_x_pos value passed by parameter
 *
 * @param mouse - a Mouse pointer
 * @param new_mouse_x_pos
 *
 * @return void
 *
 */
void set_mouse_x_pos(Mouse* mouse , short new_mouse_x_pos);

/**
 * @brief Sets the mouse's y_pos attribute to new_mouse_y_pos value passed by parameter
 *
 * @param mouse - a Mouse pointer
 * @param new_mouse_y_pos
 *
 * @return void
 *
 */
void set_mouse_y_pos(Mouse* mouse , short new_mouse_y_pos);

/**
 * @brief Sets the mouse's lb_status attribute to new_mouse_lb_status value passed by parameter
 *
 * @param mouse - a Mouse pointer
 * @param new_mouse_lb_status
 *
 * @return void
 *
 */
void set_mouse_lb_status(Mouse* mouse , int new_mouse_lb_status);

/**
 * @brief Returns the current value of the x_pos attribute of the mouse passed by parameter
 *
 * @param mouse - a Mouse pointer
 *
 * @return short - the current mouse's x_position
 *
 */
short get_mouse_x_pos(Mouse* mouse);

/**
 * @brief Returns the current value of the y_pos attribute of the mouse passed by parameter
 *
 * @param mouse - a Mouse pointer
 *
 * @return short - the current mouse's y_position
 *
 */
short get_mouse_y_pos(Mouse* mouse);

/**
 * @brief Returns the current value of the lb_status attribute of the mouse passed by parameter
 *
 * @param mouse - a Mouse pointer
 *
 * @return int - Returns MOUSE_LB_RELEASED(0) or MOUSE_LB_PRESSED(1)
 *
 */
int get_mouse_lb_status(Mouse* mouse);

 /** @} end of Mouse */

#endif /* _MOUSE_H_ */
