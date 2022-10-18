#include <xc.h>
#include "include/i2c.h"

// This is a very basic implementation of the I2C module on the PIC32MZ. Care
// should be taken when scaling this to a real device as I have not implemented
// return codes nor timeouts in the wait-for-ack logic.

#define SYS_CLK_HZ          100000000U       // System Master Clock (Hz)
#define PB_CLK_HZ           50000000U        // Peripherial Bus Clock
#define I2Cx_DEFAULT_BAUD   400000U          // Target 400kHz Bus

static uint16_t i2c_calc_brg(uint32_t);
static inline void i2c_wait_for_idle(void);

static inline void i2c_ack(void);
static inline void i2c_nack(void);

// Pinout for DM320007C:
// - Implement I2C2. Uses PBCLK2 @ 50 MHz.
// - SDA2: RA3 (J12-3)
// - SCL2: RA2 (J12-5)
void i2c_init(void){
    uint16_t brg;
    
    brg = i2c_calc_brg(I2Cx_DEFAULT_BAUD);
    
    I2C2CONCLR = 0xFFFFFFFF;
    I2C2BRG    = brg;
    I2C2CONSET = 0x8000;
    
    return;
}

void i2c_read(uint8_t *val, bool nack){
    
    if(val == NULL){
        return;
    }
    
    I2C2CONbits.RCEN = 1;
    while(I2C2CONbits.RCEN);
    while(!I2C2STATbits.RBF);
    *val = I2C2RCV;
    
    // Send either ACK or NACK.
    if(nack){
        i2c_nack();
    }else{
        i2c_ack();
    }
    
    return;
}

void i2c_write(uint8_t val, bool wait){
    
    I2C2TRN = val;
    while(I2C2STATbits.TBF == 1);
    
    i2c_wait_for_idle();

    if(wait == true){
        i2c_wait_for_ack();
    }
    
    return;
}

uint8_t i2c_status(void){
    return (uint8_t)I2C2STATbits.ACKSTAT;
}

// For simplicity, I didn't include a timeout but typically you should
// have one in case the slave device sends a NACK.
void i2c_wait_for_ack(void){
    
    while(I2C2STATbits.ACKSTAT == 1);
    
    return;
}

// Same as above, a timeout should be added for robustness.
void i2c_wait_for_nack(void){
    
    while(I2C2STATbits.ACKSTAT == 0);
    
    return;
}

void i2c_start(void){
    
    i2c_wait_for_idle();
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN == 1);
    
    return;
}

void i2c_stop(void){
    
    i2c_wait_for_idle();
    I2C2CONbits.PEN = 1;
    
    return;
}

void i2c_restart(void){
    
    i2c_wait_for_idle();
    I2C2CONbits.RSEN = 1;
    while(I2C2CONbits.RSEN == 1);
    
    return;
}

static void i2c_ack(void){
    
    i2c_wait_for_idle();
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN == 1);
    
    return;
}

static void i2c_nack(void){
    
    i2c_wait_for_idle();
    I2C2CONbits.ACKDT = 1;
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN == 1);
    
    return;
}

static uint16_t i2c_calc_brg(uint32_t baud){
    uint16_t retval = 0;
    double temp;
    
    if(baud > 0){
        temp = ((1/(2.0 * baud)) - 0.000000104) * PB_CLK_HZ;
        retval = (uint16_t)(temp - 2);
    }
    
    return retval;
}

// Acknowledge sequence not in progress
// Receive sequence not in progress
// Stop condition not in progress
// Repeated Start condition not in progress
// Start condition not in progress
// Master transmit is not in progress
static inline void i2c_wait_for_idle(void){
    
    while(I2C2CON & 0x1F);      
    while(I2C2STATbits.TRSTAT);
    
    return;
}
