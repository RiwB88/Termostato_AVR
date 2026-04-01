#define sbi(port, bit) (port) |= (1 << (bit))
#define cbi(port, bit) (port) &= ~(1 << (bit))


void setup() {
  // put your setup code here, to run once:

 sbi(DDRB, DDB4);     //pinMode(8, OUTPUT);
 sbi(DDRB, DDB5);     //pinMode(9, OUTPUT);
 sbi(DDRD, DDD4);     //pinMode(4, OUTPUT);
 sbi(DDRC, DDC6);     //pinMode(5, OUTPUT);
 sbi(DDRD, DDD7);     //pinMode(6, OUTPUT);
 sbi(DDRE, DDE6);     //pinMode(7, OUTPUT);

 sbi(DDRD, DDD0);      //pinMode(3, OUTPUT);
 cbi(DDRF, DDF7);      //pinMode(A0, INPUT);
 cbi(DDRF, DDF1);      //pinMode(A4, INPUT);
 cbi(DDRF, DDF5);      //pinMode(A2, INPUT);

Serial.begin(9600);

  LCD_Reset();
  LCD_home();
  delay(1000);

  LCD_print("Iniciando menu...");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  menu_principal();
}
