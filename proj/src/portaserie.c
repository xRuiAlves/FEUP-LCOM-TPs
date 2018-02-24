#include "portaserie.h"
#include "uart.h"
#include <minix/drivers.h>
#include "Game.h"
#include "SnakeyNix.h"
#include "spProtocol.h"


int uartHookId = 4;

static unsigned char character;
static unsigned char receivedMessage[3];
static unsigned char messageBytesCount = 0;


int uart_subscribe_int(){

    int uartMaskBit = uartHookId;   // Bit we are using for uart interrupts

    // Subscribe the uart interrupts
    if(sys_irqsetpolicy(UART_COM1_IRQ , (IRQ_REENABLE | IRQ_EXCLUSIVE) , &uartHookId) != OK)
        return UART_INT_SUBS_FAILED;    // Error occured, return negative value

    // Enable uart interrupts
    if(sys_irqenable(&uartHookId) != OK)
        return UART_INT_SUBS_FAILED;
    else
        return uartMaskBit;  // No error occured, return the mask bit for the subscribed interrupt
}



int uart_unsubscribe_int(){

    // Disable uart interrupts
    if(sys_irqdisable(&uartHookId) != OK)
        return UART_INT_UNSUBS_FAILED;

    // Unsubscribe uart interrupts and return failure/success
    return ( sys_irqrmpolicy(&uartHookId) );
}


int uart_read_LSR(u_char* status){
    unsigned long stat;   // Variable for reading the status (sys_inb takes an unsigned long as parameter)

    // Read the line status register
    if (sys_inb(UART_COM1_BASE_ADDR + UART_LSR , &stat) != OK)              // Verify sys_inb success/failure
        return UART_READ_LSR_FAILED;
    else{
        *status = (u_char) stat;    // Reading was successful, save the read value in the status
        return OK;
    }
}


int uart_read_RBR(u_char* content){
    unsigned long cont;   // Variable for reading the buffer reg (sys_inb takes an unsigned long as parameter)

    // Read the receiver buffer register
    if (sys_inb(UART_COM1_BASE_ADDR + UART_RBR , &cont) != OK)              // Verify sys_inb success/failure
        return UART_READ_RBR_FAILED;
    else{
        *content = (u_char) cont;    // Reading was successful, save the read value in the content
        return OK;
    }
}


int uart_read_IER(u_char* content){
    unsigned long cont;   // Variable for reading the int. enable reg (sys_inb takes an unsigned long as parameter)

    // Read the interrupt enable register
    if (sys_inb(UART_COM1_BASE_ADDR + UART_IER , &cont) != OK)              // Verify sys_inb success/failure
        return UART_READ_IER_FAILED;
    else{
        *content = (u_char) cont;    // Reading was successful, save the read value in the content
        return OK;
    }
}


int uart_read_IIR(u_char* content){
    unsigned long cont;   // Variable for reading the int. identification reg (sys_inb takes an unsigned long as parameter)

    // Read the interrupt identification register
    if (sys_inb(UART_COM1_BASE_ADDR + UART_IIR , &cont) != OK)              // Verify sys_inb success/failure
        return UART_READ_IIR_FAILED;
    else{
        *content = (u_char) cont;    // Reading was successful, save the read value in the content
        return OK;
    }
}


int uart_read_LCR(u_char* content){
    unsigned long cont;   // Variable for reading the line control reg (sys_inb takes an unsigned long as parameter)

    // Read the line control register
    if (sys_inb(UART_COM1_BASE_ADDR + UART_LCR , &cont) != OK)              // Verify sys_inb success/failure
        return UART_READ_LCR_FAILED;
    else{
        *content = (u_char) cont;    // Reading was successful, save the read value in the content
        return OK;
    }
}


int uart_read_DLL(u_char* content){
    unsigned long cont;   // Variable for reading the divisor latch LSB (sys_inb takes an unsigned long as parameter)

    // Read the divisor latch LSB
    if (sys_inb(UART_COM1_BASE_ADDR + UART_DLL , &cont) != OK)              // Verify sys_inb success/failure
        return UART_READ_DLL_FAILED;
    else{
        *content = (u_char) cont;    // Reading was successful, save the read value in the content
        return OK;
    }
}


int uart_read_DLM(u_char* content){
    unsigned long cont;   // Variable for reading the divisor latch MSB (sys_inb takes an unsigned long as parameter)

    // Read the divisor latch MSB
    if (sys_inb(UART_COM1_BASE_ADDR + UART_DLM , &cont) != OK)              // Verify sys_inb success/failure
        return UART_READ_DLM_FAILED;
    else{
        *content = (u_char) cont;    // Reading was successful, save the read value in the content
        return OK;
    }
}


int uart_write_THR(u_char data){
    return ( sys_outb(UART_COM1_BASE_ADDR + UART_THR , data) );
}


int uart_write_LCR(u_char data){
    return ( sys_outb(UART_COM1_BASE_ADDR + UART_LCR , data) );
}


int uart_write_IER(u_char data){
    return ( sys_outb(UART_COM1_BASE_ADDR + UART_IER , data) );
}


int uart_write_DLL(u_char data){
    return ( sys_outb(UART_COM1_BASE_ADDR + UART_DLL , data) );
}


int uart_write_DLM(u_char data){
    return ( sys_outb(UART_COM1_BASE_ADDR + UART_DLM , data) );
}


int uart_enable_divisor_latch_access(){
    u_char lcr_content;

    // Read Line Control Register
    if (uart_read_LCR(&lcr_content) != OK)
        return UART_ENABLE_DL_ACCESS_FAILED;

    // Set the DLAB bit (bit 7 , for Divisor Latch Accessing)
    lcr_content = ( lcr_content | UART_LCR_DLAB );

    // Write the new Line Control Register byte (with DLAB bit set)
    return ( uart_write_LCR(lcr_content) );
}


int uart_disable_divisor_latch_access(){
    u_char lcr_content;

    // Read Line Control Register
    if (uart_read_LCR(&lcr_content) != OK)
        return UART_DISABLE_DL_ACCESS_FAILED;

    // Un-set the DLAB bit (bit 7 un-set, for RBR (read) or THR (write) access)
    u_char mask = (u_char) ~UART_LCR_DLAB;
    lcr_content = ( lcr_content & mask );

    // Write the new Line Control Register byte (with DLAB bit un-set)
    return ( uart_write_LCR(lcr_content) );
}


int uart_set_bit_rate(u_short bit_rate){

    u_short new_bit_rate = 115200/bit_rate; // Bit rate is set by writting 115200/desired_bit_rate

    // Compute the values to write in the DLL and DLM
    u_char new_bit_rate_lsb = UART_BIT_RATE_LSB(new_bit_rate);
    u_char new_bit_rate_msb = UART_BIT_RATE_MSB(new_bit_rate);

    // Write the values in the DLL and DLM registers

    // Enable Divisor Latch Access
    if ( uart_enable_divisor_latch_access() != OK )
        return UART_SET_BIT_RATE_FAILED;

    // Write to DLL
    if ( uart_write_DLL(new_bit_rate_lsb) != OK )
        return UART_SET_BIT_RATE_FAILED;

    // Write to DLM
    if ( uart_write_DLM(new_bit_rate_msb) != OK )
        return UART_SET_BIT_RATE_FAILED;

    // Disable Divisor Latch Access
    if ( uart_disable_divisor_latch_access() != OK )
        return UART_SET_BIT_RATE_FAILED;

    return OK;
}


void uart_flush_RBR(){
    u_char lsr_content;
    u_char rbr_content;

    // Read the Line Status Register
    if ( uart_read_LSR(&lsr_content) != OK)
        return;

    // If there's anything in the RBR , flush it
    if ( lsr_content & UART_LSR_RCV_DATA )
        uart_read_RBR(&rbr_content);
}


int uart_write_character(u_char character){
    u_char lsr_content;
    u_char tryCounter = 0;       // Counts the number of tries to write the character

    while(1){
        // Read the Line Status Register
        if ( uart_read_LSR(&lsr_content) != OK)
            return UART_READ_LSR_FAILED;

        // Check if the Transmitter Holding Register is Empty
        if ( lsr_content & UART_LSR_THRE ){
            // Write the character and return success / failure
            return ( uart_write_THR(character) );
        }

        else{
            // Transmitter Holding Register not empty. Wait some time and try again
            tryCounter++;
        }

        if (tryCounter == SPP_MAX_NUM_TRIES)
            return UART_WRITE_CHAR_FAILED;  // Max number of time reached!
        else
            tickdelay(micros_to_ticks(SPP_WAIT_TIME));  // Wait some time
    }
}

int uart_write_message(u_char message_content){

    ///// MESSAGE HEADER BYTE /////

    // Write it
    if (uart_write_character(SPP_HEADER_BYTE) != OK)
        return UART_WRITE_MESSAGE_FAILED;

    // Check if it was Acknowledged
    if (uart_get_acknowledgement_byte() != SPP_ACK_BYTE)
        return UART_WRITE_MESSAGE_FAILED;


    ///// MESSAGE CONTENT BYTE /////

    // Write it
    if (uart_write_character(message_content) != OK)
        return UART_WRITE_MESSAGE_FAILED;

    // Check if it was Acknowledged
    if (uart_get_acknowledgement_byte() != SPP_ACK_BYTE)
        return UART_WRITE_MESSAGE_FAILED;


    ///// TRAILER BYTE /////

    // Write it
    if (uart_write_character(SPP_TRAILER_BYTE) != OK)
        return UART_WRITE_MESSAGE_FAILED;

    // Check if it was Acknowledged
    if (uart_get_acknowledgement_byte() != SPP_ACK_BYTE)
        return UART_WRITE_MESSAGE_FAILED;

    // Message was successfully written
    return OK;
}


u_char uart_get_acknowledgement_byte(){
    u_char lsr_content;
    u_char acknowledgement_byte;
    u_char tryCounter = 0;       // Counts the number of tries to write the character


    while(1){
        // Read the Line Status Register
        if ( uart_read_LSR(&lsr_content) != OK)
            return SPP_ERROR_BYTE;

        // Check if the Receiver Buffer Register has Anything in it
        if ( lsr_content & UART_LSR_RCV_DATA ){
            // Read the ack/nack byte
            if (uart_read_RBR(&acknowledgement_byte) != OK)
                return SPP_ERROR_BYTE;
            else{
                if (acknowledgement_byte == SPP_ACK_BYTE)       // ACK Byte
                    return SPP_ACK_BYTE;
                else if (acknowledgement_byte == SPP_NACK_BYTE) // NACK Byte
                    return SPP_NACK_BYTE;
                else                                            // Unknown Content. Error.
                    return SPP_ERROR_BYTE;
            }
        }

        else{
            // Receiver Buffer Register has nothing in it ; Wait some time for the ack/nack byte
            tryCounter++;
        }

        if (tryCounter == SPP_MAX_NUM_TRIES)    // Timeout. No Ack/Nack byte received
            return SPP_ERROR_BYTE;
        else
            tickdelay(micros_to_ticks(SPP_WAIT_TIME));  // Wait some time
    }
}


int uart_parse_message(){
    // First array element (Message Header Byte)
    if(messageBytesCount == 0){
        if (character == SPP_HEADER_BYTE){  // Valid Header Byte
            receivedMessage[0] = character;
            messageBytesCount++;
            return SPP_INCOMPLETE_MESSAGE;  // Message is not complete yet.
        }
        else{   // Invalid Header Byte!
            messageBytesCount = 0;
            return SPP_INVALID_HEADER;
        }
    }

    // Second array element (Message Content)
    else if(messageBytesCount == 1){
        receivedMessage[1] = character;
        messageBytesCount++;
        return SPP_INCOMPLETE_MESSAGE;      // Message is not complete yet.
    }

    // Third array element (Message Trailer Byte)
    else {
        if (character == SPP_TRAILER_BYTE){  // Valid Trailer Byte
            receivedMessage[2] = character;
            messageBytesCount = 0;
            return SPP_COMPLETE_MESSAGE;  // Message is now complete!
        }
        else{   // Invalid Trailer Byte!
            messageBytesCount = 0;
            return SPP_INVALID_TRAILER;
        }
    }
}


void handle_uart(){
    u_char iir_content;
    u_char message_status;

    // Read the Interrupt Identification Register to find the source of the interrupt
    if ( uart_read_IIR(&iir_content) != OK)
        return;

    // Check if there is indeed an interupt pending
    if ( iir_content & UART_IIR_NO_INT )
        return;

    // Handle the interrupt
    u_char int_id = (iir_content & UART_INT_ID_MASK);

    switch(int_id){
    // Received Data Available Interrupt
    case UART_IIR_RDA:
        // Read the Receiver Buffer Register
        if (uart_read_RBR(&character) != OK)
            return;

        // Parse the message until getting the full message
        message_status = uart_parse_message();

        if (message_status == SPP_COMPLETE_MESSAGE){
            // Write the Acknowledgement Byte for the character
            uart_write_character(SPP_ACK_BYTE);

            if (receivedMessage[1] == UP_ARROW)
                raise_event(get_game_state_machine(getSnakeyNix()) , UP_BUTTON_CLICKED);
            else if (receivedMessage[1] == DOWN_ARROW)
                raise_event(get_game_state_machine(getSnakeyNix()) , DOWN_BUTTON_CLICKED);
            else if (receivedMessage[1] == LEFT_ARROW)
                raise_event(get_game_state_machine(getSnakeyNix()) , LEFT_BUTTON_CLICKED);
            else if (receivedMessage[1] == RIGHT_ARROW)
                raise_event(get_game_state_machine(getSnakeyNix()) , RIGHT_BUTTON_CLICKED);
            else if (receivedMessage[1] == PLAYER_1_JOINED)
                raise_event(get_game_state_machine(getSnakeyNix()) , PLAYER_1_JOINED_GAME);
            else if (receivedMessage[1] == PLAYER_2_JOINED)
                raise_event(get_game_state_machine(getSnakeyNix()) , PLAYER_2_JOINED_GAME);
            else if (receivedMessage[1] == PLAYER_1_LEFT)
                raise_event(get_game_state_machine(getSnakeyNix()) , PLAYER_1_LEFT_GAME);
        }
        else if (message_status==SPP_INVALID_HEADER || message_status==SPP_INVALID_TRAILER){    // Invalid Data ; Send NACK byte
            uart_write_character(SPP_NACK_BYTE);
        }
        else{   // Message not complete yet, but the byte was Acknowledged!
            uart_write_character(SPP_ACK_BYTE);
        }

        break;

    // Receiver Line Status Interrupt
    case UART_IIR_RLS:
        // Invalid Data! Flush the RBR and send a NACK byte
        uart_flush_RBR();
        uart_write_character(SPP_NACK_BYTE);
        break;
    }
}
