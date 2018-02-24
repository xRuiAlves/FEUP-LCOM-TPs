/* int ps2_int_handler(); */
/* using an initialized global variable to save the packet byte that was read (packetByte) */
/* returns 0 on success and 1 on failure */
#include "i8042.h"

.global _packetByte               /* Byte that corresponds to the packet byte that was read from the output buffer */
.global _ps2_int_handler
.data
_packetByte:
    .byte 0

.text
_ps2_int_handler:
    /* read the status register */
    inb $KBC_STATUS_REG, %al

    /* Check if output buffer is indeed full */
    test $KBC_STATUS_OBF, %al
    jz ERRO

    /* Check for parity/timeout error */
    test $KBC_STATUS_PARITY, %al
    jnz ERRO
    test $KBC_STATUS_TIMEOUT, %al
    jnz ERRO

    /* Read the output buffer and place it's content in _packetByte */
    inb $KBC_OUT_BUF, %al
    movb %al, _packetByte

    /* All was sucessful, return 0 (Success) */
    movl $0, %eax
    jmp END

ERRO:
    /* Error occured, return 1 (Error) */
    movl $1, %eax

END:
    ret

.end
