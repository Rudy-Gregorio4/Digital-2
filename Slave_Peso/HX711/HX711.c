#include "HX711.h"
#include <util/delay.h>

static float scale = 1.0;  // Escala de calibración

// Inicializa los pines del HX711
void HX711_Init(void) {
	// Configura los pines como salida/entrada
	DDRD |= (1 << HX711_SCK);  // SCK como salida
	DDRD &= ~(1 << HX711_DOUT); // DOUT como entrada

	// Asegúrate de que SCK esté en bajo inicialmente
	PORTD &= ~(1 << HX711_SCK);
}

// Lee un valor en bruto del HX711
long HX711_Read(void) {
	long value = 0;

	// Espera a que DOUT se ponga en bajo indicando que los datos están listos
	while (PIND & (1 << HX711_DOUT));

	// Lee 24 bits de datos
	for (uint8_t i = 0; i < 24; i++) {
		// Genera un pulso de reloj
		PORTD |= (1 << HX711_SCK);
		_delay_us(1);
		value = (value << 1) | ((PIND & (1 << HX711_DOUT)) ? 1 : 0);
		PORTD &= ~(1 << HX711_SCK);
		_delay_us(1);
	}

	// Pulso adicional para configurar el canal y ganancia
	PORTD |= (1 << HX711_SCK);
	_delay_us(1);
	PORTD &= ~(1 << HX711_SCK);
	_delay_us(1);

	// Convierte a entero con signo (formato de 24 bits a 32 bits)
	if (value & 0x800000) {
		value |= 0xFF000000;
	}

	return value;
}

// Establece la escala de calibración
void HX711_Set_Scale(float new_scale) {
	scale = new_scale;
}

// Devuelve la escala de calibración actual
float HX711_Get_Scale(void) {
	return scale;
}
