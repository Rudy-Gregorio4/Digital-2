#include "SPI.h"

void SPI_init() {
	// Pines de comunicación
	// PB3: MOSI, PB4: MISO, PB5: SCK

	// Pin de control para esclavo
	// PB2: Esclavo 1

	DDRB |= (1 << DDB4);  // MISO como salida
	DDRB &= ~((1 << DDB2) | (1 << DDB3) | (1 << DDB5));  // SS, MOSI, y SCK como entradas

	// MSB enviado primero
	SPCR &= ~(1 << DORD);

	// Modo de comunicación: CPOL = 0, CPHA = 0
	SPCR &= ~(1 << CPOL);
	SPCR &= ~(1 << CPHA);

	// Pre-escalador: F_CPU/16 = 1MHz
	SPCR |= (1 << SPR0);
	SPCR &= ~(1 << SPR1);
	SPSR &= ~(1 << SPI2X);

	// SPI como esclavo
	SPCR &= ~(1 << MSTR);

	// Activar SPI
	SPCR |= (1 << SPE);
}
