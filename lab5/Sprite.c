#include "Sprite.h"
#include "stdlib.h"
#include "read_xpm.h"

Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed) {
    // Allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));

    // Check if the allocation was succesfull
    if( sp == NULL )
        return SPRITE_CREATION_FAILED;

    // Check if the pixmap isn't empty
    if (pic == NULL)
        return PIXMAP_EMPTY;

    // Read the sprite pixmap
    sp->map = read_xpm(pic, &(sp->width), &(sp->height));
    if( sp->map == NULL ) {
        free(sp);
        return SPRITE_READ_XPM_FAILED;
    }

    // Set the sprite's attributes
    sp->x = x;
    sp->y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;

    return sp;
}


void destroy_sprite(Sprite *sp) {
    // Check if the sprite isn't already destroyed
    if( sp == NULL )
        return;

    // Free the pix map associated to the sprite, if it exists
    if( sp ->map )
        free(sp->map);

    // Free the memory allocated for the sprite
    free(sp);

    // The sprite is now empty
    sp = NULL;
}
