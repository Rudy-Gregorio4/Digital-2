#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "UART/UART.h"
#include "SPI/SPI.h"
#include "POT/POT.h"

#define VREF 5.0 // Voltaje de referencia

volatile char receivedChar = 0;    // Variable para almacenar el valor del UART
char receivedNumber[4] = {0};      // Variable para almacenar el número recibido como cadena
uint8_t numIndex = 0;              // Índice para la cadena de número
uint8_t menu2 = 0;                 // Indicador para la opción 2

void setup(void) {
	cli();  // Desactivar interrupciones
	DDRD = 0xFF;  // Puerto D como salida
	DDRC = 0x03;  // Puerto C como salida

	initUART9600();  // Iniciar UART
	SPI_init();

	PORTD = 0x00;
	sei();  // Activar interrupciones
}

void showStartupMessage(void) {
	writeTextUART("\n\r*****Hello, how are you?*****");
	writeTextUART("\r\nEnter 1 to read potentiometers");
	writeTextUART("\r\nEnter 2 to input a 3-digit number\r\n");
}

void showVoltage(float voltage, char* buffer) {
	dtostrf(voltage, 4, 2, buffer); // 4 es el ancho total, 2 es la precisión
}

void processCommand(void) {
	if (menu2 == 1) {
		if (receivedChar >= '0' && receivedChar <= '9') {
			if (numIndex < 3) {
				receivedNumber[numIndex++] = receivedChar;
				char message[2] = {receivedChar, '\0'};
				writeTextUART(message); // Mostrar el carácter ingresado
			}
			if (numIndex == 3) {
				receivedNumber[numIndex] = '\0'; // Finaliza la cadena
				int result = atoi(receivedNumber); // Convierte la cadena a número
				if (result >= 0 && result <= 255) {
					PORTD = result & 0xFF; // Actualiza las LEDs en el puerto D (todos los bits)
					PORTC = (result >> 8) & 0x03; // Actualiza las LEDs en el puerto C (dos bits más significativos si fueran necesarios)
					char message[50];
					sprintf(message, "\r\nEntered number: %d\r\n", result);
					writeTextUART(message); // Mostrar el número ingresado
				}
				numIndex = 0; // Reinicia el índice
			}
			} else if (receivedChar == 'e') {
			menu2 = 0;
			showStartupMessage();
		}
		receivedChar = 0;
		} else {
		switch (receivedChar) {
			case '1':
			writeTextUART("Reading potentiometers...\r\n");
			SPI_slaveON(2);
			SPI_tx(1);
			uint8_t receivedData1 = SPI_rx();
			SPI_tx(2);
			uint8_t receivedData2 = SPI_rx();
			SPI_slaveOFF(2);

			// Mapeo de los valores del ADC a voltaje
			float voltage1 = (receivedData1 / 255.0) * VREF;
			float voltage2 = (receivedData2 / 255.0) * VREF;
			
			// Convertir a cadena con dtostrf
			char bufferVoltage1[8], bufferVoltage2[8];
			showVoltage(voltage1, bufferVoltage1);
			showVoltage(voltage2, bufferVoltage2);

			// Enviar los valores de voltaje a través de UART
			char voltageMessage[50];
			sprintf(voltageMessage, "Potentiometer 1 value: %sV\r\n", bufferVoltage1);
			writeTextUART(voltageMessage);
			sprintf(voltageMessage, "Potentiometer 2 value: %sV\r\n", bufferVoltage2);
			writeTextUART(voltageMessage);
			break;

			case '2':
			menu2 = 1;
			numIndex = 0; // Reinicia el índice al ingresar a la opción 2
			writeTextUART("Enter a 3-digit number (000-255) and press 'e' to return to the main menu:\r\n");
			break;
			case 'e':
			showStartupMessage();
			break;
			default:
			break;
		}
		receivedChar = 0;
	}
}

int main(void) {
	setup();
	showStartupMessage();
	while (1) {
		_delay_ms(5);
		if (receivedChar) processCommand();
	}
}

ISR(USART_RX_vect) {
	receivedChar = UDR0; // Almacena el carácter recibido
}
