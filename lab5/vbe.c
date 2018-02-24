#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "video.h"
#include "vbe.h"
#include "lmlib.h"
#include "utilities.h"
#include "video_test.h"

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


int vbe_get_ctrl_info(vbe_ctrl_info_t *vci_p){
    // Initialize the required variables and stuctures
    struct reg86u reg86;
    mmap_t mm;
    void *virtual_address;


    // Initialize the low memory area and check for success
    virtual_address = lm_init();
    if (virtual_address == NULL){
        return VBE_LMINIT_FAILED;
    }


    // Allocate the low memory block
    virtual_address = lm_alloc( VBE_CTRL_INFO_BLOCK_SIZE , &mm);     // Size of allocate corresponds to the the size of the vbe_ctrl_info_t struct
    if (virtual_address == NULL){
        return VBE_LMALLOC_FAILED;
    }


    // Set the 'VESA VBE signature - 'VBE2'
    vci_p->VbeSignature[0] = 'V';
    vci_p->VbeSignature[1] = 'B';
    vci_p->VbeSignature[2] = 'E';
    vci_p->VbeSignature[3] = '2';


    // Get the controller information
    phys_bytes buf = mm.phys;

    reg86.u.b.intno = BIOS_SERVICE_VIDEOCARD;       // 0x10
    reg86.u.b.ah = BIOS_VBE_SERVICE;                // 0x4F
    reg86.u.b.al = VBE_FUNCTION_RETURN_CTRL_INFO;   // 0x00, VBE call, function 0
    reg86.u.w.es = PB2BASE(buf);                    // set a segment base
    reg86.u.w.di = PB2OFF(buf);                     // set the offset accordingly

    if (sys_int86(&reg86) != OK){
        return VBE_SYSINT86_FAILED;
    }

    // Check if there was success in executing the function
    if (reg86.u.b.ah != VBE_FUNCTION_SUCCESS){
        printf("vbe::vbe_get_ctrl_info() -> function 'get controller info' was not successful! \n");
        return GET_CTRL_INFO_FAILED;
    }

    // Put the memory buffer with the function returned data in vci_p
    *vci_p = *(vbe_ctrl_info_t*) mm.virtual;        // Cast the 'virtual' struct field into a vbe_ctrl_info_t pointer and assign it's content to vbe_ctrl_info_t pointer passed in as param.


    // Free the allocated memory
    lm_free(&mm);


    // The function was successful
    return OK;
}

void vbe_print_ctrl_info(vbe_ctrl_info_t *vci_p){
    // Interprete the vbe_ctrl_info structure
	u_short vbe_version = vci_p->VbeVersion;
	u_int vram_mem_size = vci_p->TotalMemory * 64;     // Number of 64kb memory blocks * 64 = Memory in KB
	phys_bytes video_modes_pb = vci_p->VideoModePtr;
    void* base_memory_ptr = lm_init();  // Get the base virtual memory address


	// Print the VBE version
	u_char major_version_byte = VBE_MAJOR_VERSION(vbe_version);
    u_char minor_version_byte = VBE_MINOR_VERSION(vbe_version);
    printf("%x.%x\n", major_version_byte , minor_version_byte);


	// Print all the video modes, until termination word is found (0xFFFF)
    u_short segment_word = SEGMENT_WORD(video_modes_pb);    // Compute segment and offset words
    u_short offset_word  = OFFSET_WORD(video_modes_pb);

    u_short* video_modes_ptr = (u_short*) (base_memory_ptr + (segment_word << 4) + offset_word);    /* Compute the video modes list first element address
                                                                                                       , which is equal to the virtual memory base adress + (segment word * 16 + offset) */
    while ( (*video_modes_ptr) != VIDEO_MODES_TERMINATION_WORD){
        printf("0x%X:",(*video_modes_ptr));
        video_modes_ptr++;
    }


    // Print the size of VRAM memory *IN KB*
    printf("\n%u\n", vram_mem_size);
}
