#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "utilities.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

#define COLOR_BLACK                0
#define VGEXIT_FAILED              1
#define CHANGE_VBEMODE_FAILED      2
#define AUX_BUFFER_MALLOC_FAILED   3
#define VGINIT_FAILED              NULL


/**
 * @brief Returns the horizontal resolution of the screen
 *
 * @return int - horizontal resolution
 */
int getHorResolution();

/**
 * @brief Returns the vertical resolution of the screen
 *
 * @return int - vertical resolution
 */
int getVerResolution();

/**
 * @brief Returns the base address of the graphics buffer
 *
 * @return char* - graphics buffer
 */
char* getGraphicsBuffer();

/**
 * @brief Returns the base address of the auxiliary buffer (back buffer)
 *
 * @return char* - auxiliary graphics buffer
 */
char* getAuxiliaryBuffer();

/**
 * @brief Switches the graphics buffer and the auxiliary (back) buffer
 *
 * @return void
 */
void flipBuffer();

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
 * @brief Changes to video mode speficied in parameter
 *
 * @param mode  Video mode the function changes to
 *
 * @return Return OK on succes and non-zero otherwise
 */
int vg_change_vbe_mode(u_short mode);


 /** @} end of video_gr */

#endif /* __VIDEO_GR_H */
