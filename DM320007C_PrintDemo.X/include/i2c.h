#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>
#include <stdbool.h>

#define I2C_WRITE       0x00U
#define I2C_READ        0x01U

#define I2C_ACK         0x00U
#define I2C_NACK        0x01U

void i2c_init(void);

void i2c_start(void);
void i2c_stop(void);
void i2c_restart(void);

void i2c_read(uint8_t *, bool);
void i2c_write(uint8_t, bool);

uint8_t i2c_status(void);
void i2c_wait_for_ack(void);
void i2c_wait_for_nack(void);

#endif