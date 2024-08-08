#include "I2C_S.h"

void I2C_init(uint8_t address) {
	// C?digo de inicializaci?n I2C
	TWAR = address << 1;
	TWCR = (1 << TWEN) | (1 << TWEA);
}

void I2C_start(void) {
	// C?digo para iniciar la comunicaci?n I2C
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
}

void I2C_stop(void) {
	// C?digo para detener la comunicaci?n I2C
	TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

void I2C_write(uint8_t data) {
	// C?digo para escribir datos en I2C
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
}
