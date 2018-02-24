#include "SnakeyNix.h"
#include "StateMachine.h"
#include "video_gr.h"
#include "video.h"
#include "stdio.h"
#include "utilities.h"
#include <minix/drivers.h>

#define VIDEO_INIT_FAILED        -1

static void print_error(int error_num);
static Game* SnakeyNix;
unsigned int endTheGame = 0;

int main(){

    // Synchronize with the ressurection server and enable input/output operations for assembly interrupt handlers
    sef_startup();
    sys_enable_iop(SELF);


    // Initialize the game itself and check for success
    int success;
    SnakeyNix = initialize_game(&success);

    if (success != 0){
        printf("\n\nError Occured when trying to initialize the game.\n");
        print_error(success);
        end_game(SnakeyNix);
        return success;
    }
    else if (SnakeyNix == NULL)
        return FAILED_GAME_INITIALIZATION;


    if (vg_init(VBE_MODE_800x600_RGB565) == NULL){
        printf("\n\nError Occured when initializing video graphics.\n");
        end_game(SnakeyNix);
        return VIDEO_INIT_FAILED;
    }


    // Play the game until "endTheGame" flag is set
    while(endTheGame == 0){
        update_game(SnakeyNix);
    }


    // Free the allocated memory for the game and return to default text mode
    end_game(SnakeyNix);
    vg_exit();

    return OK;
}

static void print_error(int error_num){
    printf("\nCause:  ");

    switch(error_num){
        case TIMER_INT_SUBSCRIBE_FAILED:
            printf("Failed to subscribe timer interrupts\n\n");
            break;

        case KBD_INT_SUBSCRIBE_FAILED:
            printf("Failed to subscribe keyboard interrupts\n\n");
            break;

        case MOUSE_INT_SUBSCRIBE_FAILED:
            printf("Failed to subscribe mouse interrupts\n\n");
            break;

        case UART_INT_SUBSCRIBE_FAILED:
            printf("Failed to subscribe UART interrupts\n\n");
            break;

        case RTC_INT_SUBSCRIBE_FAILED:
            printf("Failed to subscribe RTC interrupts\n\n");
            break;

        case UART_CONFIGURATION_FAILED:
            printf("Failed to configure the UART\n\n");
            break;

        case RTC_CONFIGURATION_FAILED:
            printf("Failed to configure the RTC\n\n");
            break;

        case MOUSE_OBJ_CREATION_FAILED:
            printf("Failed to create Mouse object\n\n");
            break;

        case SNAKE_OBJ_CREATION_FAILED:
            printf("Failed to create Snake object\n\n");
            break;

        case APPLE_OBJ_CREATION_FAILED:
            printf("Failed to create Apple object\n\n");
            break;

        case SM_OBJ_CREATION_FAILED:
            printf("Failed to create State Machine object\n\n");
            break;

        case BUTTON_OBJ_CREATION_FAILED:
            printf("Failed to create Button object\n\n");
            break;

        case SNAKE_BMP_NOT_FOUND:
            printf("Snake's bitmap file not found\n\n");
            break;

        case BACKGROUND_BMP_NOT_FOUND:
            printf("Background's bitmap file not found\n\n");
            break;

        case MENU_SCREEN_BMP_NOT_FOUND:
            printf("Menu screen bitmap file not found\n\n");
            break;

        case CREDITS_SCREEN_BMP_NOT_FOUND:
            printf("Credits Screen bitmap file not found\n\n");
            break;

        case MOUSE_BMP_NOT_FOUND:
            printf("Mouse's bitmap file not found\n\n");
            break;

        case APPLE_BMP_NOT_FOUND:
            printf("Apple's bitmap file not found\n\n");
            break;

        case BUTTON_BMP_NOT_FOUND:
            printf("Button's bitmap file not found\n\n");
            break;

        case LOSING_SCREEN_BMP_NOT_FOUND:
            printf("Losing Screen's bitmap file not found\n\n");
            break;

        case P1_LEFT_BMP_NOT_FOUND:
            printf("Player One left Screen's bitmap file not found\n\n");
            break;

        case P1_WAITING_BMP_NOT_FOUND:
            printf("Waiting for Player One Screen's bitmap file not found\n\n");
            break;

        default:
            printf("Unknown error occured\n\n");    /* ERROR UNRECOGNIZED ! */
            break;
    }
}

Game* getSnakeyNix(){
    return SnakeyNix;
}
