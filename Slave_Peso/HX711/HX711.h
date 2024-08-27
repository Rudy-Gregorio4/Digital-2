#ifndef HX711_H
#define HX711_H

#include <avr/io.h>

// Pines de conexión del HX711
#define HX711_DOUT PD2  // Pin de datos (DOUT)
#define HX711_SCK PD3   // Pin de reloj (SCK)

// Funciones de la biblioteca HX711
void HX711_Init(void);
long HX711_Read(void);
void HX711_Set_Scale(float scale);
float HX711_Get_Scale(void);

#endif
