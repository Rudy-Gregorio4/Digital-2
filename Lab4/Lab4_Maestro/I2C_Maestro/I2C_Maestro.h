/*
 * I2C_Maestro.h
 *
 * Created: 1/08/2024 16:18:31
 *  Author: esteb
 */ 



#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>

// Funcion para inicializar el Master
void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler);

// Funcion para inicializar el Slave con direccion
void I2C_Slave_Init(uint8_t data);

// Funcion para iniciar la comunicacion I2C
uint8_t I2C_Master_Start(void);

// Funcionn de parada de la comunicacion I2C
void I2C_Master_Stop(void);

// Funcion de transmision de datos del Master al Slave
/* Esta funcion devolvera un 0 si el Slave a recibido el dato */
uint8_t I2C_Master_Write(uint8_t dato);

// Funcion de recepcion de datos enviados por el esclavo al maestro
/*Esta funcion es para leer los datos que estan en el esclavo*/
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack);

// Funcion de recpecion de datos enviados por el Slave al Master
/* Funcion de recepcion con ACK*/
uint8_t I2C_Read_ACK(void);

#endif /* I2C_MASTER_H_ */