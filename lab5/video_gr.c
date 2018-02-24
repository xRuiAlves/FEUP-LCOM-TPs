#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>

#include "utilities.h"
#include "video_test.h"
#include "video_gr.h"
#include "video.h"
#include "vbe.h"

/* Private global variables */

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

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


    // Return the mapped VRAM base address
    return video_mem;
}



int vg_exit() {
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



void vg_draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color){
    // Compute the center screen coordinates
    u_short screen_center_x = h_res / 2;
    u_short screen_center_y = v_res / 2;

    /* Compute the square's border coordinates - when size is even, the center of the screen is between pixels ; when it's odd, it's
       a regular pixel with coordinates (h_res/2 , v_res/2) */
    short x_left = screen_center_x + x - size/2;
    short x_right = screen_center_x + x + size/2 - 1 + size%2;
    short y_bottom = screen_center_y + y + size/2 - 1 + size%2;
    short y_top = screen_center_y + y - size/2;

    /* The coordinates x_left , x_right , y_bottom and y_top are 'short' and not 'unsigned short' so that we can evaluate which
       pixels are out of the screen. */

    // Draw the square
    short i, j;
    for (i=x_left ; i<=x_right ; i++){
        for (j=y_top ; j<=y_bottom ; j++){
            vg_fill_pixel(i,j,color);
        }
    }

}



void vg_draw_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color){

    double slope;

    // Compute the caracteristics of the line - Using doubles so the arithmetics are all fine
    double dx = xf - xi;
    double dy = yf - yi;

    u_short x; // For slope lower than 45 degrees
    u_short y; // For slope higher than 45 degrees

    // Slope lower or equal than 45 degrees
    if(absolute_value(dy) >= absolute_value(dx)){
        if(yi > yf){        // Swap the points order if yi > yf, to make sure y is growing
            swap_values(u_short, xi, xf);
            swap_values(u_short, yi, yf);
            dx = xf - xi;   // Update dx and dy
            dy = yf - yi;
        }

        if (dx == 0){      // In case of vertical line
            for(y = yi ; y <= yf ; y++){
                vg_fill_pixel(xi, y, color);
            }
        }
        else {             // Regular line
            slope = dy/dx;
            for(y = yi ; y <= yf ; y++){
                x = (y-yi)/slope + xi;      // Calculate the X using basic line's equation
                vg_fill_pixel(x, y, color);
            }
        }
    }

    // Slope higher than 45 degrees
    else{
        if(xi > xf){        // Swap the points order if xi > xf, to make sure x is growing
            swap_values(u_short, xi, xf);
            swap_values(u_short, yi, yf);
            dx = xf - xi;   // Update dx and dy
            dy = yf - yi;
        }

        if (dy == 0){      // In case of horizontal line
            for(x = xi ; x <= xf ; x++){
                vg_fill_pixel(x, yi, color);
            }
        }
        else{              // Regular line
            slope = dx/dy;
            for(x = xi ; x <= xf ; x++){
                y = (x-xi)/slope + yi;      // Calculate the Y using basic line's equation
                vg_fill_pixel(x, y, color);
            }
        }
    }
}


void vg_draw_sprite(Sprite* sp){
    int x, y;
    unsigned int printed_pixel_counter = 0;

    // Draw the sprite
    for (y = sp->y ; y < (sp->y + sp->height) ; y++){
        for (x = sp->x ; x < (sp->x + sp->width) ; x++){
            vg_fill_pixel(x , y , sp->map[printed_pixel_counter]);
            printed_pixel_counter++;
        }
    }
}


void vg_clear_screen(){
    int i;
    // Clear the screen by drawing 'black' in every pixel
    for (i=0 ; i<h_res*v_res ; i++){
        *(video_mem+i) = 0;
    }
}


void vg_fill_pixel(short x, short y, unsigned long color){
    /* Fill the pixel in the (x,y) coordinates , but only if both X and Y coordinates are within
       the [0..h_res-1] and [0..v_res-1] range */
    if ( (x<0) || (y<0) || (x>=h_res) || (y>=v_res))
        return;

    // Compute the no. of bytes per pixel
    u_char bytes_per_pixel = bits_per_pixel / 8;

    // Draw the pixel with the desired color
    if (bytes_per_pixel == 1)
        *(video_mem + (y*h_res*bytes_per_pixel) + (x*bytes_per_pixel) ) = (unsigned char) color;
    else
        *(video_mem + (y*h_res*bytes_per_pixel) + (x*bytes_per_pixel) ) = color;
}
