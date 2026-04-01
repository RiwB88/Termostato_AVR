#define sbi(port, bit) (port) |= (1 << (bit))
#define cbi(port, bit) (port) &= ~(1 << (bit))
#include <SoftwareSerial.h>

byte Filtrar_teclado();

byte tecla;
void enc_max(){             //AÑADIR QUE SE ENCIENDA UN LED
  cbi(PORTD, PD0);      //Encendido inicial (pin 3)
  LCD_clear();
  leer_temp();
}

void apag(){
  //digitalWrite(3, HIGH);  // Apaga la bombilla
  sbi(PORTD, PD0);      //Apagar (pin 3)
  LCD_clear();
  leer_temp();
}

void poten(){
      unsigned long ultimo = 0;
      LCD_clear();    
      do{
      if(millis() - ultimo >= 1000){            //Si es necesario bajamos el 1000 para más frecuencia de refresco
        int valor = analogRead(A2);             //Leemos potenciómetro
        float potencia = map(valor, 0, 1023, 0, 100);   
        porc_leida(potencia);

        int temp = analogRead(A4);                                   //Leemos temperatura
        float grados = map(temp, 30, 850, 0, 100);       //Mapeo de valores para LDR (de 0 a 100)
        temp_leida(grados);
        ultimo = millis();
        }
      tecla = Leer_teclado_nf(); 
    }while(tecla != 1);
}


void leer_temp(){
      unsigned long ultimo = 0;
    do{
      if(millis() - ultimo >= 1000){            //Si es necesario bajamos el 1000 para más frecuencia de refresco
        analogRead(A4);     //Estabilizar el canal y así no lee A0  LO USO ya???
        int valor = analogRead(A4);
        //Añadir valores de sensores (bare-metal)
        //mapeo: 10mv/ºC + ACD(convertidor de Analógico-Digital son 0-1023) + (5v)
        //float grados = (valor*500.0) /1024;
        
        float grados = map(valor, 30, 850, 0, 100);       //Mapeo de valores para LDR (de 0 a 100)
        temp_leida(grados);
        ultimo = millis();
        }
      tecla = Leer_teclado_nf(); 
    }while(tecla != 1);
}


void manual(){                  //Establecemos primero un valor y luego lo ajusta no en "tiempo real"
    unsigned long ultimo = 0;
    int porcentaje = 50;

    delay(250);
    LCD_clear();
    print_pot();            //Print pantalla para escoger con botones
    camb_pot(50);
    tecla = Filtrar_teclado();
    
    while(tecla != 1 && tecla != 5){
      if(tecla == 2){        //ARRIBA
          if(porcentaje < 100){
            porcentaje++;
            camb_pot(porcentaje);
          }
         /* while(tecla == Repetir_tecla(tecla, 200)){
            if(porcentaje < 100){
              porcentaje++;
              camb_pot(porcentaje);

            }
          }*/

      }if(tecla == 3){       //ABAJO
          if(porcentaje > 0){
               porcentaje--;
               camb_pot(porcentaje);
            }
          /*while(tecla == Repetir_tecla(tecla, 200)){
            if(porcentaje < 100){
              porcentaje++;
              camb_pot(porcentaje);
          }*/
        
      }
          tecla = Filtrar_teclado();
          delay(100);
    }

    LCD_clear();
    porc_leida(porcentaje);

    //FALTA: que active al bombilla al porcentaje (PWM)

    while(tecla != 1){
      if(millis() - ultimo >= 1000){            //Si es necesario bajamos el 1000 para más frecuencia de refresco
       // analogRead(A4);                         //Estabilizar el canal y así no lee A0
        int valor = analogRead(A4);
        float grados = map(valor, 30, 850, 0, 100);       //Mapeo de valores para LDR (de 0 a 100)
        temp_leida(grados);
        ultimo = millis();
        }
      tecla = Leer_teclado_nf(); 
    }
}



int escoger_pot(){                             //Posible mejora: que recuerde potencia anterior
    int porcentaje = 50;
    tecla = Filtrar_teclado();
    while(tecla != 1 && tecla != 5){

      if(tecla == 2){        //ARRIBA
          if(porcentaje < 100){
            porcentaje++;
          }
          while(tecla == Repetir_tecla(tecla, 200)){
            if(porcentaje < 100){
              porcentaje++;
            }
          }

      }if(tecla == 3){       //ABAJO
          if(porcentaje > 0){
               porcentaje--;
            }
          while(tecla == Repetir_tecla(tecla, 200)){
            if(porcentaje < 100){
              porcentaje--;
          }
        }
      }

    }

    return porcentaje;
}


