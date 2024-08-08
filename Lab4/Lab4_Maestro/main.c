/*
 * Lab4_Maestro.c
 *
 * Created: 1/08/2024 16:17:42
 * Author : esteb
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "I2C_Maestro/I2C_Maestro.h"
#include "LCD/LCD.h"


/*********************************************************************************************
* Declaracion de Variables
*********************************************************************************************/
uint8_t adcvalue = 0;	//Variable
uint8_t contador = 0;

/*********************************************************************************************
* Declaracion de Funciones
*********************************************************************************************/
void actualizarLCD(uint8_t val1, uint8_t val2);


int main(void)
{
	
	I2C_Master_Init(100000, 1);
	initLCD4bits();
	LCD_Set_Cursor(4, 1);
	LCD_Write_String("S1:");
	LCD_Set_Cursor(11, 1);
	LCD_Write_String("S2:");
	
	while (1)
	{
		
		// Lectura del primer Slave (Potenciometros) - Direccion (0x20)
		I2C_Master_Start();
		I2C_Master_Write(0x20 | TW_READ); //0x20 es la dirección base del primer esclavo, y TW_READ indica que se desea leer de ese esclavo.
		adcvalue = I2C_Read_ACK();
		I2C_Master_Stop();
		
		
		// Lectura del segundo Slave (Contador) - Direccion (0x22)
		I2C_Master_Start();
		I2C_Master_Write(0x22 | TW_READ); //0x20 es la dirección base del primer esclavo, y TW_READ indica que se desea leer de ese esclavo.
		contador = I2C_Read_ACK();
		I2C_Master_Stop();
		
		// Muestra los valores en la LCD
		actualizarLCD(adcvalue, contador);
		
		_delay_ms(500); // Espera 0.5s antes de la siguiente lectura
	}
}

/*********************************************************************************************
* Funciones
*********************************************************************************************/
void actualizarLCD(uint8_t val1, uint8_t val2) {
	// Limpiar pantalla
	LCD_CMD(0x01); // Clear display command
	
	// Escribir encabezados de sensores
	LCD_Set_Cursor(2, 1);
	LCD_Write_String("S1:");
	LCD_Set_Cursor(10, 1);
	LCD_Write_String("S2:");
	
	// Convertir los valores numéricos a cadenas
	char buffer1[5], buffer2[5];
	sprintf(buffer1, "%03d", val1); // Convierte el valor de adcvalue a una cadena
	sprintf(buffer2, "%03d", val2); // Convierte el valor de contador a una cadena
	
	// Mostrar los valores en la LCD
	LCD_Set_Cursor(2, 2);
	LCD_Write_String(buffer1);
	LCD_Set_Cursor(10, 2);
	LCD_Write_String(buffer2);
}