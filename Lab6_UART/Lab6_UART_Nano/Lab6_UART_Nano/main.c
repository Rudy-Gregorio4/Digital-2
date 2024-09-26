//***************************
// Universidad del Valle de Guatemala
// Electrónica Digital 2
// Archivo: control_push_lab6
// Hardware: ATMEGA328P
// Autor: Rudy Gregorio
// Carnet: 22127
//***************************

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Prototipos
void setup(void);
void UART_init(unsigned int ubrr);
void UART_transmit(unsigned char data);
void debounce(void);

int main(void) {
    cli();    // Deshabilitar interrupciones globales
    setup();  // Configurar pines y UART
    UART_init(103);  // Inicializar UART a 9600 baudios
    sei();    // Habilitar interrupciones globales

    while (1) {
        // Loop principal vacío, ya que todo lo maneja la interrupción
    }
}

void setup(void) {
    // Configurar botones PB0-PB4 como entradas con pull-up
    DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4)); 
    PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4);

    // Configurar el botón en PD7 como entrada con pull-up
    DDRD &= ~(1 << DDD7); 
    PORTD |= (1 << PORTD7);

    // Habilitar PCINT0 para PB0-PB4 y PCINT23 para PD7
    PCICR |= (1 << PCIE0) | (1 << PCIE2);
    PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4);
    PCMSK2 |= (1 << PCINT23);  // Habilitar interrupción para PD7
}

void UART_init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0);    // Habilitar transmisor UART
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // Modo 8-bit
}

void UART_transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));  // Esperar a que el buffer esté libre
    UDR0 = data;  // Enviar dato
}

void debounce(void) {
    _delay_ms(50);
}

ISR(PCINT0_vect) {
    debounce();
    if (!(PINB & (1 << PINB0))) {
        UART_transmit('U');  // PB0 -> 'U'
    }
    if (!(PINB & (1 << PINB1))) {
        UART_transmit('D');  // PB1 -> 'D'
    }
    if (!(PINB & (1 << PINB2))) {
        UART_transmit('L');  // PB2 -> 'L'
    }
    if (!(PINB & (1 << PINB3))) {
        UART_transmit('R');  // PB3 -> 'R'
    }
    if (!(PINB & (1 << PINB4))) {
        UART_transmit('A');  // PB4 -> 'A'
    }
}

ISR(PCINT2_vect) {
    debounce();
    if (!(PIND & (1 << PIND7))) {
        UART_transmit('B');  // PD7 -> 'B'
    }
}