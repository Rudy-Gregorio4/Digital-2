/*
 * I2C_Slave.c
 *
 * Created: 1/08/2024 17:51:05
 *  Author: esteb
 */ 
#include <avr/io.h>

void I2C_Slave_Init(uint8_t data){
	// PC4 -> SDA (Datos Seriales)
	// PC5 -> SCL (Reloj Serial)
	
	// Pines de Transmision como entrada
	DDRC &= ~((1<<DDC4)|(1<<DDC5));
	
	TWAR = data << 1 & 0b11111110;	// Se asigna la direccion que tendra
	//TWAR = (address<<1|0x01);	// Se asigna la direccion que tendra y habilita la llamada general
	
	// Habilita la interfaz, ACK automatico, Habilita ISR
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}
/*
 * I2C_Maestro.c
 *
 * Created: 1/08/2024 16:18:15
 *  Author: esteb
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include "I2C_Slave.h"

void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler){
	// PC4 -> SDA (Datos Seriales)
	// PC5 -> SCL (Reloj Serial)
	
	// Pines de Transmision como entrada
	DDRC &= ~((1<<DDC4)|(1<<DDC5));
	
	// Seleccion del prescaler 1, 4, 16, 64 -> Preferencia Prescaler de '1'
	switch(Prescaler){
		case 1:
		TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
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
		TWSR |= (1<<TWPS1)|(1<<TWPS0);
		break;
		default:
		TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
		Prescaler = 1;
		break;
		
	}
	// Calcula el Bit Rate Register
	TWBR = ((F_CPU/SCL_Clock) - 16)/(2*Prescaler);
	TWCR |= (1<<TWEN);
}
