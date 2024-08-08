#ifndef I2C_S_H
#define I2C_S_H

#include <avr/io.h>

// Declaraciones de funciones
void I2C_init(uint8_t address);
void I2C_start(void);
void I2C_stop(void);
void I2C_write(uint8_t data);

#endif // I2C_S_H
