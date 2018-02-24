#ifndef _VIDEO_H_
#define _VIDEO_H_

/** @defgroup video video
 * @{
 *
 * Constants for programming the VBE
 */

#define BIT(n)    (0x01<<(n))


/////////////////////
//  BIOS SERVICES  //
/////////////////////

#define BIOS_SERVICE_VIDEOCARD        0x10        /**< @brief BIOS video card service interrupt vector */
#define BIOS_SERVICE_PC_CONFIG        0x11        /**< @brief BIOS pc configuration service interrupt vector */
#define BIOS_SERVICE_MEM_CONFIG       0x12        /**< @brief BIOS memory configuration service interrupt vector */
#define BIOS_SERVICE_KEYBOARD         0x16        /**< @brief BIOS keyboard service interrupt vector */


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  BIOS FUNCTIONS    |    To be written in the AH register ; Arguments, if existant, are written in the AL register  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BIOS_FUNCTION_SETVIDEOMODE    0x00        /**< @brief Set the BIOS video Mode , takes the mode as argument */
#define BIOS_VBE_SERVICE              0x4F        /**< @brief To distinguish Video BIOS Extension functions from basic video BIOS services */


////////////////////////////////////////////////////////////////////////
//  BIOS FUNCTION ARGUMENTS    |    To be written in the AL register  //
////////////////////////////////////////////////////////////////////////

#define BIOS_VMODE_TEXT_25x80_16C     0x03        /**< @brief Text mode with 25 lines x 80 columns , 16 color */


//////////////////////////////////////////////////////////////
//  VBE FUNCTIONS    |    To be written in the AL register  //
//////////////////////////////////////////////////////////////

#define VBE_FUNCTION_RETURN_CTRL_INFO   0x00        /**< @brief VBE function to return the controller information , on the AX register */
#define VBE_FUNCTION_RETURN_MODE_INFO   0x01        /**< @brief VBE function to return the mode information , on the AX register */
#define VBE_FUNCTION_SET_VBE_MODE       0x02        /**< @brief VBE function to set VBE mode */


//////////////////////////////////////////////////////////////////////////////////////////////////////
//  VBE GRAPHICS MODES    |    To be written in different registers, depending on the VBE function  //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define VBE_MODE_640x480_256C       0x101      /**< @brief VBE Graphics mode with 640 x 480 pixels and 256 colors ; To be written in the BX register */
#define VBE_MODE_800x600_256C       0x103      /**< @brief VBE Graphics mode with 800 x 600 pixels and 256 colors ; To be written in the BX register */
#define VBE_MODE_1024x768_256C      0x105      /**< @brief VBE Graphics mode with 1024 x 765 pixels and 256 colors ; To be written in the BX register */
#define VBE_MODE_1280x1024_256C     0x107      /**< @brief VBE Graphics mode with 1280 x 1024 pixels and 256 colors ; To be written in the BX register */
#define VBE_MODE_640x480_RGB565     0x111      /**< @brief VBE Graphics mode with 640 x 480 pixels and 65536 colors (RGB 5-6-5) ; To be written in the BX register */
#define VBE_MODE_800x600_RGB565     0x114      /**< @brief VBE Graphics mode with 800 x 600 pixels and 65536 colors (RGB 5-6-5) ; To be written in the BX register */
#define VBE_MODE_1024x768_RGB565    0x117      /**< @brief VBE Graphics mode with 1024 x 765 pixels and 65536 colors (RGB 5-6-5) ; To be written in the BX register */
#define VBE_MODE_1280x1024_RGB565   0x11A      /**< @brief VBE Graphics mode with 1280 x 1024 pixels and 65536 colors (RGB 5-6-5) ; To be written in the BX register */
#define VBE_SET_LINEAR_MODE         BIT(14)     /**< @brief To be used along a VBE mode , to set linear framebuffer  (Example: 0x0105 | BIT(14) sets 1024x768@256, LINEAR mode) */


///////////////////////////////////////////////////////////////////////////////////////////////////
//  VBE FUNCTIONS RETURN    |    To be read from the AX register after executing a VBE function  //
///////////////////////////////////////////////////////////////////////////////////////////////////

#define VBE_FUNCTION_SUPPORTED     0x4F        /**< @brief Value returned to the AL register, if the executed function is supported */
#define VBE_FUNCTION_SUCCESS       0x00        /**< @brief Value returned to the AH register, if the function was completed successfully */
#define VBE_FUNCTION_FAILED        0x01        /**< @brief Value returned to the AH register, if the function call failed */
#define VBE_FUNCTION_NOTSUP_HW     0x02        /**< @brief Value returned to the AH register, if the function is not supported in the current HW configuration */
#define VBE_FUNCTION_INVALID_VM    0x03        /**< @brief Value returned to the AH register, if the function is invalid in the current video mode */


 /** @} end of video */

#endif  /* _VIDEO_H_ */
