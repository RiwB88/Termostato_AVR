#include <PID_v1.h>

#define sbi(port, bit) (port) |= (1 << (bit))
#define cbi(port, bit) (port) &= ~(1 << (bit))

void logCSV(unsigned long tiempo, float tempReal, float tempObj, int salidaPID);

volatile int potencia = 0;                 

volatile int ult_cero = 0;
volatile int zcd_count = 0;
volatile int triac_count = 0;

double Kp = 1.5;
double Ki = 0.15;
double Kd = 10;

long n_ticks = 19500;   

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,Kp,Ki,Kd, DIRECT);
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


void setup() {
  // put your setup code here, to run once:

  //delay(3000);//Le damos tiempo al usb

 sbi(DDRB, DDB4);     //pinMode(8, OUTPUT);
 sbi(DDRB, DDB5);     //pinMode(9, OUTPUT);
 sbi(DDRD, DDD4);     //pinMode(4, OUTPUT);
 sbi(DDRC, DDC6);     //pinMode(5, OUTPUT);
 sbi(DDRD, DDD7);     //pinMode(6, OUTPUT);
 sbi(DDRE, DDE6);     //pinMode(7, OUTPUT);

 sbi(DDRD, DDD0);      //pinMode(3, OUTPUT); Capaz cambiaría a pin 11 por OC1C
 cbi(DDRD, DDD1);      //pinMode(2, INPUT);
 cbi(DDRF, DDF7);      //pinMode(A0, INPUT);
 cbi(DDRF, DDF1);      //pinMode(A4, INPUT);
 cbi(DDRF, DDF5);      //pinMode(A2, INPUT);


/*******Timers********/
  //cli();
//Inicializamos Timer1 para interrupción
  TCCR1A = 0;
  TCCR1B = 0;
//Metemos CTC
  TCCR1B |= (1 << WGM12);  

//Establecemos prescaler(8) -> 010
  TCCR1B |= (1 << CS11);  

//OCR1A = n_ticks_semiciclo * (100 - potencia) / 100
  long pwm = n_ticks * (100 - potencia) / 100;
  OCR1A = pwm;

//Inicializar registro para ADC
  ADCSRA = (1 << ADEN)   //Habilitar ADC
       | (1 << ADPS2)  //Activar prescaler a 128
       | (1 << ADPS1)
       | (1 << ADPS0);


//Preparamos interrupción en el pin 2 (int1)
  EICRA = 0;
  cbi(EIMSK,INT1);           //En la documentación nos piden que desactivemos la interrupción para cambiar EICRA
  sbi(EICRA,ISC10);       //Aquí configuramos que sea CHANGE, es así para que el TRIAC no se vea afectado en medio ciclo (en medio del paso por 0 se apaga)
  sbi(EIMSK,INT1);           //Activamos INT1

  //sei();
/*********************/

  Input = (anal_digit(A4) * 5.0 / 1023.0) * 100.0;
  Setpoint = 50;
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1000);
  myPID.SetOutputLimits(0,100);   //Hacemos que el rango del output sea distinto

  Serial.begin(9600);

/*-*-*-*-*-*-Preparación del CSV (PID)-*-*-*-*/
/*
  Serial.println("# PID Termostato");
  Serial.print("# Kp="); Serial.println(Kp);
  Serial.print("# Ki="); Serial.println(Ki);
  Serial.print("# Kd="); Serial.println(Kd);
  Serial.println("tiempo_ms,temp_real,temp_objetivo,salida_pid");
*/
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

  LCD_Reset();
  LCD_home();
  delay(1000);

  sbi(PORTD, PD0);  // Bombilla apagada por defecto

  LCD_print("Iniciando menu...");
  delay(1000);
  

}

ISR(INT1_vect) {
    zcd_count++;

    if (TIMSK1 & (1 << OCIE1A)) return; //Filtro para el ZCD: si está habilitada la interrupción del Timer entonces saltamos interr. de ZCD

    TCNT1 = 0;                          //Reinciamos timer1
    if (potencia == 0) {      //Apagamos bombilla
        sbi(PORTD, PD0); 
        return;
    }

    long pwm = (n_ticks * (100 - potencia)) / 100;
    pwm = map(pwm, 0, 19750, 3600, 19750);

    OCR1A = (unsigned int)pwm;

    TIFR1 |= (1 << OCF1A);  // Limpiar cualquier interrupción pendiente  ¡=> OCFnA can be cleared by writing a logic one to its bit location.
    TIMSK1 |= (1 << OCIE1A); 

}

ISR(TIMER1_COMPA_vect) {
    triac_count++;

    cbi(PORTD, PD0);       
    delayMicroseconds(100); //Cambiar delay?
    sbi(PORTD, PD0);       

    //Deshabilitamos interrupción del Timer
    TIMSK1 &= ~(1 << OCIE1A); 
}

unsigned long ultimoLog = 0;    //Para el CSV


void loop() {
  // put your main code here, to run repeatedly:
    //Serial.println(anal_digit(A0));

 menu_principal();

// Bombilla encender-apagar
/*
  LCD_clear();
  LCD_print("Apagando...");
  apag();
  delay(3000);
  LCD_clear();
  LCD_print("Encendiendo...");
  enc_max();
  delay(3000);
 

//Intensidad Bombilla
/*
  Serial.println("-------");
   LCD_clear();
      potencia = 10;   // muy tenue
      LCD_print("tenue...");
    delay(3000);
     Serial.println(OCR1A);
    potencia = 50;   // medio
      LCD_print("medio...");
    delay(3000);
      Serial.println(OCR1A);
      LCD_print("maxim...");
    potencia = 80;   // casi máximo
    delay(3000);
      Serial.println(OCR1A);





//n_ticks
/*    
  LCD_clear();
  //LCD_print("Probando...");
  LCD_setCursor(0x00);
  Serial.println("19300: "); 
      n_ticks = 19300;   // muy tenue
      LCD_print("19300...");
    leer_temp_debug();
   // delay(3000);
 LCD_setCursor(0x00);
  Serial.println("19500: "); 
      n_ticks = 19500;   // muy tenue
      LCD_print("19500...");
      leer_temp_debug();
   // delay(3000);
    LCD_setCursor(0x00);
  Serial.println("19600: "); 
      n_ticks = 19600;   // muy tenue
      LCD_print("19600...");
      leer_temp_debug();
   // delay(3000);
*/


//n_int_zcd_triac
/*
    Serial.print("ZCD: "); 
    Serial.println(zcd_count);
    Serial.print("TRIAC: "); 
    Serial.println(triac_count);
    Serial.print("OCR1A: ");
    Serial.println(OCR1A);  
    delay(1000);
  */
}



