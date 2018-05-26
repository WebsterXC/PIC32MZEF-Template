#include <sys/attribs.h>
#include <proc/p32mz2048efm144.h>
#include <xc.h>

void __ISR_AT_VECTOR(_TIMER_1_VECTOR, IPL4SRS) __attribute__((nomips16)) _T1Interrupt(void){
    
    // Invert RH14 and clear the timer counter.
    LATHINV = 0x4;
    TMR1CLR = 0xFFFF;
    
    // Clear the interrupt flag for Timer 1.
    IFS0bits.T1IF = 0;
}