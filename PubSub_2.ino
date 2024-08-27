#include "config.h"

// Punteros a los feeds de Adafruit IO
AdafruitIO_Feed *Canalrgb = io.feed("rgb");
AdafruitIO_Feed *Canalpeso = io.feed("peso");
AdafruitIO_Feed *Canalultra = io.feed("ultra");

// Variables para almacenar los datos previos de los sensores
uint8_t dato_rgb_anterior = 0;
uint8_t dato_peso_anterior = 0;
uint8_t dato_ultra_anterior = 0;

// Variables para almacenar los datos actuales de los sensores
uint8_t dato_rgb;
uint8_t dato_peso;
uint8_t dato_ultra;

unsigned long startTime;

void setup() {
  Serial.begin(9600);  // Inicializa la comunicación serie
  while(! Serial);  // Espera hasta que la conexión serie esté lista
  Serial.print("Connecting to Adafruit IO");
  
  io.connect();  // Conecta el dispositivo a Adafruit IO
  
  startTime = millis();
  
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
    if (millis() - startTime > 30000) { // timeout de 30 segundos
      Serial.println("Error: No se pudo conectar a Adafruit IO");
      while(1); // bucle infinito para evitar que el dispositivo siga ejecutando el código
    }
  }
  
  Serial.println();
  Serial.println(io.statusText());  // Imprime el estado de la conexión
}

void loop() {
  io.run();  // Mantiene la conexión con Adafruit IO

  // Lee datos del puerto serie si hay disponibles
  while (Serial.available() > 0) {
    char caracter_recibido = Serial.read();  // Lee el caracter recibido
    Serial.print("Caracter recibido: ");  
    Serial.println(caracter_recibido);  // Envía el caracter a la UART

    if (caracter_recibido == '1' || caracter_recibido == '2' || caracter_recibido == '3') {
      dato_rgb = caracter_recibido;  // Usa el caracter para RGB
      Serial.print("RGB: ");
      Serial.println(dato_rgb);
      if (dato_rgb != dato_rgb_anterior) {
        delay(3000);  // Espera para que la señal se estabilice
        if(dato_rgb != dato_rgb_anterior) {
          Canalrgb->save(dato_rgb);
          dato_rgb_anterior = dato_rgb;
        }
      }
    }

    else if (caracter_recibido == '4' || caracter_recibido == '6' || caracter_recibido == '8') {
      dato_ultra = caracter_recibido;  // Usa el caracter para Ultra
      Serial.print("Ultra: ");
      Serial.println(dato_ultra);
      if (dato_ultra != dato_ultra_anterior) {
        delay(3000);  // Espera para que la señal se estabilice
        if(dato_ultra != dato_ultra_anterior) {
          Canalultra->save(dato_ultra);
          dato_ultra_anterior = dato_ultra;
        }
      }
    }

    else if (caracter_recibido == '5' || caracter_recibido == '0') {
      dato_peso = caracter_recibido;  // Usa el caracter para Peso
      Serial.print("Peso: ");
      Serial.println(dato_peso);
      if (dato_peso != dato_peso_anterior) {
        delay(3000);  // Espera para que la señal se estabilice
        if(dato_peso != dato_peso_anterior) {
          Canalpeso->save(dato_peso);
          dato_peso_anterior = dato_peso;
        }
      }
    }
  }
}
