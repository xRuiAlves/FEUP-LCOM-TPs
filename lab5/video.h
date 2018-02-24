#ifndef _VIDEO_H_
#define _VIDEO_H_


#define BIT(n)    (0x01<<(n))


/////////////////////
//  BIOS SERVICES  //
/////////////////////

#define BIOS_SERVICE_VIDEOCARD        0x10        //  BIOS video card service interrupt vector
#define BIOS_SERVICE_PC_CONFIG        0x11        //  BIOS pc configuration service interrupt vector
#define BIOS_SERVICE_MEM_CONFIG       0x12        //  BIOS memory configuration service interrupt vector
#define BIOS_SERVICE_KEYBOARD         0x16        //  BIOS keyboard service interrupt vector


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  BIOS FUNCTIONS    |    To be written in the AH register ; Arguments, if existant, are written in the AL register  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BIOS_FUNCTION_SETVIDEOMODE    0x00        //  Set the BIOS video Mode , takes the mode as argument
#define BIOS_VBE_SERVICE              0x4F        //  To distinguish Video BIOS Extension functions from basic video BIOS services


////////////////////////////////////////////////////////////////////////
//  BIOS FUNCTION ARGUMENTS    |    To be written in the AL register  //
////////////////////////////////////////////////////////////////////////

#define BIOS_VMODE_TEXT_25x80_16C     0x03        //  Text mode with 25 lines x 80 columns , 16 color


//////////////////////////////////////////////////////////////
//  VBE FUNCTIONS    |    To be written in the AL register  //
//////////////////////////////////////////////////////////////

#define VBE_FUNCTION_RETURN_CTRL_INFO   0x00        //  VBE function to return the controller information , on the AX register
#define VBE_FUNCTION_RETURN_MODE_INFO   0x01        //  VBE function to return the mode information , on the AX register
#define VBE_FUNCTION_SET_VBE_MODE       0x02        //  VBE function to set VBE mode


//////////////////////////////////////////////////////////////////////////////////////////////////////
//  VBE GRAPHICS MODES    |    To be written in different registers, depending on the VBE function  //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define VBE_MODE_640x480_256C       0x101      //  VBE Graphics mode with 640 x 480 pixels and 256 colors ; To be written in the BX register
#define VBE_MODE_800x600_256C       0x103      //  VBE Graphics mode with 800 x 600 pixels and 256 colors ; To be written in the BX register
#define VBE_MODE_1024x768_256C      0x105      //  VBE Graphics mode with 1024 x 765 pixels and 256 colors ; To be written in the BX register
#define VBE_MODE_1280x1024_256C     0x107      //  VBE Graphics mode with 1280 x 1024 pixels and 256 colors ; To be written in the BX register
#define VBE_SET_LINEAR_MODE         BIT(14)     //  To be used along a VBE mode , to set linear framebuffer  (Example: 0x0105 | BIT(14) sets 1024x768@256, LINEAR mode)


///////////////////////////////////////////////////////////////////////////////////////////////////
//  VBE FUNCTIONS RETURN    |    To be read from the AX register after executing a VBE function  //
///////////////////////////////////////////////////////////////////////////////////////////////////

#define VBE_FUNCTION_SUPPORTED     0x4F        //  Value returned to the AL register, if the executed function is supported
#define VBE_FUNCTION_SUCCESS       0x00        //  Value returned to the AH register, if the function was completed successfully
#define VBE_FUNCTION_FAILED        0x01        //  Value returned to the AH register, if the function call failed
#define VBE_FUNCTION_NOTSUP_HW     0x02        //  Value returned to the AH register, if the function is not supported in the current HW configuration
#define VBE_FUNCTION_INVALID_VM    0x03        //  Value returned to the AH register, if the function is invalid in the current video mode


///////////////////////////////////////////////////
//  VBE SPECIFICATIONS FOR EACH PARTICULAR MODE  //
///////////////////////////////////////////////////

/* 0x101  :  640x480 @ 256 colors Mode */
#define VBE_MODE_0X101_HRES         640              //  Horizontal resolution
#define VBE_MODE_0X101_VRES         480              //  Vertical resolution
#define VBE_MODE_0X101_PHYS_ADDR    0xE0000000       //  VRAM's base physical address for this mode
#define VBE_MODE_0X101_BPP          8                //  Bits per pixel

/* 0x103  :  800x600 @ 256 colors Mode */
#define VBE_MODE_0X103_HRES         800              //  Horizontal resolution
#define VBE_MODE_0X103_VRES         600              //  Vertical resolution
#define VBE_MODE_0X103_PHYS_ADDR    0xE0000000       //  VRAM's base physical address for this mode
#define VBE_MODE_0X103_BPP          8                //  Bits per pixel

/* 0x105  :  1024x768 @ 256 colors Mode */
#define VBE_MODE_0X105_HRES         1024             //  Horizontal resolution
#define VBE_MODE_0X105_VRES         768              //  Vertical resolution
#define VBE_MODE_0X105_PHYS_ADDR    0xE0000000       //  VRAM's base physical address for this mode
#define VBE_MODE_0X105_BPP          8                //  Bits per pixel

/* 0x107  :  1280x1024 @ 256 colors Mode */
#define VBE_MODE_0X107_HRES         1280             //  Horizontal resolution
#define VBE_MODE_0X107_VRES         1024             //  Vertical resolution
#define VBE_MODE_0X107_PHYS_ADDR    0xE0000000       //  VRAM's base physical address for this mode
#define VBE_MODE_0X107_BPP          8                //  Bits per pixel




#endif  /* _VIDEO_H_ */
