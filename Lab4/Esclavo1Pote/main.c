/*
 * Esclavo1.c
 *
 * Created: 1/08/2024 17:48:42
 * Author : esteb
 */ 
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include "ADC/ADC.h"
#include "I2C_Slave/I2C_Slave.h"
#include <avr/interrupt.h>
#include <util/twi.h>
#define SLAVE_ADDRESS 0x20

volatile uint8_t adc_value1;

int main(void) {
    cli();  // Deshabilita interrupciones globales
    
    ADC_init(128);  // Inicializa ADC con un prescaler de 128
    ADC0();  // Selecciona el canal ADC0
    habilitar_conversion();  // Inicia la conversión del ADC
    
    I2C_Slave_Init(SLAVE_ADDRESS);  // Inicializa I2C en modo esclavo con la dirección definida
    sei();  // Habilita interrupciones globales
    
    while (1) {
        // Loop principal
    }
}

ISR(ADC_vect) {
    adc_value1 = ADC;  // Lee el valor del ADC
    habilitar_conversion();  // Inicia una nueva conversión
}

ISR(TWI_vect) {
    switch(TWSR & 0xF8) {
        case TW_SR_DATA_ACK:  // Data received and ACK returned
            // Manejo de datos recibidos si es necesario
            TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
            break;
        
        case TW_ST_SLA_ACK:  // Slave address received and ACK returned
        case TW_ST_DATA_ACK:  // Data transmitted and ACK received
            TWDR = adc_value1;  // Carga el valor del ADC para transmitir
            TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
            break;
        
        case TW_SR_STOP:  // Stop or repeated start condition received
            TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
            break;
        
        default:
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
            break;
    }
}
