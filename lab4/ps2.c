#include "ps2.h"
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "stdio.h"
#include "utilities.h"
#include "kbd.h"

/* Global Variables */
unsigned char packetByte;
unsigned char packet[3];
unsigned char packetCount = 0;
int ps2HookId = 12;



int ps2_subscribe_int(){

    int ps2MaskBit = ps2HookId;   // Bit we are using for ps2 interrupts

    // Subscribe the mouse interrupts
    if(sys_irqsetpolicy(PS2_IRQ , (IRQ_REENABLE | IRQ_EXCLUSIVE) , &ps2HookId) != PS2_SUCCESS)
        return PS2_INT_SUBS_ERROR;    // Error occured, return negative value

    // Enable mouse interrupts
    if(sys_irqenable(&ps2HookId) != PS2_SUCCESS)
        return PS2_INT_SUBS_ERROR;
    else
        return ps2MaskBit;  // No error occured, return the mask bit for the subscribed interrupt
}



int ps2_unsubscribe_int(){

    // Disable mouse interrupts
    if(sys_irqdisable(&ps2HookId) != PS2_SUCCESS)
        return PS2_INT_UNSUBS_ERROR;

    // Unsubscribe mouse interrupts and return failure/success
    if(sys_irqrmpolicy(&ps2HookId) != PS2_SUCCESS)
        return PS2_INT_UNSUBS_ERROR;
    else
        return PS2_SUCCESS;
}



unsigned char ps2_set_streamMode(){
    // Write the set stream mode command
    return ps2_write_cmd(PS2_CMD_SET_STREAM_MODE);
}



unsigned char ps2_enable_dataReporting(){
    // Write the enable stream mode data reporting command
    return ps2_write_cmd(PS2_CMD_ENABLE_DATA_REP);
}



unsigned char ps2_disable_dataReporting(){
    // Write the disable stream mode data reporting command
    return ps2_write_cmd(PS2_CMD_DISABLE_DATA_REP);
}



unsigned char ps2_set_remoteMode(){
    // Write the set remote mode command
    return ps2_write_cmd(PS2_CMD_SET_REMOTE_MODE);
}



unsigned char ps2_request_packet(){
    // Write the request packet command
    return ps2_write_cmd(PS2_CMD_READ_DATA);
}



unsigned char ps2_write_cmd(unsigned char cmd){

    unsigned char data;     // Variable to read data from the output buffer
    unsigned char stat;     // Variable to read the status register content
    unsigned char tryCounter = 0;       // Counts the number of tries to read the outputbuffer


    // Write the mouse command
    if(kbd_write_cmd_witharg(KBD_WRITE_BYTE_MOUSE, cmd) != KBD_SUCCESS) // Write the command to the mouse and check for success / failure
        return PS2_WRITE_KBC_CMD_ERROR;

    else{
        while(1){
            // Read the status register
            if(kbd_read_statbyte(&stat) != KBD_SUCCESS)
                return PS2_READ_STATUS_ERROR;


            // Verify if the output buffer is full (To read the ACK / NACK)
            if (stat & KBC_STATUS_OBF){
                // Read the output buffer and return it's content (ACK / NACK / ERROR)
                if(kbd_read_outputbuffer(&data) != KBD_SUCCESS)
                    return PS2_READ_OUTBUFFER_ERROR;

                else{
                    if (data == PS2_ACK || data == PS2_NACK || data == PS2_ERROR){
                        /* Only return the read data if the read data corresponds to an acknowledgement byte (ACK/NACK/ERROR)
                           Otherwise it may be an unwanted packet byte, that should be ignored and not returned */
                        return data;
                    }
                    else{
                        /* Do nothing, the read data was an unwanted packet byte */
                    }
                }
            }
            else{
                if (tryCounter == PS2_MAX_TRIES)    // Tried to read output buffer too many times, return timeout notification
                    return PS2_READ_OBF_TIMEOUT_ERROR;

                tryCounter++;
                tickdelay(micros_to_ticks(KBD_DELAY));  // wait 20 ms
            }
        }
    }
}



int ps2_int_handler(){
    unsigned char stat;   // Variable for reading the kbc status Register
    unsigned char data;   // Variable for reading the output buffer


    // Read the status byte
    if (kbd_read_statbyte(&stat) != KBD_SUCCESS)              // Verify sys_inb success/failure
        return PS2_READ_STATUS_ERROR;

    // Verify if the output buffer is indeed full
    if( stat & KBC_STATUS_OBF ) {
        // Read the output buffer content
        if ( kbd_read_outputbuffer(&data) != KBD_SUCCESS)                // Verify sys_inb success/failure
            return PS2_READ_OUTBUFFER_ERROR;

        // Check for parity/timeout error
        if ( (stat & (KBC_STATUS_PARITY | KBC_STATUS_TIMEOUT)) == 0 ){     // Check if there wasn't parity error / timeout error
            packetByte = data;    // Get the Packet Byte
            return PS2_SUCCESS;
        }
        else  // Parity / timeout error occured
            return PS2_PARITY_TIMEOUT_ERROR;
    }
    else  // Output buffer was not full
        return PS2_READ_OUTBUFFER_ERROR;
}



void ps2_print_packet(){
    /* Print the Packet Bytes Raw Content */
    printf("B1=0x%02x  B2=0x%02x  B3=0x%02x  ", packet[0], packet[1], packet[2]);

    /* Print buttons state */
    unsigned char mouseLB = (packet[0] & PS2_PACKET_MOUSELB) == PS2_PACKET_MOUSELB;     // 1 if bit set, 0 otherwise
    unsigned char mouseMB = (packet[0] & PS2_PACKET_MOUSEMB) == PS2_PACKET_MOUSEMB;
    unsigned char mouseRB = (packet[0] & PS2_PACKET_MOUSERB) == PS2_PACKET_MOUSERB;
    printf("LB=%x  MB=%x  RB=%x  ", mouseLB , mouseMB , mouseRB);

    /* Print Overflow Existance */
    unsigned char xOVF = (packet[0] & PS2_PACKET_XOVF) == PS2_PACKET_XOVF;
    unsigned char yOVF = (packet[0] & PS2_PACKET_YOVF) == PS2_PACKET_YOVF;
    printf("XOV=%x  YOV=%x  ", xOVF , yOVF);

    /* Print X-delta and Y-delta */
    unsigned char xSign = (packet[0] & PS2_PACKET_XSIGN) == PS2_PACKET_XSIGN;
    unsigned char ySign = (packet[0] & PS2_PACKET_YSIGN) == PS2_PACKET_YSIGN;
    int xdelta = convertBinaryToDecimal(packet[1], xSign);
    int ydelta = convertBinaryToDecimal(packet[2], ySign);
    printf("X=%-4d  Y=%-4d\n", xdelta, ydelta);
}



int ps2_parse_packetByte(){
    // First array element (First Packet Byte)
    if(packetCount == 0){
        if(!(packetByte & BIT(3))){
            return PS2_UNSYNCHED_PACKET;    // Packet is out of synch! Ignore this packet byte
        }
        packet[0] = packetByte;
        packetCount++;
        return PS2_INCOMPLETE_PACKET;       // Packet is still not complete
    }

    // Second array element (Second Packet Byte)
    else if(packetCount == 1){
        packet[1] = packetByte;
        packetCount++;
        return PS2_INCOMPLETE_PACKET;       // Packet is still not complete
    }

    // Third array element (Third Packet Byte)
    else {
        packet[2] = packetByte;
        packetCount = 0;                    // Reset the counter
        return PS2_COMPLETE_PACKET;         // Packet is now complete!!
    }
}



int ps2_remote_read_packetByte(unsigned char * packetByte){
    unsigned char stat;
    unsigned short tryCounter = 0;

    while(1){
        // Read the status register
        if(kbd_read_statbyte(&stat) != KBD_SUCCESS)
            return PS2_READ_STATUS_ERROR;


        // Verify if the output buffer is full
        if (stat & KBC_STATUS_OBF){
            // Verify if there wasn't timeout / parity error
            if ( (stat & (KBC_STATUS_PARITY | KBC_STATUS_TIMEOUT)) == 0 ) {
                // Read the output buffer and return it's content
                if(kbd_read_outputbuffer(packetByte) != KBD_SUCCESS)
                    return PS2_READ_OUTBUFFER_ERROR;
                else{
                    return PS2_SUCCESS;
                }
            }
            else    // There was parity / timeout error
                return PS2_PARITY_TIMEOUT_ERROR;
        }
        else{
            if (tryCounter == PS2_MAX_TRIES)    // Tried to read output buffer too many times, return timeout notification
                return PS2_READ_OBF_TIMEOUT_ERROR;

            tryCounter++;
            tickdelay(micros_to_ticks(KBD_DELAY));  // wait 20 ms
        }
    }
}
