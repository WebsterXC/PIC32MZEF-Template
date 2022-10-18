#include <stdlib.h>
#include <stdio.h>
#include <proc/p32mz2048efm144.h>
#include "include/timers.h"
#include "include/uart.h"
#include "include/i2c.h"
#include "include/eeprom.h"

#define EEPROM_INSTALLED    0       // Change to 1 if 24LC32A is installed.

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
 * 
 * Additionally, this program offers an example of working with I2C and
 * implements it by interfacing with a Microchip 24LC32A EEPROM. I purchased
 * a DIP version of the EEPROM and set it up in a breadboard, using RA2 & RA3
 * as my I2C interface.
 * 
 */
static void wait(uint32_t);
static void eeprom_example();

int main(int argc, char **argv){
    char message[] = "Hello World!\n\r\0";
    
    // Configure LED2 to flash (timer 1 interrupt).
    TRISHbits.TRISH2 = 0;
    LATHbits.LATH2 = 1;
    
    // Set multivector interrupt mode.
    INTCONbits.MVEC = 1;
    
    // Initialize peripherials.
    timer1_init();      // Used for rollover interrupt.
    uart_init();        // Used for terminal (printf) output.
    i2c_init();         // Used for communicaiton with EEPROM.
    
    // Initialization 
    __builtin_enable_interrupts();
    
    printf("\r\n\r\n|----Initialized----|\r\n");

#if EEPROM_INSTALLED
    int status = 0;
    eeprom_example();
    wait(10000000);
    printf("Erasing EEPROM...\r\n");
    eeprom_flash();
    printf("Verifying EEPROM... ");
    status = eeprom_verify();
    printf("Result (0 is Success): %d\r\n\r\n", status);
#endif
    
    // Main program entry (we don't have one).
    while(1){
        printf(message);
        wait(30000000);
    }
 
    return 1;
}

//---------------------------------------------------------------------------//

// Small EEPROM implementation for both single and sequential read/writes.
static void eeprom_example(){
    const uint16_t addr  = 0x3A0;
    const uint16_t addr2 = 0x5E0;
    uint8_t data[1] = {0x00};
    uint8_t data2[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                         0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
                         0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                         0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F};
    uint8_t block[32];
    
    // Write 0xAA to address 0x3A0 and read it back.
    printf("Write 0xAA to address %x...\r\n", addr);
    eeprom_write(addr, 0xAA);
    eeprom_wait_for_write_cycle();
    printf("Read address %x: ", addr);
    eeprom_read(addr, data);
    printf("%x\r\n\r\n", data[0]);
    
    // Write 32 increasing bytes starting at address 0x5E0 and read it back.
    printf("Sequential write to address %x...\r\n", addr2);
    eeprom_write_seq(addr2, data2, EEPROM_SEQ_BYTES);
    eeprom_wait_for_write_cycle();
    printf("Readback 32 from address %x: \r\n", addr2);
    eeprom_read_seq(addr2, block, EEPROM_SEQ_BYTES);
    for(int i = 0; i < EEPROM_SEQ_BYTES; i++){
        printf("%X ", block[i]);
    }
    printf("\r\n\r\n");
    
    return;
}

// Blocking delay - should not normally be used as it ties up the CPU.
static void wait(uint32_t time){
    unsigned int t;
    
    for(t = 0; t < time; t++);
    return;
}
