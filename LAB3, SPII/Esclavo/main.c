//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica Digital 2
// Autor: Rudy Gregorio
// Carné: 22127
// Proyecto: Laboratorio 3
// Hardware: Atmega238p
//******************************************************************************




// CÓDIGO DEL ESCLAVO

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "ADC/ADC.h"   // Incluir librería de ADC
#include "SPI/SPI.h"   // Incluir librería SPI

uint8_t valor1 = 0, valor2 = 0, alternar = 0; // Variables de procesamiento

void setup(void) {
	cli();           // Deshabilitar interrupciones
	DDRC = 0;        // Configurar puerto C como entrada
	SPI_init();      // Inicializar SPI
	SPCR |= (1 << SPIE); // Habilitar interrupción SPI
	initADC();       // Inicializar ADC
	sei();           // Habilitar interrupciones
}

int main(void) {
	setup();
	while (1) {
		ADCSRA |= (1 << ADSC);  // Iniciar conversión ADC
		_delay_ms(20);          // Retardo para evitar errores de procesamiento
	}
}

ISR(ADC_vect) {
	if (alternar == 0) {
		ADMUX = (ADMUX & ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0))); // Seleccionar canal ADC 1
		valor1 = ADCH;
		} else {
		ADMUX = (ADMUX & ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0))) | (1 << MUX0); // Seleccionar canal ADC 0
		valor2 = ADCH;
	}
	alternar = !alternar;
	ADCSRA |= (1 << ADIF); // Limpiar bandera de interrupción
}

ISR(SPI_STC_vect) {
	if (SPDR == 1) {
		SPDR = valor2;  // Enviar valor2 al maestro
		} else if (SPDR == 2) {
		SPDR = valor1;  // Enviar valor1 al maestro
	}
}
