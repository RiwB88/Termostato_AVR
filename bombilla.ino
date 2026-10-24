float tempFiltrada = 0;

#define sbi(port, bit) (port) |= (1 << (bit))
#define cbi(port, bit) (port) &= ~(1 << (bit))

byte Filtrar_teclado();
byte leerTecladoEstable();
byte tecla; 
void enc_max(){             //OPCIONAL: añadir que se encienda un led

  cbi(PORTD, PD0);      //Encendido inicial (pin 3)
  LCD_clear();
  leer_temp(); 

}

void apag(){
  //digitalWrite(3, HIGH);  // Apaga la bombilla
  sbi(PORTD, PD0);      //Apagar (pin 3)
  LCD_clear();
  
  leer_temp(); 

  //potencia = 0;
}

void poten(){
      unsigned long ultimo = 0;
      LCD_clear();    
      do{
      if(millis() - ultimo >= 1000){            //Si es necesario bajamos el 1000 para más frecuencia de refresco
        int valor = anal_digit(A2);             //Leemos potenciómetro
        
        float potenciometro = map(valor, 100, 930, 0, 100);  
        potencia = constrain(potenciometro,0,100); 
                //Serial.println(potencia);

        //porc_leida(potencia);

        float temp = leerTemperatura();                                   //Leemos temperatura
        //float grados = map(temp, 30, 850, 0, 100);       //Mapeo de valores para LDR (de 0 a 100)
        temp_leida_porc(temp,potencia);
        ultimo = millis();
        }
      tecla = leerTecladoEstable();
      //tecla = Leer_teclado_nf(); 
    }while(tecla != 1);
}


void leer_temp(){
      unsigned long ultimo = 0;
    do{
      anal_digit(A4);     //Estabilizar el canal y así no lee A0 
      float temp = leerTemperatura();                                   //Leemos temperatura 

      if(millis() - ultimo >= 1000){            //Si es necesario bajamos el 1000 para más frecuencia de refresco

        temp_leida(temp);

        ultimo = millis();
        }
      tecla = leerTecladoEstable();

    }while(tecla != 1);
}


float leerTemperatura() {
  long suma = 0;

  for (int i = 0; i < 16; i++) {  //Usamos 16 porque su máximo son 14 bits
    suma += anal_digit(A4);
  }

  float media = suma / 4.0;       //Luego lo dividimos entre 4 que es desplazar 2 bits, quedándonos con dos decimales y difiere

  float nueva = (media * 5.0 / 4095.0) * 100.0;   //Usamos 2^12 - 1 y no 1023 como sería normal porque tenemos 12 bits

  if (tempFiltrada == 0.0) tempFiltrada = nueva;

  tempFiltrada = tempFiltrada * 0.8 + nueva * 0.2; //Añadimos estabilidad usando el filtro EMA con factor de suavizado [α = 0.2]
  

  return tempFiltrada;
}


void manual(){                  //Establecemos primero un valor y luego lo ajusta no en "tiempo real"
    unsigned long ultimo = 0;
    int porcentaje = potencia;

    delay(250);
    LCD_clear();
    print_pot();            //Print pantalla para escoger con botones
    camb_pot(potencia);
    tecla = leerTecladoEstable();
    
    porcentaje = (int)escoger_double((double)potencia, 1.0, 100.0, "POTENCIA");

    if(tecla == 5){
      potencia = porcentaje;
      sbi(EIMSK,INT1);           //Activamos INT1
    }

    while(tecla != 1){
      if(millis() - ultimo >= 1000){            //Si es necesario bajamos el 1000 para más frecuencia de refresco
       // analogRead(A4);                         //Estabilizar el canal y así no lee A0
        float temp = leerTemperatura();                                   //Leemos temperatura
        //float grados = map(temp, 30, 850, 0, 100);       //Mapeo de valores para LDR (de 0 a 100)
        temp_leida_porc(temp, porcentaje);
        ultimo = millis();
        }
      tecla = leerTecladoEstable();
      //tecla = Leer_teclado_nf(); 
    }
}


int escoger_pot() {              // Puede mejorarse para recordar valor anterior
  int porcentaje = 50;
  byte tecla = 0;

  while (tecla != 1 && tecla != 5) {

    tecla = leerTecladoEstable();

    if (tecla == 2 && porcentaje < 100) {   // ARRIBA
      porcentaje++;
    }

    if (tecla == 3 && porcentaje > 0) {     // ABAJO
      porcentaje--;
    }

    delay(30);   // pequeño respiro, no bloqueante
  }

  return porcentaje;
}


void leer_temp_debug(){
      unsigned long ultimo = 0;
      int i = 0;
    do{
      if(millis() - ultimo >= 1000){            //Si es necesario bajamos el 1000 para más frecuencia de refresco
        anal_digit(A4);     //Estabilizar el canal y así no lee A0  LO USO ya???

        float temp = leerTemperatura();                                   //Leemos temperatura
        //float grados = map(temp, 30, 850, 0, 100);       //Mapeo de valores para LDR (de 0 a 100)
        temp_leida(temp);
        ultimo = millis();
        }
      tecla = leerTecladoEstable();
      //tecla = Leer_teclado_nf(); 
      i++;
      delay(1000);
    }while(i < 5);
}


