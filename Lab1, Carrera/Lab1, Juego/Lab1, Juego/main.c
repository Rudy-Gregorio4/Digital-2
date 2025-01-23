//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica Digital 2
// Autor: Rudy Gregorio
// Carné: 22127
// Proyecto: Laboratorio 1
// Hardware: Atmega238p
// Creado: 04/04/2024
//******************************************************************************

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

void setup(void);
void multiplex(void);

volatile int player1 = 0;
volatile int player2 = 0;
volatile int count = 0;

const uint8_t display[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

void startRace() {
    for (int i = 5; i >= 0; i--) {
        PORTD = display[i];
        _delay_ms(1000);
    }
    PORTD = 0x00; // Apagar display después de conteo
    count = 0; // Iniciar contadores de jugadores en 0
    player1 = 0;
    player2 = 0;
    PORTB &= 0xE1; // Limpiar los bits PB1-PB4
    PORTC &= 0xF0; // Limpiar los bits PC0-PC3
}

int main(void) {
    setup();
    
    while (1) {
        if (count == 1) {
            startRace();
            count = 0; // Resetear la cuenta para no iniciar nuevamente sin presionar el botón
        }
        
        // Mostrar puntos de jugadores en el display (opcional, puedes cambiar esto)
        PORTD = display[player1];
        _delay_ms(500);
        PORTD = display[player2];
        _delay_ms(500);
    }
}

void setup(void) {
    cli();
    
    DDRB = 0b11111110;   // PB0 como entrada, PB1-PB4 como salidas
    PORTB = 0b00000001;  // Pull Up en PB0
    
    DDRD = 0b11111111;   // Salida de DISPLAY
    PORTD = 0b00000000;  // DISPLAY apagado
    
    DDRC = 0b00001111;   // PC0-PC3 como salidas, PC4-PC6 como entradas
    PORTC = 0b11110000;  // Pull Up en PC4-PC6
    
    PCMSK1 |= (1<<PCINT12)|(1<<PCINT13)|(1<<PCINT14); // PCINT12 (PC4), PCINT13 (PC5), PCINT14 (PC6)
    PCICR |= (1<<PCIE1);  // Mascara de interrupción para PCINT[14:8]
    
    sei(); // Activar interrupciones
}

ISR(PCINT1_vect) {
    if((PINC & (1<<PINC4)) == 0) {
        count++;
        if (count > 1) {
            count = 1;  // Asegurarse de no sobrepasar el valor de 1
        }       
    }
    
    if((PINC & (1<<PINC5)) == 0 && count == 0) {
        player1++;
        if (player1 > 9) {
            player1 = 9;   // Limitar el valor máximo a 9
        }
        PORTB = (PORTB & 0xE1) | (player1 << 1); // Actualizar PB1-PB4 con el valor de player1
    }
    
    if((PINC & (1<<PINC6)) == 0 && count == 0) {
        player2++;
        if (player2 > 9) {
            player2 = 9;   // Limitar el valor máximo a 9
        }
        PORTC = (PORTC & 0xF0) | player2; // Actualizar PC0-PC3 con el valor de player2
    }
}
