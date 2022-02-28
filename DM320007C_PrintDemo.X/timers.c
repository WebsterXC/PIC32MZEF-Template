#include <xc.h>
#include "include/timers.h"

void timer1_init(){
    
    // Set clock divisor to 256.
    T1CONbits.TCKPS = 3;
    
    // Clear timer 1 accumulator and set period to max.
    TMR1CLR = 0xFFFF;
    PR1SET = 0xFFFF;
    
    // Set interrupt prioirty and enable flags.
    IPC1bits.T1IP = 6;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    
    T1CONbits.ON = 1;
    
    return;
}