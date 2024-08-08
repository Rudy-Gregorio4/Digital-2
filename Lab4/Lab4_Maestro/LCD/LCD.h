/*
 * LCD.h
 *
 * Created: 1/08/2024 18:07:49
 *  Author: esteb
 */ 

#define F_CPU 16000000
#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include <util/delay.h>

#define E (1<<PORTC1)

//Funci�n para inicializar LCD en modo 4 bits
void initLCD4bits(void);

//Funci�n para colocar en el puerto un valor
void LCD_Port(char a);

//Funci�n para enviar un comando
void LCD_CMD(char a);

//Funci�n para enviar un caracter
void LCD_Write_Char(char c);

//Funci�n para enviar una cadena
void LCD_Write_String(char *a);

//Funci�n para desplazar a la derecha
void LCD_Shift_Right(void);

//Funci�n para desplazar a la izquierda
void LCD_Shift_Left(void);

//Funci�n para establecer el cursor
void LCD_Set_Cursor(char c, char f);

#endif /* LCD_H_ */