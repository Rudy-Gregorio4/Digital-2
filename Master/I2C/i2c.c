/*
 * i2c.c
 *
 * Created: 16/08/2024 11:20:33
 *  Author: esteb
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <stdbool.h>
#include <util/twi.h> // Biblioteca para definir los códigos de estado I2C
#include "I2C.h"

uint8_t esclavo, dato, aux;

#define SLA_R(address) ((address << 1) | 1)



// Funcion para la configuracion del I2C Master
void I2C_Config_MASTER(uint8_t Prescaler, unsigned long SCL_Clock){
	DDRC &= ~((1<<DDC4) | (1<<DDC5));
	
	TWBR = ((F_CPU/SCL_Clock)-16)/(2*Prescaler);
	
	
	switch(Prescaler){
		case 1:
		TWSR &= ~((1<<TWPS1) | (1<<TWPS0));
		break;
		case 4:
		TWSR &= ~(1<<TWPS1);
		TWSR |= (1<<TWPS0);
		break;
		case 16:
		TWSR &= ~(1<<TWPS0);
		TWSR |= (1<<TWPS1);
		break;
		case 64:
		TWSR |= ~(1<<TWPS1) | (1<<TWPS0);
		break;
		default:
		TWSR &= ~((1<<TWPS1) | (1<<TWPS0));
		Prescaler = 1;
		break;
		
	}
	
	TWCR = 1 << TWEN;   //Habilita la interfaz
}

// Funcion para la configuracion del I2C Slave
void I2C_Config_SLAVE(uint8_t address){
	DDRC &= ~((1<<DDC4) | (1<<DDC5));
	
	address <<= 1;  //Ubica la dirección y
	address |= 0x01; //habilita para reconocer las llamadas generales de I2C
	TWAR = address;
	
	TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //Habilita el BUS, con reconocimiento e interrupción
}

// Funcion Dato, direccion del esclavo
void I2C_esclavo(uint8_t dato, uint8_t direction){
	esclavo = direction;  //Dirección del esclavo
	esclavo = esclavo << 1;    //Compone la SLA+W*************************************************************************
	
	
	aux = I2C_Init();   //Condicion de inicio
	if (aux != 0x01)  //Si no se establece la conexión
	{
		TWCR |= (1 << TWINT);  //Borrar la bandera
		return;   //No continua
	}
	
	aux = I2C_write_data(esclavo);     //Direcciona con la SLA+W
	if (aux != 0x01)   //Si hay algun error de comunicación
	{
		I2C_Stop();  //No continua, termina la comunicación y la cierra
		return;
	}
	
	I2C_write_data(dato);   //Envia el dato al esclavo
	I2C_Stop();  //No continua, termina la comunicación y la cierra
}

// Funcion para iniciar la comunicacion I2C
uint8_t I2C_Init(){
	uint8_t state; // Variable que indica que no obtuvo el bus
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);	// Condicion de inicio
	while (!(TWCR & (1<<TWINT)));	// Espera la bandera TWINT
	state = TWSR & 0xF8;	// Obtiene el estado, 1 condicion de inicio, 2 inicio repetido
	
	if (state == 0x08 || state == 0x10)
	{
		return 1;
	}
	
	return state;
}

// Funcion para escribir un byte de datos
uint8_t I2C_write_data(uint8_t dato){
	uint8_t state;
	
	TWDR = dato;	// Carga el dato al registro de datos
	TWCR = (1<<TWEN)|(1<<TWINT);	// Inicia el envio
	
	while (!(TWCR & (1<<TWINT)));	// Espera la bandera TWINT
	state = TWSR & 0xF8;	// Obtiene el estado
	
	// Hay 3 posibilidades de exito:
	// Transmitio una SLA + W con ACK
	// Transmitio una SLA + R con ACK
	// Transmitio un dato con ACK
	if (state == 0x18 || state == 0x28 || state == 0x40)
	{
		return 0x01;
	}
	
	return state;	// So hay algun error
}

// Funcion para parar la comunicacion I2C
void I2C_Stop(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// Condicion de paro
	while(TWCR & (1<<TWSTO));	// El bit se limpia por HW
}

// Funcion para leer un byte de datos desde el esclavo para el maestro.
uint8_t I2C_read_data(uint8_t direccion_esclavo, uint8_t *dato){
	// Iniciar concdicion de START
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
	while(!(TWCR & (1<<TWINT)));	// Esperar a que se complete
	
	// Verifica la condicion de start
	if ((TWSR & 0xF8) != TW_START)
	{
		return 1;	// No se pudo iniciar la condicion de start
	}
	
	// Enviar direccion del esclavo con bit de lectura
	TWDR = SLA_R(direccion_esclavo);
	TWCR = (1<<TWEN)|(1<<TWINT);
	while(!(TWCR & (1<<TWINT)));	// Espera a que se complete la transmision
	
	// Verifica ACK/NACK de la direccion
	if ((TWSR & 0xF8) == TW_MR_SLA_NACK)
	{
		I2C_Stop();	// Detiene la transmision si es un NACK
		return 1;
	} else if ((TWSR & 0xF8) != TW_MR_SLA_ACK)
	{
		I2C_Stop();	// Detener si recibe otro dato erroneo
		return 1;
	}
	
	// Lee el byte del Slave
	TWCR = (1<<TWEN)|(1<<TWINT);
	while(!(TWCR & (1<<TWINT)));	// Esperar a que se complete la recepcion
	
	// Verifica la recepcion
	if ((TWSR & 0xF8) != TW_MR_DATA_NACK && (TWSR & 0xF8) != TW_MR_DATA_ACK)
	{
		I2C_Stop();	// Detener la recepcion
		return 1;
	}
	
	// Lee dato recibido
	*dato = TWDR;
	
	I2C_Stop();
	
	return *dato;
}