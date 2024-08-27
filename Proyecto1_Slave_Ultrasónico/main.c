/*
 * Proyecto1_Slave_Ultrasónico.c
 *
 * Created: 13/08/2024 22:49:39
 * Author : esteb
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/twi.h>

#include "I2C/I2C.h"


volatile uint8_t data = 0;
volatile uint16_t distancia = 0, tiempo = 0;


int main(void)
{
	cli();
	
	DDRD |= (1<<DDD3);	// Trigger como output
	DDRD &= ~(1<<DDD2);	// Echo como input
	
	PORTD &= ~((1<<PORTD2)|(1<<PORTD3));	// Ambos apagados
	
	TCCR1A = 0x00;
	TCCR1B |= (1<<CS11);	// Prescaler de 8
	
	I2C_Config_SLAVE(0x20);		// Configura direccion del Slave
	
	
	
	sei();
	while (1)
	{
		// Generamos el pulso del TRIG
		PORTD |= (1<<PORTD3);
		_delay_us(10);
		PORTD &= ~(1<<PORTD3);
		
		// Esperamos la entrada en echo
		while((PIND & (1<<PIND2))==0);
		TCNT1 = 0;
		while(PIND & (1<<PIND2));
		
		// Calculamos la distancia
		tiempo = TCNT1 * 0.5;
		distancia = tiempo/58.3;
	}
}


/****************************************************************************************
* Interrupciones ISR
****************************************************************************************/
ISR(TWI_vect){
	uint8_t state;
	
	// Lee el estado del bus de datos
	state = TWSR & 0xFC;
	
	switch(state){
		case 0x60:
		case 0x70:
		TWCR |= (1 << TWINT);
		break;
		case 0x80:
		case 0x90:
		data = TWDR;  // Recibió el dato, llamada general
		TWCR |= (1 << TWINT);
		break;
		case 0xA8: // SLA+R recibido, Master mode Lecture
		case 0xB8: // Dato transmitido y ACK recibido
		TWDR = distancia; // Carga el dato en el registro de datos
		TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA); // Listo para la próxima operación
		break;
		case 0xC0: // Dato transmitido y NACK recibido
		case 0xC8: // Último dato transmitido y ACK recibido
		TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		break;
		default: // Libera el BUS de datos
		TWCR |= (1 << TWINT) | (1 << TWSTO);
		break;
	}
}