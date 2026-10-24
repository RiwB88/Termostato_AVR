byte Filtrar_teclado();
byte leerTecladoEstable();

typedef enum {
  ENCENDIDO = 0,
  APAGADO = 1,
  MAN = 2,
  TERMOSTATO = 3,
  POTENCIOMETRO = 4
}modos;

typedef enum {
  TERMOSTATO_S = 0,
  PARAMETROS = 1,
  RESET = 2,
  ATRAS = 3
}modos_2;

typedef enum{
  P = 0,
  I = 1,
  D = 2,
  VOLVER = 3
}pid_var;

byte linea[4] = {0x12, 0x52, 0x26, 0x66};

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

  LCD_clear();

  lis_1(linea[ind_lin]);

while(true){

  tecla = leerTecladoEstable();

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
    //Teclado_libre();
    }else{
      reset = false;
      ind_lin = 0;
      lis_1(linea[ind_lin]);
      //Teclado_libre();
    }
  delay(30);
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
  cbi(EIMSK,INT1);           //Desactivamos INT1
  enc_max();
  //debug_sel("encendido");
  break;
  case APAGADO:
  //En modo Apagado: Temperatura leída por el sensor. 
  //Apagar: Apaga el agente calefactor.
  cbi(EIMSK,INT1);           //Desactivamos INT1
  apag();
  //debug_sel("Apagando");
  break;
  case MAN:
  //En modo Control manual: Temperatura leída por el sensor y porcentaje de funcionamiento del agente calefactor. 
  //Control manual: Establece el porcentaje de funcionamiento dal agente calefactor y regula la energía suministrada.
  manual();
  //debug_sel("manual");
  break;
  case TERMOSTATO:
  //En Modo Termostato: Temperatura leída por el sensor, temperatura objetivo y porcentaje de funcionamiento del agente calefactor. 
  //Termostato: Regula la energía suministrada al agente calefactor manteniendo la temperatura objetivo establecida. (PID )
  cbi(EIMSK,INT1);           //Activamos INT1
  sub_menu();
  //debug_sel("termostato");
  break;
  case POTENCIOMETRO:
  //En modo Potenciómetro: Temperatura leída por el sensor y porcentaje de funcionamiento del agente calefactor. 
  //Potenciómetro: Regula la energía sumimistrada al agente calefactor por medio del potenciómetro.
  sbi(EIMSK,INT1);           //Activamos INT1
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
  LCD_clear();
  dtostrf(val, 6, 2, temp_str);     //No se puede usar %f directamente en sprintf
  sprintf(buf, "Temp = %s", temp_str);
  LCD_setCursor(0x41);
  LCD_print(buf); //sprintf
  LCD_write(0xDF);
  LCD_print("C");
  //delay(1000);
}

void temp_leida_porc(float val, int pot){
  char buf[24];
  char temp_str[8];   
  LCD_clear();
  dtostrf(val, 6, 2, temp_str);     //No se puede usar %f directamente en sprintf
  sprintf(buf, "Temp = %s", temp_str);
  LCD_setCursor(0x41);
  LCD_print(buf); //sprintf
  LCD_write(0xDF);
  LCD_print("C");
  sprintf(buf, "Porc = %d%%", pot);
  LCD_setCursor(0x15);
  LCD_print(buf); //sprintf
  //delay(1000);
}

void temp_leida_term(float val){
  char buf[21];
  char s[8];
/*
  dtostrf(Kp, 5, 2, s);
  sprintf(buf, "P%-6sI", s);        
  LCD_setCursor(0x00);
  LCD_print(buf);
  dtostrf(Ki, 5, 2, s);
  LCD_print(s);


  dtostrf(Kd, 5, 2, s);
  sprintf(buf, "D%-6s", s);
  LCD_setCursor(0x40);
  LCD_print(buf);
*/
  //dtostrf(Setpoint, 6, 2, s);
  sprintf(buf, "Obj:%d", (int)Setpoint);
  LCD_setCursor(0x00);
  LCD_print(buf);
  LCD_write(0xDF);
  LCD_print("C");

  dtostrf(val, 6, 2, s);
  sprintf(buf, "Temp:%s", s);
  LCD_setCursor(0x40);
  LCD_print(buf);
  LCD_write(0xDF);
  LCD_print("C");


  sprintf(buf, "Porc:%d%%", potencia);
  LCD_setCursor(0x14);
  LCD_print(buf);
}

void porc_leida(int por){
  char buf[24];
  LCD_clear();
  sprintf(buf, "Pote =  %d %%", por);
   //LCD_clear();
   LCD_setCursor(0x15);
   LCD_print(buf); //sprintf
   //delay(1000);
}

/*
void print_pot(){
  LCD_setCursor(0x00);
  LCD_clear();
  LCD_print("    ///// + /////");
  LCD_setCursor(0x40);
  LCD_print("Escoja potencia= ");
  LCD_setCursor(0x14);
  LCD_print("    ///// - /////");
}
*/
void camb_pot(int val){
  char buf[24];
  LCD_clear();
  sprintf(buf, "Escoja potencia= %d", val);
  LCD_setCursor(0x40);
  LCD_print(buf);

}


void sub_menu(){

int ind_lin_term = 0;

byte tecla;

  LCD_clear();

  lis_termo(linea[ind_lin_term]);

while(true){

  tecla = leerTecladoEstable();

  if(tecla == 3){   //Botón abajo
    if(ind_lin_term < 3){
      ind_lin_term++;
    }else{
      ind_lin_term=0;
    }
    lis_termo(linea[ind_lin_term]);
  }
  
  else if(tecla == 2){                   //Botón arriba
    if(ind_lin_term > 0){
      ind_lin_term--;
    }else{
      ind_lin_term=3;
    }
    lis_termo(linea[ind_lin_term]);
  }

   else if(tecla == 5){                 //Botón OK
    int modo = ind_lin_term;
    ir_a_termo(modo);
  }
  delay(30);
}
}

void lis_termo(byte pos){
LCD_clear();

  LCD_setCursor(pos);
  LCD_print("<-");
  
  LCD_setCursor(0x00);
  LCD_print("1 TERMOSTATO");

  LCD_setCursor(0x40);
  LCD_print("2 PARAMETROS");

  LCD_setCursor(0x14);
  LCD_print("3 RESET");

  LCD_setCursor(0x54);
  LCD_print("4 MENU");


}


void ir_a_termo(int modo){
  switch(modo){
  case TERMOSTATO_S:
  sbi(EIMSK,INT1);           //Desactivamos INT1
  pid_pr();
  //debug_sel("Termostato");
  break;
  case PARAMETROS:
  cbi(EIMSK,INT1);           //Desactivamos INT1
  sub_menu_term();
  break;
  case RESET:
  cbi(EIMSK,INT1);           //Desactivamos INT1
  reset();
  break;
  case ATRAS:
  menu_principal();
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

void lis_param(byte pos){
  char buf[24];
  char val_str[8];
  LCD_clear();

  LCD_setCursor(pos);
  LCD_print("<-");

  LCD_setCursor(0x00);
  dtostrf(Kp, 5, 2, val_str);
  sprintf(buf, "Kp = %s", val_str);
  LCD_print(buf);

  LCD_setCursor(0x40);
  dtostrf(Ki, 5, 2, val_str);
  sprintf(buf, "Ki = %s", val_str);
  LCD_print(buf);

  LCD_setCursor(0x14);
  dtostrf(Kd, 5, 2, val_str);
  sprintf(buf, "Kd = %s", val_str);
  LCD_print(buf);

  LCD_setCursor(0x54);
  LCD_print("VOLVER");
}

void sub_menu_term(){

int ind_lin_term = 0;

byte tecla;

  LCD_clear();

  lis_param(linea[ind_lin_term]);

while(true){

  tecla = leerTecladoEstable();

  if(tecla == 3){   //Botón abajo
    if(ind_lin_term < 3){
      ind_lin_term++;
    }else{
      ind_lin_term=0;
    }
    lis_param(linea[ind_lin_term]);
  }
  
  else if(tecla == 2){                   //Botón arriba
    if(ind_lin_term > 0){
      ind_lin_term--;
    }else{
      ind_lin_term=3;
    }
    lis_param(linea[ind_lin_term]);
  }
   else if(tecla == 5){                 //Botón OK
    int modo = ind_lin_term;
    ir_a_param(modo);
  }
  delay(30);
}
}

void ir_a_param(int modo){    //Submenu donde escoger las variables para el PID (Kp, Ki, Kd)
  switch(modo){
  case P:
  variable(modo);
  break;
  case I:
  variable(modo);
  break;
  case D:
  variable(modo);
  break;
  case VOLVER:
  sub_menu();
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


