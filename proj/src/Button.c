#include "Button.h"
#include "stdlib.h"
#include "video_gr.h"


///////////////////////////////////////
///////////////////////////////////////
/*    Button Class Implementation    */
///////////////////////////////////////
///////////////////////////////////////


///////////////////////////////////
//    Attributes  ::  Private    //
///////////////////////////////////


struct Button_Class{
    // Button's Position
    short x_pos;
    short y_pos;

    // Button's Graphics
    Bitmap* default_bitmap;
    Bitmap* hovered_bitmap;

    // Button's Attributes
    short width;
    short height;

    // Button's status
    int status;
};


///////////////////////////////
//    Methods  ::  Public    //
///////////////////////////////


Button* create_button(short x_pos , short y_pos , Bitmap* default_bmp , Bitmap* hovered_bmp){

    // Allocate space for the "Object"
    Button *button = (Button *) malloc ( sizeof(Button) );

    // Check if the mem. allocation was successful
    if( button == NULL )
        return BUTTON_ALLOC_FAILED;

    // Initialize the various button's Attributes
    button->x_pos = x_pos;
    button->y_pos = y_pos;
    button->default_bitmap = default_bmp;
    button->hovered_bitmap = hovered_bmp;
    button->status = BUTTON_DEFAULT_STATUS;
    button->width = default_bmp->bitmapInfoHeader.width;
    button->height = default_bmp->bitmapInfoHeader.height;

    // Return the newly created button's address
    return button;

}


void delete_button(Button* button){

    // Check if the button is existant
    if ( button == NULL )
        return;

    // Free the allocated memory for the bitmaps
    deleteBitmap(button->default_bitmap);
    deleteBitmap(button->hovered_bitmap);

    // Free the memory allocated for the button
    free(button);

    // The button is now un-existant
    button = NULL;

}


void draw_button(Button* button){
    /* Draw the button, deppending on its status */
    if(button->status == BUTTON_DEFAULT_STATUS)
        drawBitmap(button->default_bitmap , button->x_pos , button->y_pos , getAuxiliaryBuffer());
    else
        drawBitmap(button->hovered_bitmap , button->x_pos , button->y_pos , getAuxiliaryBuffer());
}


int get_button_status(Button* button){
    return button->status;
}


short get_button_height(Button* button){
    return button->height;
}


short get_button_width(Button* button){
    return button->width;
}


void set_button_status(Button* button , int new_status){
    button->status = new_status;
}


int check_button_mouse_collision(Button* button , Mouse* mouse){
    // Check if the mouse (after button clicking) is colliding with the button
    short mouse_asserted_x_pos = get_mouse_x_pos(mouse) + (MOUSE_BITMAP_SIZE/2);
    short mouse_asserted_y_pos = get_mouse_y_pos(mouse) + (MOUSE_BITMAP_SIZE/2);

    if(mouse_asserted_x_pos>=button->x_pos && mouse_asserted_x_pos<=button->x_pos+button->width && mouse_asserted_y_pos>=button->y_pos && mouse_asserted_y_pos<=button->y_pos+button->height)
        return COLLISION_FOUND;
    else
        return NO_COLLISION_FOUND;
}
