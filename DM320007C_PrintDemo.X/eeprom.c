#include <xc.h>
#include "include/eeprom.h"
#include "include/i2c.h"

// Driver for Microchip 24LC32A over I2C.

// Read a single byte from an address in EEPROM.
void eeprom_read(uint16_t addr, uint8_t *data){
    uint8_t addr_hi;
    uint8_t addr_lo;
    uint8_t byte;
    
    if(addr > EEPROM_TOTAL_BYTES){
        addr = EEPROM_TOTAL_BYTES;
    }
    
    // Split 16-bit address into bytes.
    addr_hi = (addr & 0xFF00) >> 8;
    addr_lo = (addr & 0x00FF);
    
    // Perform read from memory over I2C.
    i2c_start();
    i2c_write((EEPROM_I2C_ADDR << 1) | I2C_WRITE, true);
    i2c_write(addr_hi, true);
    i2c_write(addr_lo, true);
    i2c_start();
    i2c_write((EEPROM_I2C_ADDR << 1) | I2C_READ, true);
    i2c_read(&byte, true);
    i2c_stop();
    
    *data = byte;
    
    return;
}

// Read bytes sequentially starting from an address in EEPROM.
void eeprom_read_seq(uint16_t addr, uint8_t *data, uint8_t bytes){
    uint8_t addr_hi;
    uint8_t addr_lo;
    int i;
    
    // Split 16-bit address into bytes.
    addr_hi = (addr & 0xFF00) >> 8;
    addr_lo = (addr & 0x00FF);
    
    // Perform sequential read over I2C.
    i2c_start();
    i2c_write((EEPROM_I2C_ADDR << 1) | I2C_WRITE, true);
    i2c_write(addr_hi, true);
    i2c_write(addr_lo, true);
    i2c_start();
    i2c_write((EEPROM_I2C_ADDR << 1) | I2C_READ, true);
    
    for(i = 0; i < bytes-1; i++){
        i2c_read(&data[i], false);
    }
    i2c_read(&data[i], true);
    i2c_stop();
    
    return;
}

// Write single byte to an address in EEPROM.
void eeprom_write(uint16_t addr, uint8_t data){
    uint8_t addr_hi;
    uint8_t addr_lo;
    
    if(addr > EEPROM_TOTAL_BYTES){
        return;
    }
    
    // Split 16-bit address into bytes.
    addr_hi = (addr & 0xFF00) >> 8;
    addr_lo = (addr & 0x00FF);
    
    // Perform single byte write over I2C.
    i2c_start();
    i2c_write((EEPROM_I2C_ADDR << 1) | I2C_WRITE, true);
    i2c_write(addr_hi, true);
    i2c_write(addr_lo, true);
    i2c_write(data, true);
    i2c_stop();
    
    return;
}

// Write up to 32 bytes sequentially starting from an address in EEPROM.
void eeprom_write_seq(uint16_t addr, uint8_t *data, uint8_t bytes){
    uint8_t addr_hi;
    uint8_t addr_lo;
    int i;
    
    if(addr > EEPROM_TOTAL_BYTES){
        return;
    }
    if(bytes > EEPROM_SEQ_BYTES){
        bytes = EEPROM_SEQ_BYTES;
    }
    
    // Split 16-bit address into bytes.
    addr_hi = (addr & 0xFF00) >> 8;
    addr_lo = (addr & 0x00FF);
    
    // Perform sequential byte write over I2C.
    i2c_start();
    i2c_write((EEPROM_I2C_ADDR << 1) | I2C_WRITE, true);
    i2c_write(addr_hi, true);
    i2c_write(addr_lo, true);
    
    for(i = 0; i < bytes; i++){
        i2c_write(data[i], true); 
    }
    i2c_stop();
    
    return;
}

// Erase (fill) data between start and end addresses. End byte not included.
void eeprom_erase(uint16_t start, uint16_t end){
    
    // TODO
    
    return;
}

// Delete (fill) data on entire EEPROM. The 24LC32A is organized into 32-byte 
// pages and we can't perform a write operation across boundaries. Split the 
// entire memory into sections and write each page with a fill.
void eeprom_flash(void){
    uint16_t address;
    uint8_t filldata[EEPROM_PAGE_SIZE];
    int i;
    
    // Fill a dummy array with fill codes.
    for(i = 0; i < EEPROM_PAGE_SIZE; i++){
        filldata[i] = EEPROM_ERASE_CODE;
    }
    
    // Now write each page.
    address = 0x0;
    for(i = 0; i < EEPROM_TOTAL_BYTES; i+=EEPROM_PAGE_SIZE){
        eeprom_write_seq(address, filldata, EEPROM_PAGE_SIZE);
        eeprom_wait_for_write_cycle();
        address += EEPROM_PAGE_SIZE;
    }
    
    return;
}

// Verify that the EEPROM is fully erased. Returns 0 on success, 1 otherwise.
int eeprom_verify(void){
    uint16_t address;
    uint8_t data[EEPROM_PAGE_SIZE];
    bool pageOK;
    int i, j;
    int retval = 0;
    
    pageOK = true;
    address = 0x0;
    for(i = 0; i < EEPROM_TOTAL_BYTES; i+=EEPROM_PAGE_SIZE){
        eeprom_read_seq(address, data, EEPROM_PAGE_SIZE);
        eeprom_wait_for_write_cycle();
        address += EEPROM_PAGE_SIZE;
        for(j = 0; j < EEPROM_PAGE_SIZE; j++){
            if(data[j] != EEPROM_ERASE_CODE){
                pageOK = false;
            }
        }
        
        if(pageOK == false){
            retval = 1;
            break;
        }
    }
    
    return retval;
}

// The 24LC32A actually takes a while to write data after receiving it. The
// datasheet specifies how to poll the I2C bus to determine when done. I needed
// to jump over my I2C driver on this one and touch registers - in the future 
// this capability should be baked into the I2C file.
void eeprom_wait_for_write_cycle(void){
    uint8_t status = 1;
    
    // Send start condition, write address, and see if an ACK or NACK came back.
    while(status == 1){
        i2c_start();
        i2c_write((EEPROM_I2C_ADDR << 1) | I2C_WRITE, false);
        //status = I2C2STATbits.ACKSTAT;
        status = i2c_status();
    }
    
    return;
}