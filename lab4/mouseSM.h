#ifndef _MOUSE_SM_H_
#define _MOUSE_SM_H_


/* Possible States for the State Machine */
typedef enum { INIT = 0 , DRAWING_MODE , FINAL } State;

/* Possible Events for the State Machine */
typedef enum { RB_DOWN = 0 , RB_UP , LINE_DRAWN } Event;


/* Simplified version (very simplified!!) of Sergio Prados' State Machine implementation (Recomended documentation in the Class Presentation)
   in his blog at https://sergioprado.org/maquina-de-estados-em-c/ , second example. In our implementation we don't use an Event Buffer, since
   this state machine is very simple and needs not to handle multiple events at once */

struct StateMachine{
    State state;                 /* Current state, starts in the INIT state */
    Event event;                 /* Starts empty, holds an occured event when am event is triggered */
};



////////////////////////////
//  FUNCTION PROTOTYPES   //
////////////////////////////

/**
 * @brief Checks for events on the mouse pakcet
 *
 * @param length    Length of the X variation that is required for the line in mouse_test_gesture()
 */
void mouse_event_trigger(short length);

/**
 * @brief Handles a mouse event, changing to the appropriate state based on the event
 */
void handle_mouse_event();



#endif      /* _MOUSE_SM_H_ */
