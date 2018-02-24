#include "Mouse.h"
#include "stdlib.h"
#include "i8042.h"
#include "video_gr.h"


//////////////////////////////////////
//////////////////////////////////////
/*    Mouse Class Implementation    */
//////////////////////////////////////
//////////////////////////////////////




///////////////////////////////////
//    Attributes  ::  Private    //
///////////////////////////////////


struct Mouse_Class{
    // Mouse's Position
    short x_pos;
    short y_pos;

    // Mouse's Status
    int lb_status;

    // Mouse's Graphics
    Bitmap* bitmap;
};




///////////////////////////////
//    Methods  ::  Public    //
///////////////////////////////


Mouse* create_mouse(short mouse_x_pos , short mouse_y_pos , Bitmap* mouse_bitmap){

    // Allocate space for the "Object"
    Mouse *mouse = (Mouse *) malloc ( sizeof(Mouse) );

    // Check if the mem. allocation was successful
    if( mouse == NULL )
        return MOUSE_ALLOC_FAILED;

    // Initialize the various mouse Attributes
    mouse->x_pos = mouse_x_pos;
    mouse->y_pos = mouse_y_pos;
    mouse->bitmap = mouse_bitmap;
    mouse->lb_status = MOUSE_LB_IS_NOT_PRESSED;

    // Return the newly created mouse's address
    return mouse;

}


void delete_mouse(Mouse* mouse){

    // Check if the mouse is existant
    if ( mouse == NULL )
        return;

    // Free the bitmap's memory
    deleteBitmap(mouse->bitmap);

    // Free the memory allocated for the mouse
    free(mouse);

    // The mouse is now un-existant
    mouse = NULL;

}


void draw_mouse(Mouse* mouse){
    drawBitmap(mouse->bitmap , mouse->x_pos , mouse->y_pos , getAuxiliaryBuffer());
}


void set_mouse_x_pos(Mouse* mouse , short new_mouse_x_pos){
    mouse->x_pos = new_mouse_x_pos;
}


void set_mouse_y_pos(Mouse* mouse , short new_mouse_y_pos){
    mouse->y_pos = new_mouse_y_pos;
}


void set_mouse_lb_status(Mouse* mouse , int new_mouse_lb_status){
    mouse->lb_status = new_mouse_lb_status;
}


short get_mouse_x_pos(Mouse* mouse){
    return (mouse->x_pos);
}


short get_mouse_y_pos(Mouse* mouse){
    return (mouse->y_pos);
}


int get_mouse_lb_status(Mouse* mouse){
    return mouse->lb_status;
}


void assert_mouse_pos(Mouse* mouse){
    // Make sure the mouse stays inside the windows borders
    if (mouse->x_pos < 0)
        mouse->x_pos = 0;
    else if (mouse->x_pos >= getHorResolution()-MOUSE_BITMAP_SIZE)
        mouse->x_pos = getHorResolution()-MOUSE_BITMAP_SIZE;

    if (mouse->y_pos < 0)
        mouse->y_pos = 0;
    else if (mouse->y_pos >= getVerResolution()-MOUSE_BITMAP_SIZE)
        mouse->y_pos = getVerResolution()-MOUSE_BITMAP_SIZE;
}
