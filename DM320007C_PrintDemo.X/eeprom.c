#include <xc.h>
#include "include/eeprom.h"
#include "include/i2c.h"

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
    i2c_write((EEPROM_I2C_ADDR << 1) || I2C_WRITE, true);
    i2c_write(addr_hi, true);
    i2c_write(addr_lo, true);
    i2c_start();
    i2c_write((EEPROM_I2C_ADDR << 1) || I2C_READ, true);
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
    i2c_write((EEPROM_I2C_ADDR << 1) || I2C_WRITE, true);
    i2c_write(addr_hi, true);
    i2c_write(addr_lo, true);
    i2c_start();
    i2c_write((EEPROM_I2C_ADDR << 1) || I2C_READ, true);
    
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
    i2c_write((EEPROM_I2C_ADDR << 1) || I2C_WRITE, true);
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
    i2c_write((EEPROM_I2C_ADDR << 1) || I2C_WRITE, true);
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
    uint16_t bytes;
    uint8_t fill[EEPROM_SEQ_BYTES];
    
    // Get total bytes and fill a dummy page of blank values.
    bytes = end - start;
    for(int i = 0; i < EEPROM_SEQ_BYTES; i++){
        fill[i] = EEPROM_SEQ_BYTES;
    }
    
    if(bytes > EEPROM_SEQ_BYTES){
        // TODO: Flash/Dynamic Erase
    }else{
        eeprom_write_seq(start, fill, bytes);
    }
    
    return;
}

// Delete (fill) data on entire EEPROM.
void eeprom_flash(void){
    
    eeprom_erase(0, EEPROM_TOTAL_BYTES);
    
    return;
}