/*
 * I2C.h
 *
 * Created: 14/08/2024 20:53:35
 *  Author: esteb
 */ 



#ifndef I2C_H_
#define I2C_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <stdbool.h>
#include <util/twi.h> // Biblioteca para definir los códigos de estado I2C

// Funcion para la configuracion del I2C Master
void I2C_Config_MASTER(uint8_t Prescaler, unsigned long SCL_Clock);

// Funcion para la configuracion del I2C Slave
void I2C_Config_SLAVE(uint8_t address);

// Funcion
void I2C_esclavo(uint8_t dato, uint8_t direction);  //Dato, direccion del esclavo

// Funcion para iniciar la comunicacion I2C
uint8_t I2C_Init();

// Funcion para escribir un byte de datos
uint8_t I2C_write_data(uint8_t dato);

// Funcion para parar la comunicacion I2C
void I2C_Stop();

// Funcion para leer un byte de datos desde el esclavo para el maestro.
uint8_t I2C_read_data(uint8_t direccion_esclavo, uint8_t *dato);



#endif /* I2C_H_ */