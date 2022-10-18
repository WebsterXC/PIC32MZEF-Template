#ifndef _EEPROM_H
#define _EEPROM_H

#define EEPROM_TOTAL_BYTES  4096U    
#define EEPROM_PAGE_SIZE    32U
#define EEPROM_I2C_ADDR     0x50
#define EEPROM_SEQ_BYTES    32U
#define EEPROM_ERASE_CODE   0xFF

void eeprom_read(uint16_t, uint8_t *);
void eeprom_read_seq(uint16_t, uint8_t *, uint8_t);

void eeprom_write(uint16_t, uint8_t);
void eeprom_write_seq(uint16_t, uint8_t *, uint8_t);

void eeprom_erase(uint16_t, uint16_t);
void eeprom_flash(void);
int eeprom_verify(void);

void eeprom_wait_for_write_cycle(void);

#endif