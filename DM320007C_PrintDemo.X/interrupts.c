#include <sys/attribs.h>
#include <proc/p32mz2048efm144.h>
#include <xc.h>

void  __ISR( _TIMER_1_VECTOR, IPL6SOFT) __attribute__((nomips16)) _T1Interrupt(void){
    
    LATHINV = 0x4;
    IFS0bits.T1IF = 0;
}