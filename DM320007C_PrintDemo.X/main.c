#include <stdlib.h>
#include <stdio.h>
#include <proc/p32mz2048efm144.h>
#include "include/timers.h"
#include "include/uart.h"

/* This project is meant to be a solid foundation for building programs with
 * the DM320007C development board from Microchip. It takes care of defining
 * the correct configuration bits, interrupt setup, and printf() functionality.
 * 
 * I purchased one of these boards and was frustrated that there was no quick
 * way to get up and running. The examples don't help much and generally require
 * so much toolchain and system path setup, it never ends up compiling. This
 * project is intended on being imported to MPLABX, and immidiately ready for
 * compile & download to the development kit.
 * 
 * This program uses the Timer1 interrupt to flash the green user LED found
 * on the board. It also gives default settings for getting printf() to work,
 * which by default uses UART2.
 */
static void wait(unsigned int);

int main(int argc, char **argv){
    
    // Configure LED2 to flash (timer 1 interrupt).
    TRISHbits.TRISH2 = 0;
    LATHbits.LATH2 = 1;
    
    // Set multivector interrupt mode.
    INTCONbits.MVEC = 1;
    
    // Initialize peripherials.
    timer1_init();
    uart_init();
    
    __builtin_enable_interrupts();
    
    char message[] = "Hello World!\n\r\0";
    while(1){
        printf(message);
        wait(100000);
    }
 
    return 1;
}

// Blocking delay - should not normally be used as it ties up the CPU.
static void wait(unsigned int time){
    unsigned int t;
    
    for(t = 0; t < time; t++);
    return;
}

