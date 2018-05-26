#include <xc.h>
#include "timers.h"

void timer1_init(){
    
    /* Configure prescaler. */
    T1CONbits.TCKPS = 3;
    
    TMR1CLR = 0xFFFF;
    PR1SET = 0xFFFF;
    
    /* Set interrupt prioirty and enable flags. */
    IPC1bits.T1IP = 4;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    
    T1CONbits.ON = 1;
    
    return;
}