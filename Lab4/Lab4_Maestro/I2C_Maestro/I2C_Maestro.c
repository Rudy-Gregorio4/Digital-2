/*
 * I2C_Maestro.c
 *
 * Created: 1/08/2024 16:18:15
 *  Author: esteb
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include "I2C_Maestro.h"
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

void I2C_Slave_Init(uint8_t data){
	// PC4 -> SDA (Datos Seriales)
	// PC5 -> SCL (Reloj Serial)
	
	// Pines de Transmision como entrada
	DDRC &= ~((1<<DDC4)|(1<<DDC5));
	
	TWAR = data << 1 & 0b11111110;	// Se asigna la direccion que tendra
	//TWAR = (data<<1|0x01);	// Se asigna la direccion que tendra y habilita la llamada general
	
	// Habilita la interfaz, ACK automatico, Habilita ISR
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}


// Funcion para iniciar la comunicacion I2C
uint8_t I2C_Master_Start(void){
	uint8_t estado;
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Iniciar condicion de start
	while(!(TWCR & (1<<TWINT)));		// Espera a que termine la flag TWINT
	
	estado = TWSR & 0xF8;	// Verifica Estado
	if ((estado != 0x08) || (estado != 0x10))
	{
		return 1;
		} else{
		return estado;
	}
}

// Funcionn de parada de la comunicacion I2C
void I2C_Master_Stop(void){
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO);	// Inicia el envio de secuencia de parada
	while(TWCR & (1<<TWSTO));	//Esperamos a que el bit se limpie
}

// Funcion de transmision de datos del Master al Slave
/* Esta funcion devolvera un 0 si el Slave a recibido el dato */
uint8_t I2C_Master_Write(uint8_t dato){
	uint8_t estado;
	
	TWDR = dato;	// Carga el dato al registro
	TWCR = (1<<TWEN)|(1<<TWINT);	// Inicia el envio del dato
	
	while(!(TWCR & (1<<TWINT)));	// Espera al flag de twint
	estado = TWSR & 0xF8;	// Verifica el estado
	// Verificar si se transmitio una SLA + W con ACK, SLA + R con ACK
	if (estado == 0x18 || estado == 0x28 || estado == 0x40)
	{
		return 1;
		} else{
		return estado;
	}
	
}

// Funcion de recepcion de datos enviados por el esclavo al maestro
/*Esta funcion es para leer los datos que estan en el esclavo*/
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack){
	uint8_t estado;
	if (ack)
	{
		TWCR |= (1<<TWEA);	// Lectura con ack
		} else{
		TWCR &= ~(1<<TWEA);	// Lectura sin ack
	}
	
	TWCR |= (1<<TWINT);	// Iniciamos la lectura
	while(!(TWCR & (1<<TWINT)));	// Espera al flag twint
	estado = TWSR & 0xF8;	// Verificar estado
	
	//Verifica dato leido con ACK con o sin ACK
	if (estado == 0x58 || estado == 0x50)
	{
		*buffer = TWDR;
		return 1;
		} else{
		return estado;
	}
	
}

uint8_t I2C_Read_ACK(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); // Start TWI with ACK
	while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag set
	return TWDR; // Return received data
}

