#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "Sprite.h"

#define COLOR_BLACK            0
#define VGEXIT_FAILED          1
#define CHANGE_VBEMODE_FAILED  2
#define VGINIT_FAILED          NULL

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */


/**
 * @brief Initializes the video module in graphics mode
 *
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen,
 *  and the number of colors
 *
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);



 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 *
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);


/**
 * @brief Draws a square with size 'size' with color 'color' and coordinates (x,y) relative to the center of the screen
 *
 * @param x - x coordinate relative to the center of the screen
 * @param y - y coordinate relative to the center of the screen
 * @param size - size of the square
 * @param color - color of the square
 *
 * @return (void)
 */
void vg_draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color);


/**
 * @brief Draws a line that connects the dots (xi,yi) and (xf,yf) with the color 'color'
 *
 * @param xi - coordinate of the initial X
 * @param yi - coordinate of the initial Y
 * @param xf - coordinate of the final X
 * @param yf - coordinate of the final Y
 * @param color - color of the line
 *
 * @return (void)
 */
void vg_draw_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);


/**
 * @brief Draws a sprite
 *
 * @param sp    Sprite to be drawn
 *
 * @return (void)
 */
void vg_draw_sprite(Sprite* sp);


/**
 * @brief Clears the screen by making every pixel in the screen black
 *
 * @return (void)
 */
void vg_clear_screen();


/**
 * @brief Fills a pixel with coordinates (x,y) within the range [0..HRES-1] and [0..VRES-1] with the desired color
 *
 * @param x - x coordinate
 * @param y - y coordinate
 * @param color - color to fill the pixel with
 *
 * @return (void)
 */
void vg_fill_pixel(short x, short y, unsigned long color);


/**
 * @brief Changes to video mode speficied in parameter
 *
 * @param mode  Video mode the function changes to
 *
 * @return Return OK on succes and non-zero otherwise
 */
int vg_change_vbe_mode(u_short mode);


 /** @} end of video_gr */

#endif /* __VIDEO_GR_H */
