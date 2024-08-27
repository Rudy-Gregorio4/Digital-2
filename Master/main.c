/*
 * Combined Master.c
 *
 * Created: 14/08/2024 21:12:56
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

// Incluye las librerías necesarias
#include "I2C/I2C.h"
#include "LCD8bits/LCD8bits.h"  // Cambié la librería por la nueva
#include "PWM/PWM1.h"

// Definición de direcciones I2C y constantes
#define Slave1 0x20  // Dirección del Slave 1 (Sensor Ultrasonico)
#define Slave2 0x08  // Dirección del Slave 2 (Sensor de Peso)
#define Slave3 0x29  // Dirección del Slave 3 (Sensor RGB)
#define TCS34725_ADDRESS 0x29 // Dirección I2C del sensor TCS34725
#define WEIGHT_THRESHOLD 100  // Umbral de peso para encender el LED

uint8_t dato1, dato2, dato3, status;  // Variables de almacenamiento de los Slaves
uint8_t gra, med, peq;
char buffer[32];  // Buffer para cadena de caracteres

// Prototipos de funciones adicionales
void TWI_init(void);
void TCS34725_init(void);
void TCS34725_configure(void);
void TCS34725_readRGB(uint16_t *r, uint16_t *g, uint16_t *b);
void LED_control(uint16_t r, uint16_t g, uint16_t b);
void UART_init(uint16_t ubrr);
void UART_transmit(char data);
void UART_transmit_string(const char* str);

// Función de configuración inicial
void setup(void) {
    cli();

    // Configuración de puertos
    DDRD = 0xFF;  // Puerto D como Output (LCD)
    DDRB = 0xFF;  // Puerto B como Output
    DDRC |= (1 << PC2)|(1 << PC0)|(1 << PC1)|(1 << PC3);  // Configura PC0-PC3 como salidas para LEDs

    // Configuración de UART, PWM, I2C y sensor TCS34725
    UCSR0B = 0;  // Deshabilita UART
    Lcd_Init8bits();  // Inicializar la LCD en modo 8 bits
    Lcd_Clear();  // Limpiar la pantalla LCD
    I2C_Config_MASTER(4, 200000);  // Prescaler = 4 , 200kHz de comunicación
    TWI_init();  // Inicializa el I2C
    UART_init(103);  // 9600 bps para 16MHz
    TCS34725_init();  // Inicializa el sensor RGB
    initPWM1A(no_invertido, 8, 39999);
    initPWM1B(no_invertido, 8, 39999);

    med = 0;
    peq = 0;
    gra = 0;

    sei();  // Habilitar interrupciones globales
}

int main(void)
{
	setup();
	uint16_t r, g, b;  // Variables para almacenar los valores RGB

	while (1)
	{
		_delay_ms(10);

		// Menú a mostrar en la LCD
		Lcd_Set_Cursor(0, 0);  // Posicionar el cursor en la primera línea, primer columna
		Lcd_Write_String("Peq: ");  // Escribir "Peq: "
		Lcd_Set_Cursor(0, 6);  // Mover el cursor a la columna 6
		Lcd_Write_String("Med: ");  // Escribir "Med: "
		Lcd_Set_Cursor(0, 12);  // Mover el cursor a la columna 12
		Lcd_Write_String("Gra: ");  // Escribir "Gra: "

		// Leer el valor del Slave 2 (sensor de peso)
		I2C_read_data(Slave2, &dato2);

		// Verificar si el peso es mayor al umbral establecido
		if (dato2 == 0) {
			UART_transmit('9');		//Enviar caracter "5" significando que si funcionó el servo
			PORTC |= (1 << PC2);  // Enciende el LED en PC2
		} else {
			PORTC &= ~(1 << PC2);  // Apaga el LED en PC2
			
		}

		// Comparaciones basadas en el valor numérico del Slave 1
		I2C_read_data(Slave1, &dato1);
		if (dato1 == 10)
		{
			peq = peq + 1;
			UART_transmit('4');
			_delay_ms(2000);
		}
		else if (dato1 == 8)
		{
			med = med + 1;
			UART_transmit('6');
			_delay_ms(2000);
		}
		else if (dato1 == 6)
		{
			gra = gra + 1;
			UART_transmit('8');
			_delay_ms(2000);
		}

		// Escribir los valores de peq, med, y gra en la LCD
		snprintf(buffer, sizeof(buffer), "%03d", peq);
		Lcd_Set_Cursor(1, 0);  // Posicionar el cursor en la segunda línea, primer columna
		Lcd_Write_String(buffer);  // Escribir el valor de "peq"

		snprintf(buffer, sizeof(buffer), "%03d", med);
		Lcd_Set_Cursor(1, 6);  // Mover el cursor a la columna 6
		Lcd_Write_String(buffer);  // Escribir el valor de "med"

		snprintf(buffer, sizeof(buffer), "%03d", gra);
		Lcd_Set_Cursor(1, 12);  // Mover el cursor a la columna 12
		Lcd_Write_String(buffer);  // Escribir el valor de "gra"

		// Leer los valores RGB del sensor
		TCS34725_readRGB(&r, &g, &b);

		// Controlar los LEDs y el servo basados en los valores RGB
		LED_control(r, g, b);

		_delay_ms(3000);  // Espera 3 segundos antes de la siguiente lectura
	}
}

// Implementación de las funciones TWI, UART e inicialización del sensor TCS34725...

void TWI_init(void) {
    TWSR = 0x00;  // Prescaler a 1
    TWBR = 0x48;  // Bit rate register (SCL freq = F_CPU/(16 + 2(TWBR)*prescaler))
    TWCR = (1<<TWEN);  // Habilitar el módulo TWI
}

void TWI_start(void) {
    TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);  // Enviar condición de inicio
    while (!(TWCR & (1<<TWINT)));  // Esperar a que se complete
}

void TWI_stop(void) {
    TWCR = (1<<TWSTO) | (1<<TWEN) | (1<<TWINT);  // Enviar condición de stop
}

void TWI_write(uint8_t u8data) {
    TWDR = u8data;
    TWCR = (1<<TWEN) | (1<<TWINT);  // Iniciar transmisión
    while (!(TWCR & (1<<TWINT)));  // Esperar a que se complete
}

uint8_t TWI_read_ack(void) {
    TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);  // Leer con ACK
    while (!(TWCR & (1<<TWINT)));  // Esperar a que se complete
    return TWDR;
}

uint8_t TWI_read_nack(void) {
    TWCR = (1<<TWEN) | (1<<TWINT);  // Leer sin ACK
    while (!(TWCR & (1<<TWINT)));  // Esperar a que se complete
    return TWDR;
}

// Función de inicialización del sensor TCS34725
void TCS34725_init(void) {
    TWI_start();
    TWI_write(TCS34725_ADDRESS << 1);
    TWI_write(0x80 | 0x00);  // Dirección del registro ENABLE
    TWI_write(0x01);  // Activar el sensor
    TWI_stop();

    _delay_ms(3);

    TWI_start();
    TWI_write(TCS34725_ADDRESS << 1);
    TWI_write(0x80 | 0x00);  // Dirección del registro ENABLE
    TWI_write(0x03);  // Habilitar ADC
    TWI_stop();

    // Configurar ganancia e integración
    TCS34725_configure();
}

// Función de configuración del sensor TCS34725
void TCS34725_configure(void) {
    TWI_start();
    TWI_write(TCS34725_ADDRESS << 1);
    TWI_write(0x80 | 0x01);  // Dirección del registro ATIME
    TWI_write(0xEB);  // Integración de 700 ms
    TWI_stop();

    TWI_start();
    TWI_write(TCS34725_ADDRESS << 1);
    TWI_write(0x80 | 0x0F);  // Dirección del registro CONTROL
    TWI_write(0x00);  // Ganancia 1x
    TWI_stop();
}

// Leer valores RGB del sensor TCS34725
void TCS34725_readRGB(uint16_t *r, uint16_t *g, uint16_t *b) {
    TWI_start();
    TWI_write(TCS34725_ADDRESS << 1);
    TWI_write(0x80 | 0x14);  // Dirección del registro de datos clear
    TWI_stop();

    TWI_start();
    TWI_write((TCS34725_ADDRESS << 1) | 0x01);  // Modo de lectura
    uint16_t clear = TWI_read_ack();
    clear |= ((uint16_t)TWI_read_ack()) << 8;

    *r = TWI_read_ack();
    *r |= ((uint16_t)TWI_read_ack()) << 8;

    *g = TWI_read_ack();
    *g |= ((uint16_t)TWI_read_ack()) << 8;

    *b = TWI_read_ack();
    *b |= ((uint16_t)TWI_read_nack()) << 8;
    TWI_stop();
}

// Control de LEDs basado en valores RGB
// Función para controlar los LEDs y el PWM según el color leído
void LED_control(uint16_t r, uint16_t g, uint16_t b) {
	PORTC &= ~(0x0F);  // Apagar todos los LEDs (PC0-PC3)


	// Rojo
	if (r > 900 && g < 800 && b < 800) {
		updateDutyCA1(50);  // Mover el servo a 50 grados
		//PORTC |= (1<<PC0);  // Enciende el LED rojo
		UART_transmit('1');  // Enviar '1' por UART
	}
	// Verde
	else if (g > 1100 && r < 1200 && b < 1200) {
		updateDutyCA1(130);  // Mover el servo a 130 grados
		//PORTC |= (1<<PC1);  // Enciende el LED verde
		UART_transmit('2');  // Enviar '2' por UART
	}
	// Azul
	else if (b > 600 && r < 800 && g < 800) {
		updateDutyCA1(255);  // Mover el servo a 255 grados
		//PORTC |= (1<<PC3);  // Enciende el LED azul
		UART_transmit('3');  // Enviar '3' por UART
	}
	
		// Blanco
	else  {
			//PORTC |= (1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3);  // Enciende todos los LEDs
		}
}


// Inicialización del UART
void UART_init(uint16_t ubrr) {
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << TXEN0);  // Habilitar transmisión
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // Asynchronous 8N1
}

// Transmisión de datos por UART
void UART_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));  // Esperar a que el buffer esté vacío
    UDR0 = data;  // Enviar el dato
}

// Transmisión de una cadena de caracteres por UART
void UART_transmit_string(const char* str) {
    while (*str) {
        UART_transmit(*str++);
    }
}
