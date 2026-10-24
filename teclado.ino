#include <SoftwareSerial.h>

#define T1_MIN 0
#define T1_MAX 120

#define T2_MIN 121
#define T2_MAX 280

#define T3_MIN 281
#define T3_MAX 420

#define T4_MIN 421
#define T4_MAX 550

#define T5_MIN 551
#define T5_MAX 850

#define LIBRE 850



byte leerTecla() {
    anal_digit(A0);
  int valor = anal_digit(A0);


  if (valor > LIBRE) return 0;
  if (valor >= T5_MIN && valor <= T5_MAX) return 5;
  if (valor >= T4_MIN && valor <= T4_MAX) return 4;
  if (valor >= T3_MIN && valor <= T3_MAX) return 3;
  if (valor >= T2_MIN && valor <= T2_MAX) return 2;
  if (valor >= T1_MIN && valor <= T1_MAX) return 1;

  return 0;
}

byte Filtrar_teclado() {
  byte teclaActual = 0;
  byte teclaAnterior = 0;
  int contador = 0;

  while (true) {
    teclaActual = leerTecla();

    if (teclaActual == teclaAnterior && teclaActual != 0) {
      contador++;
      if (contador >= 5) return teclaActual;
    } else {
      contador = 0;
    }

    teclaAnterior = teclaActual;
    delay(5);
  }
}

bool repetirTecla(byte tecla, unsigned long tiempo) {
  unsigned long inicio = millis();

  while (millis() - inicio < tiempo) {
    if (leerTecla() != tecla) return false;
  }
  return true;
}



byte leerTecladoEstable() {
  static byte teclaAnterior = 0;
  static unsigned long tCambio = 0;
  static unsigned long tRepeticion = 0;
  unsigned long mantenido = 0;
  unsigned long intervalo;

  byte teclaActual = leerTecla();   // lectura analógica simple
  unsigned long ahora = millis();

  if (teclaActual != teclaAnterior) {
    tCambio = ahora;
    teclaAnterior = teclaActual;

    return 0;
  }
  mantenido = ahora - tCambio;

  if (mantenido < 82)
    return 0; 
  
  if (teclaActual == 0) return 0;


  if      (mantenido < 1000)  intervalo = 500;  
  else if (mantenido < 2000)  intervalo = 300;  
  else if (mantenido < 3000)  intervalo = 150;   
  else                        intervalo = 80;   

  if((ahora - tRepeticion) >= intervalo) {
    Serial.println(teclaActual);
    tRepeticion = ahora;
    return teclaActual;
  }

  return 0;
}

unsigned int anal_digit(uint8_t canal) {
  uint8_t ch;
  switch(canal) {
    case A0: ch = 7; break;
    case A1: ch = 6; break;
    case A2: ch = 5; break;
    case A3: ch = 4; break;
    case A4: ch = 1; break;
    case A5: ch = 0; break;
    default: ch = canal & 0x0F; break;
  }

  ADMUX = (1 << REFS0) | (ch & 0x0F);
  sbi(ADCSRA, ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}




