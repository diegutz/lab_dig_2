#define LED_G GREEN_LED
#define LED_B BLUE_LED
#define LED_R RED_LED
#define PB0 2
#define PB1 3
#define PB2 4
#define PB3 5
#define PB4 8
#define PB5 9
#define PB6 7
#define PB7 6
#define PE0 11
#define PE1 12
#define PE2 13
#define PE3 14
#define PE4 15
#define PE5 17
#define PE6 18
#define PE7 19
const byte ledg = LED_G;  //definimos pin del led
const byte ledb = LED_B;  //definimos pin del led
const byte ledr = LED_R;  //definimos pin del led
const byte interruptPin = PUSH1;  //definimos el boton 1
const byte interruptPin2 = PUSH2; //definimos el boton 2
volatile byte state1 = LOW;        //iniciamos variable volatil
volatile byte state2 = LOW;        //iniciamos variable volatil
volatile byte iniciar = LOW;        //iniciamos variable volatil
volatile byte run = LOW;
volatile byte cont1 = 0;
volatile byte cont2 = 0;
byte port_b[8]={0,0,0,0,0,0,0,0};
byte port_e[8]={0,0,0,0,0,0,0,0};

void setup() {
  pinMode(ledg, OUTPUT);              //definimos las entradas y salidas
  pinMode(ledb, OUTPUT);              //definimos las entradas y salidas
  pinMode(ledr, OUTPUT);              //definimos las entradas y salidas
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  config_leds();
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING); //iniciamos interrupcion 1 con flanco negativo
  attachInterrupt(digitalPinToInterrupt(interruptPin2), blink2, FALLING); //iniciamos interrupcion 2 con flanco positivo
}

void loop() {
  if (iniciar == HIGH && run ==LOW){
    digitalWrite(ledr,HIGH);
    delay(2000);
    digitalWrite(ledg,HIGH);
    delay(1000);
    digitalWrite(ledr,LOW);
    run = HIGH;
  }
  led1();
  led2();
  if(cont1 == 7){
    run = LOW;
    iniciar = LOW;
    digitalWrite(ledg,LOW);
    delay(500);
    digitalWrite(ledg,HIGH);
    delay(500);
    digitalWrite(ledg,LOW);
    delay(500);
    digitalWrite(ledg,HIGH);
    delay(500);
  }
  if(cont2 == 7){
    run = LOW;
    iniciar = LOW;
    digitalWrite(ledg,LOW);
    delay(500);
    digitalWrite(ledg,HIGH);
    delay(500);
    digitalWrite(ledg,LOW);
    delay(500);
    digitalWrite(ledg,HIGH);
    delay(500);
  }
  delay(50);
}
void blink() 
{
    iniciar = HIGH;
    if (run == HIGH){
      cont1++;
    port_b[cont1-1] = 0;
    port_b[cont1] = 1;
    }
}
void blink2(){
  iniciar = HIGH;
  if (run == HIGH){
      cont2++;
    port_e[cont2-1] = 0;
    port_e[cont2] = 1;
    }
}
void led1(void){
  digitalWrite(PB0,port_b[0]); 
  digitalWrite(PB1,port_b[1]);
  digitalWrite(PB2,port_b[2]);
  digitalWrite(PB3,port_b[3]);
  digitalWrite(PB4,port_b[4]);
  digitalWrite(PB5,port_b[5]);
  digitalWrite(PB6,port_b[6]);
  digitalWrite(PB7,port_b[7]);
}
void led2(void){
  digitalWrite(PE0,port_e[0]); 
  digitalWrite(PE1,port_e[1]);
  digitalWrite(PE2,port_e[2]);
  digitalWrite(PE3,port_e[3]);
  digitalWrite(PE4,port_e[4]);
  digitalWrite(PE5,port_e[5]);
  digitalWrite(PE6,port_e[6]);
  digitalWrite(PE7,port_e[7]);
}
void config_leds(void){
  pinMode(PB0,OUTPUT);
  pinMode(PB1,OUTPUT);
  pinMode(PB2,OUTPUT);
  pinMode(PB3,OUTPUT);
  pinMode(PB4,OUTPUT);
  pinMode(PB5,OUTPUT);
  pinMode(PB6,OUTPUT);
  pinMode(PB7,OUTPUT);
  
  pinMode(PE0,OUTPUT);
  pinMode(PE1,OUTPUT);
  pinMode(PE2,OUTPUT);
  pinMode(PE3,OUTPUT);
  pinMode(PE4,OUTPUT);
  pinMode(PE5,OUTPUT);
  pinMode(PE6,OUTPUT);
  pinMode(PE7,OUTPUT);
  
  digitalWrite(PB0,LOW); 
  digitalWrite(PB1,LOW);
  digitalWrite(PB2,LOW);
  digitalWrite(PB3,LOW);
  digitalWrite(PB4,LOW);
  digitalWrite(PB5,LOW);
  digitalWrite(PB6,LOW);
  digitalWrite(PB7,LOW);
  
  digitalWrite(PE0,LOW);
  digitalWrite(PE1,LOW);
  digitalWrite(PE2,LOW);
  digitalWrite(PE3,LOW);
  digitalWrite(PE4,LOW);
  digitalWrite(PE5,LOW);
  digitalWrite(PE6,LOW);
  digitalWrite(PE7,LOW);
}
