#include <PID_v1.h>



void logCSV(unsigned long tiempo, float tempReal, float tempObj, int salidaPID) {
  Serial.print(tiempo);
  Serial.print(",");
  Serial.print(tempReal);
  Serial.print(",");
  Serial.print(tempObj);
  Serial.print(",");
  Serial.println(salidaPID);
}

void pid_pr_log(){
    LCD_clear();    
    int neregui = 0;
    Serial.println("tiempo_ms,temp_real,temp_objetivo,salida_pid");

    do{
        anal_digit(A4);
        Input = leerTemperatura();

        if (millis() - ultimoLog >= 1000) {
            ultimoLog = millis();
            neregui++;
            logCSV(millis(), Input, Setpoint, Output);
            temp_leida(Input);
        }

        if(myPID.Compute()){
            potencia = (int)Output;
        }
        tecla = leerTecladoEstable();

    }while(tecla != 1);  
}                         

void pid_pr(){
    LCD_clear();    
    Setpoint = escoger_double(Setpoint, 1.0, 100.0, "TEMP. OBJ.");
    LCD_clear();
    do {
        anal_digit(A4);
        Input = leerTemperatura();
        if (millis() - ultimoLog >= 1000) {
            ultimoLog = millis();
            temp_leida_term(Input);
        }
        if (myPID.Compute()) {
            potencia = (int)Output;
        }
        tecla = leerTecladoEstable();
    } while (tecla != 1);

    potencia = 0;       // apagar al salir
    sub_menu();
}

void reset(){
  LCD_clear();
  Kp=0;
  Ki=0;
  Kd=0;
  myPID.SetTunings(0, 0, 0);
  LCD_home();
  LCD_print("Par. restablecidos");
  delay(1500);
  sub_menu();

} 

int variable(int var){
  int k = 0;
  Serial.println(var);
  switch(var){
    case P:
      Kp = escoger_double(Kp,0.1,5.0,"Kp");
      myPID.SetTunings(Kp, Ki, Kd);
      break;
    case I:
      Ki = escoger_double(Ki,0.01,0.5,"Ki");
      myPID.SetTunings(Kp, Ki, Kd);
      break;
    case D:
      Kd = escoger_double(Kd,0.5,15.0,"Kd");
      myPID.SetTunings(Kp, Ki, Kd);
      break;
    default:
      sub_menu_term();
  }
  sub_menu_term();
}


double escoger_double(double inicial, double incremento, double max, char *variable){
  double k = inicial;
  char buf_ajustar[19];
  char buf[17];
  char val_str[8];
  
  LCD_clear();
  LCD_setCursor(0x00);
  sprintf(buf_ajustar, "Ajustando %s:", variable);
  LCD_print(buf_ajustar);
  LCD_setCursor(0x49);
  LCD_print("+");
  LCD_setCursor(0x5D);
  LCD_print("-");

  
  dtostrf(k, 6, 2, val_str); // Muestra valor inicial antes de esperar tecla
  sprintf(buf, "< %s >", val_str);
  LCD_setCursor(0x19);
  LCD_print(buf);

  tecla = leerTecladoEstable();   
  
  while(tecla != 1 && tecla != 5){
    if (tecla == 2) {             // ARRIBA
      k += incremento;
      if (k > max) k = max;       // En coma flotante los incrementos no los representa bien, nos aseguramos
    }
    if (tecla == 3) {             // ABAJO
      k -= incremento;
      if (k < 0.0) k = 0.0;     
    }

      dtostrf(k, 6, 2, val_str); // Muestra valor inicial antes de esperar tecla
      sprintf(buf, "< %s >", val_str);
      LCD_setCursor(0x19);
      LCD_print(buf);

      tecla = leerTecladoEstable();
      }
  if(tecla == 5){
    return k;
  }else{
    sub_menu_term();
  }
}
