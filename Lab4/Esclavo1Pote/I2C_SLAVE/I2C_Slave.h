/*
 * I2C_Slave.h
 *
 * Created: 1/08/2024 17:51:17
 *  Author: esteb
 */ 
#include <avr/io.h>

#ifndef I2C_SLAVE_H_
#define I2C_SLAVE_H_

// Funcion para inicializar el Slave con direccion
void I2C_Slave_Init(uint8_t data);

#endif /* I2C_SLAVE_H_ */