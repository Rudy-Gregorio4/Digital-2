//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electr?nica Digital 2
// Autor: Rudy Gregorio
// Carn?: 22127
// Proyecto: Laboratorio 4
// Hardware: Atmega238p
// Creado: 04/04/2024
//******************************************************************************

// ESCLAVO 1, BITS CON LEDS

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "I2C_S/I2C_S.h" 

#define SLA_R(address) ((address << 1) | 1)
#define SLAVE_ADDRESS 0x22 // Direcci?n del esclavo

void setup(void);
void send_count_via_i2c(void);
// Funcion para la configuracion del I2C Slave
void I2C_Config_SLAVE(uint8_t address);

volatile int count = 0;

int main(void) {
    setup();
    
    while (1) {
        send_count_via_i2c();
		
    }
}

void setup(void) {
    cli();
    
    DDRB = 0b11111100;   // PB0 y PB1 como entrada
    PORTB = 0b00000011;  // Pull Up en PB0 y PB1
    
    DDRD = 0b11111111;   // Salida de LEDS
    PORTD = 0b00000000;  // LEDS apagado
    
    PCICR |= (1 << PCIE0); // Habilitar interrupciones de PCINT[7:0]
    PCMSK0 |=  (1 << PCINT1) | (1 << PCINT0); // Habilitar interrupci?n en PB1 y PB0
	
    PCICR |= (1<<PCIE0); // Mascara de interrupci?n
    UCSR0B = 0;
	
    I2C_Config_SLAVE(SLAVE_ADDRESS);
    
	sei(); // Activar interrupciones
}

ISR(PCINT0_vect) {
    
    if (!(PINB & (1 << PINB0))) {
        count++;
        if (count > 15) {
            count = 0;
        }
        PORTD = count;
        //send_count_via_i2c();
    } else if (!(PINB & (1 << PINB1))) {
        count--;
        if (count < 0) {
            count = 15;
        }
        PORTD = count;
        //send_count_via_i2c();
    }
}

void send_count_via_i2c(void) {
    I2C_start();
    I2C_write(SLAVE_ADDRESS << 1); // Enviar direcci?n del esclavo con bit de escritura
    I2C_write(count); // Enviar el valor del contador
    I2C_stop();
}


// Funcion para la configuracion del I2C Slave
void I2C_Config_SLAVE(uint8_t address){
	DDRC &= ~((1<<DDC4) | (1<<DDC5));
	
	address <<= 1;  //Ubica la dirección y
	address |= 0x01; //habilita para reconocer las llamadas generales de I2C
	TWAR = address;
	
	TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //Habilita el BUS, con reconocimiento e interrupción
}