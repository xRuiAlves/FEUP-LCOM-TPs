#ifndef _FONT_H_
#define _FONT_H_

#include "Bitmap.h"
#include "utilities.h"

/** @defgroup Font Font
 * @{
 * Functions for manipulating font in graphics mode
 */

#define DIGIT_BMP_SIZE              16
#define DIGIT_SEPARATION_SIZE       2
#define BIG_DIGIT_SIZE              46
#define BIG_DIGIT_SEPARATION_SIZE   6
#define SMALL                       0
#define BIG                         1

#define BITMAP_0_PATH "/home/SnakeyNix/zero.bmp"
#define BITMAP_1_PATH "/home/SnakeyNix/one.bmp"
#define BITMAP_2_PATH "/home/SnakeyNix/two.bmp"
#define BITMAP_3_PATH "/home/SnakeyNix/three.bmp"
#define BITMAP_4_PATH "/home/SnakeyNix/four.bmp"
#define BITMAP_5_PATH "/home/SnakeyNix/five.bmp"
#define BITMAP_6_PATH "/home/SnakeyNix/six.bmp"
#define BITMAP_7_PATH "/home/SnakeyNix/seven.bmp"
#define BITMAP_8_PATH "/home/SnakeyNix/eight.bmp"
#define BITMAP_9_PATH "/home/SnakeyNix/nine.bmp"
#define BITMAP_SPACE_PATH "/home/SnakeyNix/space.bmp"
#define BITMAP_TWO_DOTS_PATH "/home/SnakeyNix/twoDots.bmp"
#define BITMAP_BAR_PATH "/home/SnakeyNix/bar.bmp"

#define BITMAP_BIG_0_PATH "/home/SnakeyNix/bigZero.bmp"
#define BITMAP_BIG_1_PATH "/home/SnakeyNix/bigOne.bmp"
#define BITMAP_BIG_2_PATH "/home/SnakeyNix/bigTwo.bmp"
#define BITMAP_BIG_3_PATH "/home/SnakeyNix/bigThree.bmp"
#define BITMAP_BIG_4_PATH "/home/SnakeyNix/bigFour.bmp"
#define BITMAP_BIG_5_PATH "/home/SnakeyNix/bigFive.bmp"
#define BITMAP_BIG_6_PATH "/home/SnakeyNix/bigSix.bmp"
#define BITMAP_BIG_7_PATH "/home/SnakeyNix/bigSeven.bmp"
#define BITMAP_BIG_8_PATH "/home/SnakeyNix/bigEight.bmp"
#define BITMAP_BIG_9_PATH "/home/SnakeyNix/bigNine.bmp"

/**
 * @brief Draws on the buffer "buffer" in the position (x_pos, y_pos) the digit "digit_to_draw" of size "sizeOfDigit"
 *
 * @param digit_to_draw
 * @param x_pos
 * @param y_pos
 * @param sizeOfDigit - there are big digit bitmaps and small digit bitmaps, this parameter distinguishes them
 * @param buffer - the buffer in which to draw
 *
 * @return void
 *
 */
void draw_digit(char digit_to_draw , short x_pos , short y_pos, u_char sizeOfDigit , char* buffer);

/**
 * @brief Draws on the buffer "buffer" in the position (x_pos, y_pos) the "string" passed by parameter
 *
 * @param string - the string to draw
 * @param string_size - the size of the string to draw
 * @param x_pos
 * @param y_pos
 * @param sizeOfFont - there are big digit bitmaps and small digit bitmaps, this parameter distinguishes them
 * @param buffer - the buffer in which to draw
 *
 * @return void
 *
 */
void draw_string(char string[] , int string_size , short x_pos , short y_pos, u_char sizeOfFont , char* buffer);

/**
 * @brief Loads the bitmaps for all the characters and stores them in the global static variables in the file
 *
 * @return void
 *
 */
void initialize_font();

/**
 * @brief Deletes all the previously loaded bitmaps, thus freeing the memory space occupied by them
 *
 * @return void
 *
 */
void terminate_font();

 /** @} end of Font */

#endif /* _FONT_H_ */
