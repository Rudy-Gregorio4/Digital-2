#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "I2C/I2C.h"  // Asegúrate de incluir esta librería y configurarla adecuadamente

#define HX711_dout PD4
#define HX711_sck  PD5
#define LED_PIN    PB0
#define SLAVE_ADDRESS 0x08  // Dirección del esclavo

long escala = 1;
int peso_calibracion = 270;  // Peso referencial en gramos (270 gramos)
volatile uint8_t led_state = 0;  // Estado del LED (encendido o apagado)

void HX711_init(void);
long HX711_read(void);
void HX711_set_scale(long scale);
void HX711_tare(int times);
float HX711_get_units(int times);
void HX711_calibrate(int peso_referencial, int times);
void UART_init(void);
void UART_transmit(char data);
void UART_print(const char* str);
void UART_println(const char* str);
void float_to_string(float value, char* buffer);
void servo_init(void);
void servo_set_angle(int angle);

void HX711_init(void) {
	DDRD &= ~(1 << HX711_dout);
	DDRD |= (1 << HX711_sck);
	PORTD &= ~(1 << HX711_sck);
}

long HX711_read(void) {
	long count = 0;
	while (PIND & (1 << HX711_dout));
	for (int i = 0; i < 24; i++) {
		PORTD |= (1 << HX711_sck);
		count = count << 1;
		PORTD &= ~(1 << HX711_sck);
		if (PIND & (1 << HX711_dout)) count++;
	}
	PORTD |= (1 << HX711_sck);
	count ^= 0x800000;
	PORTD &= ~(1 << HX711_sck);
	return count;
}

void HX711_set_scale(long scale) {
	escala = scale;
}

void HX711_tare(int times) {
	long sum = 0;
	for (int i = 0; i < times; i++) {
		sum += HX711_read();
		_delay_ms(100);
	}
	escala = sum / times;
}

float HX711_get_units(int times) {
	long sum = 0;
	for (int i = 0; i < times; i++) {
		sum += HX711_read();
		_delay_ms(100);
	}
	return (float)(sum / times - escala);
}

void HX711_calibrate(int peso_referencial, int times) {
	long sum = 0;
	for (int i = 0; i < times; i++) {
		sum += HX711_read();
		_delay_ms(100);
	}
	long promedio = sum / times;
	escala = promedio - peso_referencial;

	UART_println("Calibración completa!");
	UART_print("Nueva escala: ");
	char buffer[10];
	sprintf(buffer, "%ld", escala);
	UART_println(buffer);
}

void UART_init(void) {
	unsigned int ubrr = F_CPU / 16 / 9600 - 1;
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void UART_print(const char* str) {
	while (*str) {
		UART_transmit(*str++);
	}
}

void UART_println(const char* str) {
	UART_print(str);
	UART_transmit('\r');
	UART_transmit('\n');
}

void float_to_string(float value, char* buffer) {
	int int_part = (int)value;
	int frac_part = (int)((value - int_part) * 100);

	if (frac_part < 0) {
		frac_part = -frac_part;
	}

	sprintf(buffer, "%d.%02d", int_part, frac_part);
}

void servo_init(void) {
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = 39999;
	DDRB |= (1 << PB1);
}

void servo_set_angle(int angle) {
	if (angle < 0) angle = 0;
	if (angle > 180) angle = 180;
	OCR1A = ((angle * 11) + 1000);
}

// Interrupción del TWI/I2C
ISR(TWI_vect) {
	uint8_t state;

	// Lee el estado del bus de datos
	state = TWSR & 0xFC;

	switch (state) {
		case 0x60:  // SLA+W recibido, ACK devuelto
		case 0x70:  // Dirección general recibida, ACK devuelto
		TWCR |= (1 << TWINT) | (1 << TWEA);
		break;

		case 0xA8:  // SLA+R recibido, ACK devuelto (el maestro quiere leer un dato)
		case 0xB8:  // Dato transmitido, ACK recibido
		led_state = (PINB & (1 << LED_PIN)) ? 1 : 0;  // Lee el estado del LED
		TWDR = led_state;  // Carga el estado del LED en el registro de datos
		TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
		break;

		case 0xC0:  // Dato transmitido, NACK recibido
		case 0xC8:  // Último dato transmitido, ACK recibido
		TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		break;

		default:  // Condición de error, libera el bus
		TWCR |= (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 << TWEA);
		break;
	}
}

int main(void) {
	HX711_init();
	UART_init();
	servo_init();
	DDRB |= (1 << LED_PIN);

	I2C_Config_SLAVE(SLAVE_ADDRESS);  // Configura el esclavo con la dirección especificada

	sei();  // Habilita interrupciones globales

	HX711_tare(20);
	HX711_calibrate(270, 20);

	while (1) {
		float peso = HX711_get_units(10);

		if (peso < 100) {
			PORTB |= (1 << LED_PIN);
			} else {
			PORTB &= ~(1 << LED_PIN);
		}

		if (peso > 100) {
			servo_set_angle(180);
			} else {
			servo_set_angle(0);
		}

		UART_print("Peso: ");
		char buffer[10];
		float_to_string(peso, buffer);
		UART_print(buffer);
		UART_println(" g");

		_delay_ms(1000);
	}

	return 0;
}
