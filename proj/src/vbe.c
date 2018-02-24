#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "video.h"
#include "vbe.h"
#include "lmlib.h"
#include "utilities.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)
#define SEGMENT_WORD(x) (((x) >> 16) & 0x0FFFF)
#define OFFSET_WORD(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

    // Initialize the required variables and stuctures
    void *virtual_address;
    struct reg86u reg86;
    mmap_t mm;


    // Initialize the low memory area and check for success
    virtual_address = lm_init();
    if (virtual_address == NULL){
        return VBE_LMINIT_FAILED;
    }


    // Allocate the low memory block
    virtual_address = lm_alloc( VBE_MODE_INFO_BLOCK_SIZE , &mm);     // Size of allocate corresponds to the the size of the vbe_mode_info_t struct
    if (virtual_address == NULL){
        return VBE_LMALLOC_FAILED;
    }


    // Get the mode information
    phys_bytes buf = mm.phys;

    reg86.u.b.intno = BIOS_SERVICE_VIDEOCARD;       // 0x10
    reg86.u.b.ah = BIOS_VBE_SERVICE;                // 0x4F
    reg86.u.b.al = VBE_FUNCTION_RETURN_MODE_INFO;   // 0x01, VBE call, function 1
    reg86.u.w.es = PB2BASE(buf);                    // set a segment base
    reg86.u.w.di = PB2OFF(buf);                     // set the offset accordingly
    reg86.u.w.cx = mode;                            // mode to retreive the info. from

    if (sys_int86(&reg86) != OK){
        return VBE_SYSINT86_FAILED;
    }

    // Check if there was success in executing the function
    if (reg86.u.b.ah != VBE_FUNCTION_SUCCESS){
        printf("vbe::vbe_get_mode_info() -> function 'get mode info' was not successful! \n");
        return GET_MODE_INFO_FAILED;
    }


    // Put the memory buffer with the function returned data in vmi_p
    *vmi_p = *(vbe_mode_info_t*) mm.virtual;        // Cast the 'virtual' struct field into a vbe_mode_info_t pointer and assign it's content to vbe_mode_info_t pointer passed in as param.


    // Free the allocated memory
    lm_free(&mm);


    // The function was successful
    return OK;
}
