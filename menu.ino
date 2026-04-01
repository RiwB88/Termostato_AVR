byte Filtrar_teclado();

typedef enum {
  ENCENDIDO = 0,
  APAGADO = 1,
  MANUAL = 2,
  TERMOSTATO = 3,
  POTENCIOMETRO = 4
}modos;


void menu_principal(){

/************Inicio**************/

  // LCD_Reset();
  // LCD_home();
  // delay(1000);

  // LCD_print("Iniciando menu...");
  // delay(1000);

/************MENÚ**************/
bool lista_main = true;        //Llamo main a la página 1 (lis_1)
bool reset = false;
int ind_lin = 0;

byte tecla;
byte linea[4] = {0x12, 0x52, 0x26, 0x66};

  LCD_clear();

  lis_1(linea[ind_lin]);

while(true){

  tecla = Filtrar_teclado();

  if(tecla == 3){   //Botón abajo
      if(ind_lin != 3){
        ind_lin++;
        if(lista_main){
          lis_1(linea[ind_lin]);
        }else{
          lis_2(linea[ind_lin]);
        }
      }else{                      //ind_lin = 3
        if(lista_main){
          lis_2(linea[ind_lin]);
          lista_main = false;
        }else{
          ind_lin = 0;
          lis_1(linea[ind_lin]);
          lista_main = true;
        }
      }
  }
  else if(tecla == 2){                   //Botón arriba
       if(ind_lin != 0){
        ind_lin--;
        if(lista_main){
          lis_1(linea[ind_lin]);
        }else{
          lis_2(linea[ind_lin]);
        }
      }else{                      //ind_lin = 0
        if(lista_main){
          ind_lin = 3;
          lis_2(linea[ind_lin]);
          lista_main = false;
        }else{
          ind_lin = 0;
          lis_1(linea[ind_lin]);
          lista_main = true;
        }
      } 
  }
   else if(tecla == 5){                 //Botón OK
   int modo = ind_lin;
      if(lista_main){
        ir_a(modo);
        reset = true;
      }else{
        modo++;
        ir_a(modo);
        reset = true;
      }
   }
  if(!reset){
    Teclado_libre();
    }else{
      reset = false;
      ind_lin = 0;
      lis_1(linea[ind_lin]);
      Teclado_libre();
    }
  delay(200);
}

  delay(10000);
}


void lis_1(byte pos){
LCD_clear();

  LCD_setCursor(pos);
  LCD_print("<-");

  LCD_setCursor(0x00);
  LCD_print("1 ENCENDIDO");

  LCD_setCursor(0x40);
  LCD_print("2 APAGADO");

  LCD_setCursor(0x14);
  LCD_print("3 CTL MANUAL");

  LCD_setCursor(0x54);
  LCD_print("4 TERMOSTATO");


}

void lis_2(byte pos){
LCD_clear();

  LCD_setCursor(pos);
  LCD_print("<-");

  LCD_setCursor(0x00);
  LCD_print("2 APAGADO");

  LCD_setCursor(0x40);
  LCD_print("3 CTL MANUAL");

  LCD_setCursor(0x14);
  LCD_print("4 TERMOSTATO");

  LCD_setCursor(0x54);
  LCD_print("5 POTENCIOMETRO");


}

void ir_a(int modo){
  switch(modo){
  case ENCENDIDO:
  //En modo Encendido: Temperatura leída por el sensor. 
  //Enciende el agente calefactor suministrando el 100% de energía.
  enc_max();
  //debug_sel("encendido");
  break;
  case APAGADO:
  //En modo Apagado: Temperatura leída por el sensor. 
  //Apagar: Apaga el agente calefactor.
  apag();
  //debug_sel("Apagando");
  break;
  case MANUAL:
  //En modo Control manual: Temperatura leída por el sensor y porcentaje de funcionamiento del agente calefactor. 
  //Control manual: Establece el porcentaje de funcionamiento dal agente calefactor y regula la energía suministrada.
  manual();
  //debug_sel("manual");
  break;
  case TERMOSTATO:
  //En Modo Termostato: Temperatura leída por el sensor, temperatura objetivo y porcentaje de funcionamiento del agente calefactor. 
  //Termostato: Regula la energía suministrada al agente calefactor manteniendo la temperatura objetivo establecida. (PID )
  debug_sel("termostato");
  break;
  case POTENCIOMETRO:
  //En modo Potenciómetro: Temperatura leída por el sensor y porcentaje de funcionamiento del agente calefactor. 
  //Potenciómetro: Regula la energía sumimistrada al agente calefactor por medio del potenciómetro.
  poten();
  //debug_sel("potenciometro");
  break;
  default:
  LCD_clear();
  LCD_home();
  LCD_print("ERROR-selecciona opc valida");
  delay(2000);
  menu_principal();
  break;
  }
}

void debug_sel(char* str){    //Permite ver si se hace la selección correcta del menú al pulsar OK
  LCD_clear();
  LCD_home();
  LCD_print(str);
  delay(2000);
  menu_principal();
}

void temp_leida(float val){
  char buf[24];
  char temp_str[8];   
  dtostrf(val, 6, 2, temp_str);     //No se puede usar %f directamente en sprintf
  sprintf(buf, "Temp = %s", temp_str);
   //LCD_clear();
   LCD_setCursor(0x41);
   LCD_print(buf); //sprintf
   //delay(1000);
}

void porc_leida(int por){
  char buf[24];
  sprintf(buf, "Pote =  %d %%", por);
   //LCD_clear();
   LCD_setCursor(0x15);
   LCD_print(buf); //sprintf
   //delay(1000);
}

void print_pot(){
  LCD_setCursor(0x00);
  LCD_print("   ///// + /////");
  LCD_setCursor(0x40);
  LCD_print("Escoja potencia =");
  LCD_setCursor(0x14);
  LCD_print("   ///// - /////");
}

void camb_pot(int val){
  char buf[24];
  sprintf(buf, "Escoja potencia =%d", val);
  LCD_setCursor(0x40);
  LCD_print(buf);

}
/*   LCD_setCursor(0x00);
  LCD_print("1 ENCENDIDO");

  LCD_setCursor(0x40);
  LCD_print("2 APAGADO");

  LCD_setCursor(0x14);
  LCD_print("3 CTL MANUAL");

  LCD_setCursor(0x54);
  LCD_print("4 TERMOSTATO");
 */

// while(!Serial.available()) {}
// Serial.readString();

// //shift del display
// for(i=0; i<20; i++){
// LCD_scrollDisplayLeft();
// delay(100);
// }

// while(!Serial.available()) {}
// Serial.readString();

// //Parte alta del juego de caracteres
// LCD_clear();

// for (i = 0xe0; i< 0xe0; i++){
// LCD_write(i);
// delay(100);
// }
// LCD_setCursor(0x40);                                //lcd.setCursor(64, 0);
// for (i = 0xf0; i< 0xf0+15; i++){
// LCD_write(i);
// delay(100);
// }
// LCD_write(i);

// while(!Serial.available()) {}
// Serial.readString();


