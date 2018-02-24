#include "Snake.h"
#include "stdlib.h"
#include "stdio.h"
#include "video_gr.h"

///////////////////////////////////////////
///////////////////////////////////////////
/*    Snake Node Class Implementation    */
///////////////////////////////////////////
///////////////////////////////////////////

#define SNAKE_NODE_ALLOC_FAILED   NULL

///////////////////////////////////
//    Attributes  ::  Private    //
///////////////////////////////////z

typedef struct Snake_Node_t{
    // Node's Position
    short x_pos;
    short y_pos;

    // Nodes after and before (doubly linked list)
    struct Snake_Node_t* snake_node_after;
    struct Snake_Node_t* snake_node_before;
} Snake_Node;


///////////////////////////////
//    Methods  ::  Public    //
///////////////////////////////

Snake_Node* create_snake_node(short x_pos , short y_pos , Snake_Node* snake_node_after , Snake_Node* snake_node_before){

    // Allocate space for the "Object"
    Snake_Node* node = (Snake_Node *) malloc ( sizeof(Snake_Node) );

    // Check if the mem. allocation was successful
    if( node == NULL )
        return SNAKE_NODE_ALLOC_FAILED;

    // Initialize the various node's Attributes
    node->x_pos = x_pos;
    node->y_pos = y_pos;
    node->snake_node_after = snake_node_after;
    node->snake_node_before = snake_node_before;

    // Return the newly created node's address
    return node;
}

void delete_snake_node(Snake_Node* snake_node){

    // Check if the snake_node is existant
    if ( snake_node == NULL )
        return;

    // Free the memory allocated for the snake_node
    free(snake_node);

    // The snake_node is now un-existant
    snake_node = NULL;
}



//////////////////////////////////////
//////////////////////////////////////
/*    Snake Class Implementation    */
//////////////////////////////////////
//////////////////////////////////////


///////////////////////////////////
//    Attributes  ::  Private    //
///////////////////////////////////

/* The snake acts somehow as a doubly linked list, with the nodes as elements */
struct Snake_Class{
    // Snake's Size
    u_short size;
    u_short num_of_nodes;

    // Snake's charateristics
    char x_speed;
    char y_speed;
    direction current_direction;

    // Snake's Body
    Snake_Node* head;
    Snake_Node* back;

    // Snake's graphics
    Bitmap* node_bmp;
};


///////////////////////////////
//    Methods  ::  Public    //
///////////////////////////////

Snake* create_snake(u_short initial_size , short x_pos , short y_pos , direction dir , Bitmap* node_bmp){

    // Allocate space for the "Object"
    Snake *snake = (Snake *) malloc ( sizeof(Snake) );

    // Check if the mem. allocation was successful
    if( snake == NULL )
        return SNAKE_ALLOC_FAILED;

    // Initialize the various snake's Attributes
    snake->size = initial_size;
    snake->node_bmp = node_bmp;
    set_snake_direction(snake , dir);

    // Create the "starting" node
    Snake_Node* starting_node = create_snake_node(x_pos , y_pos , NULL , NULL);

    if (starting_node == NULL)
        return SNAKE_NODE_ALLOC_FAILED;

    snake->head = starting_node;
    snake->back = starting_node;
    snake->num_of_nodes = 1;

    // Return the newly created snake's address
    return snake;

}


void delete_snake(Snake* snake){

    // Check if the snake is existant
    if ( snake == NULL )
        return;

    // Free the memory allocated for all the snake's nodes
    Snake_Node* next_node = snake->head;
    int snake_deleted = 0;
    do{
        next_node = snake->head->snake_node_before;
        if (next_node == NULL)
            snake_deleted = 1;
        delete_snake_node(snake->head);
        snake->head = next_node;
    } while(!snake_deleted);

    // Free the allocated memory for the bitmap
    deleteBitmap(snake->node_bmp);

    // Free the memory allocated for the snake
    free(snake);

    // The snake is now un-existant
    snake = NULL;
}

void draw_snake(Snake* snake){

    Snake_Node* current_node = snake->head;     // The starting node to draw is the head

    do{
        drawBitmap(snake->node_bmp , current_node->x_pos , current_node->y_pos , getAuxiliaryBuffer());
        current_node = current_node->snake_node_before;     // The next node to draw is the next in the tail (node before)
    } while (current_node != NULL);
}


void move_snake(Snake* snake){
    /* Moving the snake is creating a new node at the new position the snake has moved to, and delete the
       last node. The middle nodes remain the same! */

    /* Making the "mod division" by hor/ver resolution makes the snake stay inside screen borders. Summing the hor/ver resolution before
       doing this division, keeps the snake from going to negative values of X and Y, thus fixing movement beyond all the borders */
    Snake_Node* new_head = create_snake_node((snake->head->x_pos + snake->x_speed + getHorResolution()) % getHorResolution() , (snake->head->y_pos + snake->y_speed + getVerResolution()) % getVerResolution() , NULL , snake->head);

    snake->head->snake_node_after = new_head;   // The new head will come after the current head

    snake->head = new_head;     // Update the head to the new head


    if (snake->num_of_nodes < snake->size){  // Snake hasn't reached full size yet, due to beggining of
        snake->num_of_nodes ++;              // the game or due to growth from eating an apple
    }

    // Snake is at full size, delete the last node
    else{
        Snake_Node* node_to_delete = snake->back;

        snake->back = snake->back->snake_node_after;    // Change the last node
        snake->back->snake_node_before = NULL;          // The last node has no nodes before it

        delete_snake_node(node_to_delete);
    }

}


void set_snake_direction(Snake* snake , direction new_direction){
    switch(new_direction){
    case RIGHT:
        snake->x_speed = SNAKE_SPEED;
        snake->y_speed = 0;
        break;
    case LEFT:
        snake->x_speed = -SNAKE_SPEED;
        snake->y_speed = 0;
        break;
    case UP:
        snake->x_speed = 0;
        snake->y_speed = -SNAKE_SPEED;
        break;
    case DOWN:
        snake->x_speed = 0;
        snake->y_speed = SNAKE_SPEED;
        break;
    }

    snake->current_direction = new_direction;

}

direction get_snake_direction(Snake* snake){
    return snake->current_direction;
}


int check_snake_self_collision(Snake* snake){
    // Check if the head isn't colliding with any of the other nodes!
    Snake_Node* current_node = snake->head->snake_node_before;

    while (current_node != NULL){
        // Check if the node we are currently analising is colliding with the head
        if ((current_node->x_pos == snake->head->x_pos) && (current_node->y_pos == snake->head->y_pos))
            return COLLISION_FOUND;   // Collision verified! Return 1!

        current_node = current_node->snake_node_before;     // The next node to draw is the next in the tail (node before)
    }

    // No collisions were verified!! Return 0...
    return NO_COLLISION_FOUND;

}


int check_snake_head_apple_colision(Snake* snake, Apple* apple){
    if (!get_apple_status(apple))  // Can't collide with the apple when the apple isn't active ; No need for any more verifications, there is no collision
        return NO_COLLISION_FOUND;

    // If the two object's position are the same, they are colliding!
    if ((snake->head->x_pos == get_apple_x_pos(apple)) && (snake->head->y_pos == get_apple_y_pos(apple)))
        return COLLISION_FOUND;
    else
        return NO_COLLISION_FOUND;
}


int check_snake_mouse_click_collision(Snake* snake , Mouse* mouse){
    // Check if the mouse (after button clicking) isn't colliding with any of the snake's nodes!
    Snake_Node* current_node = snake->head;

    short mouse_asserted_x_pos = get_mouse_x_pos(mouse)+ (MOUSE_BITMAP_SIZE/2) - ((get_mouse_x_pos(mouse)+(MOUSE_BITMAP_SIZE/2))%SNAKE_BITMAP_SIZE);
    short mouse_asserted_y_pos = get_mouse_y_pos(mouse)+ (MOUSE_BITMAP_SIZE/2) - ((get_mouse_y_pos(mouse)+(MOUSE_BITMAP_SIZE/2))%SNAKE_BITMAP_SIZE);

    do{
        // Check if the node we are currently analising is colliding with the head
        if ((current_node->x_pos == mouse_asserted_x_pos) && (current_node->y_pos == mouse_asserted_y_pos))
            return COLLISION_FOUND;   // Collision verified! Return 1!

        current_node = current_node->snake_node_before;     // The next node to draw is the next in the tail (node before)
    } while (current_node != NULL);

    // No collisions were verified!! Return 0...
    return NO_COLLISION_FOUND;
}


void grow_snake(Snake* snake){
    snake->size ++;
}
