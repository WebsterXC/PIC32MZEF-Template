#include <stdlib.h>
#include <stdio.h>
#include <proc/p32mz2048efm144.h>

#include "timers.h"
#include "uart.h"

/* This project is meant to be a solid foundation for building programs with
 * the DM320007C development board from Microchip. It takes care of defining
 * the correct configuration bits, interrupt setup, and printf() functionality.
 * 
 * This program uses the Timer1 interrupt to flash the green user LED found
 * on the board. It also gives default settings for getting printf() to work,
 * which by default uses UART2.
 */

static void wait(void);
int main(int argc, char **argv){
    
    TRISHCLR = 0xFFFF;  // Assign all of Port H as outputs.
    LATHCLR = 0xFFFF;   // Clear the state register.
    
    /* The UART derives it's clock from the peripherial bus clock, which
     * is sourced from the system clock (200 MHz). 200/(P2DIV+1) = 50 MHz */
    PB2DIV = 3;
    
    // Set multivector interrupt mode.
    INTCONbits.MVEC = 1;
    
    /* Initialize the shadow interrupt registers. This is a predefined
     * value that Microchip has said to use if you don't care about
     * using or wanting to maintain shadow registers. */
    PRISS = 0x76543210;
    
    // Initialize peripherials.
    timer1_init();
    uart_init();
    
    __builtin_enable_interrupts();
    wait();
    
    char message[] = "Hello World!\n\r\0";
    while(1){
        printf(message);
        wait();
    }
 
    return 1;
}

/* Non-Timer based wait. */
static void wait(void){
    unsigned int t;
    for( t=0 ; t < 100000 ; t++);
}

