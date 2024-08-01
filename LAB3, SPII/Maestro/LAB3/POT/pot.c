#include "POT.h"

int hundreds = 0, tens = 0, units = 0;
int pythonMode = 0, changeState = 0, lockState = 0;

char numberList[] = "0123456789"; // Lista de números a mostrar

// Activar o desactivar el modo Python
void usePython(uint8_t activate) {
	pythonMode = activate ? 1 : 0;
}

// Imprimir un número usando UART
void printNumber(int number) {
	writeUART(numberList[number]);
}

// Mostrar el valor en formato de centenas, decenas y unidades
void displayValue(uint8_t value) {
	hundreds = value / 100;
	tens = (value % 100) / 10;
	units = value % 10;

	printNumber(hundreds);
	printNumber(tens);
	printNumber(units);
}

// Mostrar los valores de los potenciómetros
void potentiometer(uint8_t potValue1, uint8_t potValue2) {
	if (pythonMode) {
		// Cambiar el estado de enclavamiento y el estado de cambio
		lockState = lockState ? lockState : (changeState = 2, 1);
		changeState = (changeState == 1) ? 2 : 1;
	}

	writeUART(10); // Enviar un enter

	if (!pythonMode) {
		writeTextUART("Potentiometer 1 value: ");
		changeState = 1;
		lockState = 0;
	}

	// Mostrar el valor del segundo potenciómetro si está en el estado adecuado
	if (changeState == 1) {
		displayValue(potValue2);
	}

	// Mostrar el valor del primer potenciómetro si está en el estado adecuado
	if (!pythonMode) {
		writeTextUART("Potentiometer 2 value: ");
	}

	if (changeState == 2 || !pythonMode) {
		displayValue(potValue1);
	}
}

// Mostrar el valor del contador
void counter(uint8_t countValue) {
	writeUART(10);  // Enviar un enter
	writeTextUART("Counter value: ");
	displayValue(countValue);
}
