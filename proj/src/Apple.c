#include "Apple.h"
#include "stdlib.h"
#include "video_gr.h"


//////////////////////////////////////
//////////////////////////////////////
/*    Apple Class Implementation    */
//////////////////////////////////////
//////////////////////////////////////




///////////////////////////////////
//    Attributes  ::  Private    //
///////////////////////////////////


struct Apple_Class{
    // Apple's Position
    short x_pos;
    short y_pos;

    // Apple's Graphics
    Bitmap* bitmap;

    // Apple's status
    int is_active;
};




///////////////////////////////
//    Methods  ::  Public    //
///////////////////////////////


Apple* create_apple(short x_pos , short y_pos , Bitmap* apple_bmp){

    // Allocate space for the "Object"
    Apple *apple = (Apple *) malloc ( sizeof(Apple) );

    // Check if the mem. allocation was successful
    if( apple == NULL )
        return APPLE_ALLOC_FAILED;

    // Initialize the various apple's Attributes
    apple->x_pos = x_pos;
    apple->y_pos = y_pos;
    apple->bitmap = apple_bmp;
    apple->is_active = 1;

    // Return the newly created apple's address
    return apple;

}


void delete_apple(Apple* apple){

    // Check if the apple is existant
    if ( apple == NULL )
        return;

    // Free the allocated memory for the bitmap
    deleteBitmap(apple->bitmap);

    // Free the memory allocated for the apple
    free(apple);

    // The apple is now un-existant
    apple = NULL;

}


void draw_apple(Apple* apple){
    /* The apple is only drawn when it is active , that is , when it has a position that was given by a mouse click! */
    if(apple->is_active)
        drawBitmap(apple->bitmap , apple->x_pos , apple->y_pos , getAuxiliaryBuffer());
}


short get_apple_x_pos(Apple* apple){
    return apple->x_pos;
}


short get_apple_y_pos(Apple* apple){
    return apple->y_pos;
}


int get_apple_status(Apple* apple){
    return apple->is_active;
}


void set_apple_x_pos(Apple* apple , short x_pos){
    apple->x_pos = x_pos;
}


void set_apple_y_pos(Apple* apple , short y_pos){
    apple->y_pos = y_pos;
}


void set_apple_status(Apple* apple , int new_status){
    apple->is_active = new_status;
}

void move_apple_right(Apple* apple){
    if (apple->x_pos + APPLE_SIZE < getHorResolution())
        apple->x_pos += APPLE_SIZE;
}

void move_apple_left(Apple* apple){
    if (apple->x_pos - APPLE_SIZE >= 0)
        apple->x_pos -= APPLE_SIZE;
}

void move_apple_down(Apple* apple){
    if (apple->y_pos + APPLE_SIZE < getVerResolution())
        apple->y_pos += APPLE_SIZE;
}

void move_apple_up(Apple* apple){
    if (apple->y_pos - APPLE_SIZE >= 0)
        apple->y_pos -= APPLE_SIZE;
}
