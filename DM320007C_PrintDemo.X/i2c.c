#include <xc.h>
#include "include/i2c.h"

#define SYS_CLK_HZ          40000000U       // System Master Clock (Hz)
#define PB_CLK_HZ           10000000U       // Peripherial Bus Clock
#define I2Cx_DEFAULT_BAUD   400000U         // Target 400kHz Bus

static uint16_t i2c_calc_brg(uint32_t);
static inline void i2c_wait_for_idle(void);

static inline void i2c_restart(void);
static inline void i2c_ack(void);
static inline void i2c_nack(void);
static inline void i2c_wait_for_ack(void);
static inline void i2c_wait_for_nack(void);

void i2c_init(void){
    uint16_t brg;
    
    brg = i2c_calc_brg(I2Cx_DEFAULT_BAUD);
    
    I2C1CONCLR = 0xFFFFFFFF;
    I2C1BRG    = brg;
    I2C1CONSET = 0x8000;
    
    return;
}

void i2c_read(uint8_t *val, bool nack){
    
    if(val == NULL){
        return;
    }
    
    I2C1CONbits.RCEN = 1;
    while(I2C1CONbits.RCEN);
    while(!I2C1STATbits.RBF);
    *val = I2C1RCV;
    
    // Send either ACK or NACK.
    if(nack){
        i2c_nack();
    }else{
        i2c_ack();
    }
    
    return;
}

void i2c_write(uint8_t val, bool wait){
    
    I2C1TRN = val;
    while(I2C1STATbits.TBF == 1);
    
    i2c_wait_for_idle();

    if(wait == true){
        i2c_wait_for_ack();
    }
    
    return;
}

inline void i2c_wait_for_ack(void){
    
    while(I2C1STATbits.ACKSTAT == 1);
    
    return;
}

inline void i2c_wait_for_nack(void){
    
    while(I2C1STATbits.ACKSTAT == 0);
    
    return;
}

void i2c_start(void){
    
    i2c_wait_for_idle();
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN == 1);
    
    return;
}

void i2c_stop(void){
    
    i2c_wait_for_idle();
    I2C1CONbits.PEN = 1;
    
    return;
}

inline void i2c_restart(void){
    
    i2c_wait_for_idle();
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN == 1);
    
    return;
}

inline void i2c_ack(void){
    
    i2c_wait_for_idle();
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN == 1);
    
    return;
}

inline void i2c_nack(void){
    
    i2c_wait_for_idle();
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN == 1);
    
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
    
    while(I2C1CON & 0x1F);      
    while(I2C1STATbits.TRSTAT);
    
    return;
}
