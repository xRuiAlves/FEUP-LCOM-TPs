#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "video_gr.h"
#include "video.h"
#include "vbe.h"

/* Private global variables */

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static char *aux_buffer;

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


/* Function to set the screen resolution and bits_per_pixel in the current video mode */
static void vg_set_screen_res(unsigned hres , unsigned vres , unsigned bpp){
    // Set the Private Global Variables
    h_res = hres;
    v_res = vres;
    bits_per_pixel = bpp;
}

int getHorResolution() { return h_res; }

int getVerResolution() { return v_res; }

char* getGraphicsBuffer() { return video_mem; }

char* getAuxiliaryBuffer() { return aux_buffer; }

void flipBuffer(){
    memcpy(video_mem, aux_buffer , h_res * v_res * (bits_per_pixel/8) );
}


void *vg_init(unsigned short mode){

    // Change the graphics mode and check if it was successful
    if( vg_change_vbe_mode(mode) != OK ) {
        printf("video_gr::vg_init() -> vg_change_vbe_mode() failed \n");
        return VGINIT_FAILED;
    }


    // Get the mode information in order to map the memory
    vbe_mode_info_t vmode_info;
    if (vbe_get_mode_info(mode, &vmode_info) != OK){    // Check if no error occured
        printf("video_gr::vg_init() -> failed to get mode information! \n");
        return VGINIT_FAILED;
    }


    // Set the static global attributes with the correct values
    vg_set_screen_res(vmode_info.XResolution , vmode_info.YResolution , vmode_info.BitsPerPixel);


    // Get the VRAM's base physical addresss
    phys_bytes vram_base = (phys_bytes) vmode_info.PhysBasePtr;


    int r;
    struct mem_range mr;

    u_char bytes_per_pixel = bits_per_pixel / 8;        // Compute the number of bytes per pixel

    u_int vram_size = h_res * v_res * bytes_per_pixel;  // Compute the VRAM's size


    /* Allow memory mapping */

    mr.mr_base = vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
        panic("video_gr::vg_init() -> sys_privctl (ADD_MEM) failed: %d\n", r);
        return VGINIT_FAILED;
    }


    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED){
        panic("video_gr::vg_init() -> couldn't map video memory\n");
        return VGINIT_FAILED;
    }


    /* Allocate memory for the auxiliary buffer */
    aux_buffer = (char*) malloc(vram_size);

    if(aux_buffer == NULL){
        return NULL;
    }

    // Return the mapped VRAM base address
    return video_mem;
}



int vg_exit() {
    // Free the allocted memory for the aux buffer
    if (aux_buffer != NULL)
        free(aux_buffer);


    // Return to default text mode
    struct reg86u reg86;

    reg86.u.b.intno = BIOS_SERVICE_VIDEOCARD;     /* BIOS video services */

    reg86.u.b.ah = BIOS_FUNCTION_SETVIDEOMODE;    /* Set Video Mode function */
    reg86.u.b.al = BIOS_VMODE_TEXT_25x80_16C;     /* 80x25 text mode*/

    if( sys_int86(&reg86) != OK ) {
        printf("\tvg_exit(): sys_int86() failed \n");
        return VGEXIT_FAILED;
    } else
        return OK;
}



int vg_change_vbe_mode(u_short mode){
    struct reg86u reg86;

    reg86.u.b.intno = BIOS_SERVICE_VIDEOCARD;       // 0x10
    reg86.u.b.ah = BIOS_VBE_SERVICE;                // 0x4F
    reg86.u.b.al = VBE_FUNCTION_SET_VBE_MODE;       // 0x02, VBE call, function 2
    reg86.u.w.bx = mode | VBE_SET_LINEAR_MODE;      // BX = Mode + bit 14, which initializes the buffer in linear mode

    // Make sys_int86() call and return success / failure
    if (sys_int86(&reg86) != OK)
        return CHANGE_VBEMODE_FAILED;

    // Check if there was success in executing the function
    if (reg86.u.b.ah != VBE_FUNCTION_SUCCESS){
        printf("video_gr::vg_init() -> function 'change vbe mode' was not successful! \n");
        return CHANGE_VBEMODE_FAILED;
    }

    // All was successfull
    return OK;
}
