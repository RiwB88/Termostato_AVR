#include <SoftwareSerial.h>



// void setup() {


// }

// void loop() {
//   int i = 0;
//   byte boton = 0;
//   byte tecla;
//   int valor;

//   Serial.println("");
//   Serial.println("Teclado Iniciado");

//   while(true){  
//       //Leer_teclado_nf();

//       Serial.println();
//       Serial.print(i++);
//       Serial.print(" Tecla pulsada...    ");
//       Serial.print(tecla = Filtrar_teclado());
//       Serial.print(" (");
//       Serial.print(valor);
//       Serial.print(") Procesando tecla... Esperando fin de tecla...");

//      // Teclado_libre();  //Esperamos a que se libere el teclado

//       Serial.println();

//     int repe = 500;

//     while(tecla == Repetir_tecla(tecla, repe)){

//         //Si (tecla != 0)
//         Serial.println();
//         Serial.print("  Tecla ");
//         Serial.print(tecla);
//         Serial.print("  Repetida. Procesando tecla....");

//         repe = 200;       //Disminuimos tiempo necesario para que se cuente como tecla repetida
//     }
//   }
// }

byte Repetir_tecla(byte tecla, int espera){
  byte boton = 255;
  int Fi[5] = {0, 0, 0, 0, 0};
  int j; 
  int flag = 0;

  //Serial.println(" Entrando en repetición ");

  for(j = 0; j <= espera && flag == 0; j++){
    int valor = analogRead(A0);

    switch(tecla){
      case 1: 
        if(valor >= 0 && valor < 70){
          if(Fi[0]++ == espera){
            boton = 1;
          }
        }
        else boton = 0;

        break;

      case 2: 
        if(valor >= 70 && valor < 234){
          if(Fi[1]++ == espera){
            boton = 2;
          }
        }
        else boton = 0;
        break;

      case 3: 
        if(valor >= 234 && valor < 414){
           if(Fi[2]++ == espera){
            boton = 3;
           }
        }
        else boton = 0;
        break;

      case 4: 
        if(valor >= 414 && valor < 600){
          if(Fi[3]++ == espera)
            boton = 4;
        }
        else boton = 0;
        break;

      case 5: 
        if(valor >= 600 && valor < 900){
          if(Fi[4]++ == espera)
            boton = 5;
        }
        else boton = 0;
        break;

      default:
        boton = 0;
        break;
    }

    if(boton != 255){       //Verificar si esto en verdad está bien
      flag++;
    }

    delay(1);
  }

  if (boton == 0){
    Teclado_libre();
  }
  return boton;
}




byte Filtrar_teclado(){      //Esperar pulsación y filtrar rebotes
  byte boton = 0;
  byte Fi[5] = {5, 5, 5, 5, 5};

  while(boton == 0){
    int valor = analogRead(A0);
    
    if(valor > 900){
      for(int i = 0; i < 5; i++){       //Reiniciamos Fi
        Fi[i] = 5;
      }
    }

  else if(valor >= 580){      //ESTABA EN 600
    Fi[4]--;

      for(int i = 0; i < 5; i++){ 
        if(i != 4)      //Reiniciamos Fi
          Fi[i] = 5;
      }
  }
  else if(valor >= 414){
    Fi[3]--;

        for(int i = 0; i < 5; i++){ 
          if(i != 3)      //Reiniciamos Fi
            Fi[i] = 5;
      }
  }

  else if(valor >= 234){
    Fi[2]--;
  
        for(int i = 0; i < 5; i++){ 
          if(i != 2)      //Reiniciamos Fi
            Fi[i] = 5;
      }
  }

  else if(valor >= 100){        //Antes estaba en 73
    Fi[1]--;

        for(int i = 0; i < 5; i++){ 
          if(i != 1)      //Reiniciamos Fi
            Fi[i] = 5;
      }
  }

  else if(valor >= 0){
    Fi[0]--;

        for(int i = 0; i < 5; i++){ 
          if(i != 0)      //Reiniciamos Fi
            Fi[i] = 5;
      }
  }

  for(int i = 0; i < 5; i++){       //Verificamos si Fi == 0
        if(Fi[i] == 0){
          boton = i + 1;
        }
      }

  }
  Serial.println(boton);

  return boton;
}


void Teclado_libre(){
  int valor = analogRead(A0);
  while(valor < 900){
    valor = analogRead(A0);
  }
}



byte Leer_teclado_nf() {
  byte boton = 255;
  int valor = analogRead(A0);

 /* while(valor > 900){     Quito esto para que no se bloquee y se actualize el lector del sensor
   boton = 0;
   valor = analogRead(A0);
  } */

  if(valor >= 650){     //ESTABA A 600
    boton = 5;
  }
  else if(valor >= 414){
    boton = 4;
  }

  else if(valor >= 234){
    boton = 3;
  }

  else if(valor >= 70){
    boton = 2;
  }

  else if(valor >= 0){
    boton = 1;
  }

  // Serial.print(valor);
  // Serial.print(" que es la tecla .... ");
  // Serial.print(boton);
  // Serial.println();


  return boton;
}




