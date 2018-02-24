.global _timerIntCounter
.global _timer_int_handler
.global _timer_get_int_counter

.text

/* void timer_int_handler() */
/* using an initialized global variable (timerIntCounter) that is incremented */
_timer_int_handler:

    /* Increment the Timer Interruption Counter */
    incl _timerIntCounter

    ret



/* unsigned int timer_get_int_counter() */
_timer_get_int_counter:

    /* Move the Timer Interruption Counter number into eax (return value) */
    movl _timerIntCounter, %eax

    ret

.end
