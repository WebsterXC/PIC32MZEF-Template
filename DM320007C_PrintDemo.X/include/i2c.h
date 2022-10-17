#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>
#include <stdbool.h>

#define I2C_WRITE       0x00U
#define I2C_READ        0x01U

// I2C Module Return Codes
#define ERR_ACK_WAIT    0x01U
#define ERR_NACK_WAIT   0x02U
#define ERR_IDLE_WAIT   0x04U
#define ERR_BAD_ADDR    0x08U

#define I2C1            0x01U

void i2c_init(void);

void i2c_start(void);
void i2c_stop(void);

void i2c_read(uint8_t *, bool);
void i2c_write(uint8_t, bool);

#endif