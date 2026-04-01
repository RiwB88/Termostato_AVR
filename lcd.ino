//#include <LiquidCrystal.h>
//#include <string.h>


#define sbi(port, bit) (port) |= (1 << (bit))
#define cbi(port, bit) (port) &= ~(1 << (bit))

//Crea el objeto LCD con los pines que usa el shield
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int pinKeypad = A0;

void LCD_write(byte car)
{
 sbi(PORTB, PB4); //digitalWrite(8, HIGH);  // RS a 1
 cbi(PORTB, PB5); //digitalWrite(9, LOW);  // E a 0

 leerCar(car >> 4); //Leemos parte alta
 sbi(PORTB, PB5);  //digitalWrite(9, HIGH);  // E a 1
 delayMicroseconds(10);

 cbi(PORTB, PB5);  // E a 0
 leerCar(car & 0x0F); //leemos parte alta
 sbi(PORTB, PB5); //digitalWrite(9, HIGH);  // E a 1
 delayMicroseconds(10);

 cbi(PORTB, PB5);  // E a 0 
 delayMicroseconds(100);
}

void LCD_print(char* s){
  byte c;
  int longitud = strlen(s);
  for(int i = 0; i < longitud; i++){
    c = s[i];
    LCD_write(c);
  }
}

/************************Reset**********************/

void LCD_Reset(){
  delay(50);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//modo 8 bits//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

  cbi(PORTB, PB4);  //digitalWrite(8, LOW);  // RS a 0 (recibe comandos)
  cbi(PORTB, PB5);  // E a 0 

  sbi(PORTD, PD4);  //digitalWrite(4, HIGH);
  sbi(PORTC, PC6); //digitalWrite(5, HIGH);
  cbi(PORTD, PD7); //digitalWrite(6, LOW);
  cbi(PORTE, PE6); //digitalWrite(7, LOW);

  sbi(PORTB, PB5);  // E a 1   Mandamos 0011 -----> paquete 1
  delayMicroseconds(10);
  cbi(PORTB, PB5);  // E a 0
  delay(5);

  sbi(PORTB, PB5);  // E a 1    Mandamos 0011 -----> paquete 2
  delayMicroseconds(10);
  cbi(PORTB, PB5);  // E a 0     
  delayMicroseconds(110);

  sbi(PORTB, PB5);  // E a 1    Mandamos 0011 ------> paquete 3
  delayMicroseconds(10);



  cbi(PORTB, PB5);  // E a 0
  cbi(PORTD, PD4); //digitalWrite(4, LOW);              //Mandamos 0010

  sbi(PORTB, PB5);  // E a 1
  delayMicroseconds(10);
  cbi(PORTB, PB5);  // E a 0
  delayMicroseconds(100);

/*-------------//Pasamos a 4 bits//---------------*/

  LCD_command(0x28);     //Binario 00101000
  delayMicroseconds(37);

  LCD_command(0x0C);                    //LCD_command(0x08);    //Binario 00001000
  delayMicroseconds(37);

  LCD_command(0x01);    //Binario 00000001
  delayMicroseconds(2000);

  LCD_command(0x06);    //Binario 00000110

  delayMicroseconds(37);

}

void LCD_command(byte car){

 cbi(PORTB, PB4);   //digitalWrite(8, LOW);  // RS a 0
 cbi(PORTB, PB5);  //digitalWrite(9, LOW);  // E a 0

 leerCar(car >> 4);     //Leemos parte alta
 sbi(PORTB, PB5);  //digitalWrite(9, HIGH);  // E a 1

 delayMicroseconds(10);

 cbi(PORTB, PB5);  // E a 0
 leerCar(car & 0x0F);      //Parte baja
 sbi(PORTB, PB5); //digitalWrite(9, HIGH);  // E a 1

 delayMicroseconds(10);

 cbi(PORTB, PB5);  // E a 0 

}


void leerCar(byte oct){
 if(oct & 0x01) sbi(PORTD, PD4); else cbi(PORTD,PD4);   //digitalWrite(4, (car >> 0)&1);              
 if(oct & 0x02) sbi(PORTC, PC6); else cbi(PORTC,PC6);   //digitalWrite(5, (car >> 1)&1);
 if(oct & 0x04) sbi(PORTD, PD7); else cbi(PORTD,PD7);   //digitalWrite(6, (car >> 2)&1);
 if(oct & 0x08) sbi(PORTE, PE6); else cbi(PORTE,PE6);   //digitalWrite(7, (car >> 3)&1);
}


void funcion2(){
char s1[21];
char s2[21];
char s3[21];
byte h=3, m=23, s=7, c=85;
int j;

LCD_clear();

for(j=0; j<5; j++){
    LCD_home();
  sprintf(s1,"hora %2d:%02d:%02d:%02d", h++, m--, s++, c--);
  LCD_print(s1);
  delay(1000);
}

for(j=0; j<5; j++){
  //Entero sin signo
  sprintf(s3, "millis= %u", millis());
  LCD_home();
  LCD_print(s3);

  delay(1000);

  //Coma flotante
  sprintf(s2, "ms/us= %s", dtostrf((float)micros()/millis(),9,3,s3));
  LCD_home();
  LCD_print(s2);
}
}




void LCD_clear(){
  LCD_command(0x01);
  delay(2);
}

void LCD_home(){
  LCD_command(0x02);
  delay(2);
}

void LCD_setCursor(byte pos){
  LCD_command(0x80 | pos);
  delayMicroseconds(37);
}

void LCD_scrollDisplayLeft(){
  LCD_command(0x18);      
  delayMicroseconds(37);

}
