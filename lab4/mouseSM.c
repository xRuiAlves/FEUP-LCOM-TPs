#include "mouseSM.h"
#include "ps2.h"
#include "i8042.h"
#include "utilities.h"
#include "stdio.h"

/* Extern Variables from ps2.c */
extern unsigned char packet[3];
extern unsigned char packetCount;

/* Create the State Machine */
struct StateMachine state_machine;
int mouseEventOccured = 0;          // Flag to check if a mouse event has occured (1 if event occured, 0 if not)


void mouse_event_trigger(short length){
    /* Since we want a line with a positive slope, that means that when the line is being drawn, the xSign and the ySign must be equal (positive if drawn in the
       top-right direction or negative if drawn in the bottom-left direction) and can not change during the drawing (can't change direction from top-right to
       bottom-left in the middle of the drawing) */
    static unsigned char xSign = 0;
    static unsigned char ySign = 0;
    static int xDisplacement = 0;
    unsigned char newXsign;
    unsigned char newYsign;

    /* Right mouse button pressed while not in drawing mode */
    if ((state_machine.state == INIT) && (packet[0] & PS2_PACKET_MOUSERB)){
        state_machine.event = RB_DOWN;
        xDisplacement = 0;      // Reset displacement
        mouseEventOccured = 1;
        return;
    }

    /* Right mouse button released while in drawing mode */
    if ((state_machine.state == DRAWING_MODE) && (!(packet[0] & PS2_PACKET_MOUSERB))){
        state_machine.event = RB_UP;
        mouseEventOccured = 1;
        return;
    }

    /* In drawing mode, check if the line is being correctly drawn */
    if (state_machine.state == DRAWING_MODE){
        /* Get the 'new' (or not) x and y signs from the packet */
        newXsign = (packet[0] & PS2_PACKET_XSIGN) == PS2_PACKET_XSIGN;
        newYsign = (packet[0] & PS2_PACKET_YSIGN) == PS2_PACKET_YSIGN;

        /* If the mouse X or Y displacement are 0, the line being drawn is not positively slopped, it's vertical or horizontal, respectively */
        if ( (packet[1] == 0) || (packet[2] == 0) ){
            xDisplacement = 0;
        }

        /* Check for Overflow in both X and Y axis */
        else if ( (packet[0] & PS2_PACKET_XOVF) || (packet[0] & PS2_PACKET_XOVF) ){
            xDisplacement = 0;
        }

        /* Different sigs for X and Y means that the line is not being drawn with positive slope... */
        else if ( newXsign != newYsign ){
            xDisplacement = 0;
        }

        /* X and Y Sign changed from the last packet -> Reset the drawing line */
        else if ( (newXsign != xSign) || (newYsign != ySign) ) {
            // Update the signs (new direction)
            xSign = newXsign;
            ySign = newYsign;
            // Update the displacement
            xDisplacement = absoluteValue(convertBinaryToDecimal(packet[1],xSign));
        }

        /* Still drawing the same line -> increment the displacement */
        else {
            xDisplacement += absoluteValue(convertBinaryToDecimal(packet[1],xSign));
        }

        /* Check if the required length was drawn */
        if (xDisplacement >= length){
            state_machine.event = LINE_DRAWN;
            mouseEventOccured = 1;
        }
    }
}

void handle_mouse_event(){
    switch(state_machine.event){
        case RB_DOWN:
            /* If the right button is pressed and we are in the INIT state, the state changes to DRAWING_MODE */
            if(state_machine.state == INIT) state_machine.state = DRAWING_MODE;
            break;
        case RB_UP:
            /* If the right button is released and we are in the DRAWING_MODE state, the stte changes to INIT */
            if(state_machine.state == DRAWING_MODE) state_machine.state = INIT;
            break;
        case LINE_DRAWN:
            /* If the mouse movement draws a line within the specified parameters and the state is DRAWING_MODE (the right button is pressed)
               then the state changes to FINAL, and the function is finished */
            if(state_machine.state == DRAWING_MODE) state_machine.state = FINAL;
            break;
    }
}
