#ifndef _SPRITE_H_
#define _SPRITE_H_

#define SPRITE_CREATION_FAILED     NULL
#define SPRITE_READ_XPM_FAILED     NULL
#define PIXMAP_EMPTY               NULL


typedef struct {
    int x;       // current x position
    int y;       // current y position
    int width;   // width dimension
    int height;  // height dimension
    int xspeed;  // speed in the X axis
    int yspeed;  // speed in the Y axis
    char *map;   // pix map
} Sprite;

/**
 * @brief Creates a new sprite with the arguments passed in as parameters, allocation memory for the new Sprite
 *
 * @param pic   Un-parsed pixmap
 * @param x Sprite's x position
 * @param y Sprite's y position
 * @param xspeed    Sprite's speed in the X axis
 * @param yspeed    Sprite's speed in the Y axis
 *
 *`@return Returns a pointer to the newly-created sprite
 */
Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed);

/**
 * @brief Destroys a sprite, freeing all the memory allocated for it
 *
 * @param sp    Pointer to the sprite to be destroyed
 *
 * @return (void)
 */
void destroy_sprite(Sprite *sp) ;


#endif  /* _SPRITE_H_ */
