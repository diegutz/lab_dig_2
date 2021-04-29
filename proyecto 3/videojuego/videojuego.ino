#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  
int8_t  carril_1[19];
int8_t  carril_1_temp[19];
int8_t  carril_2[19];
int8_t  carril_2_temp[19];
int8_t  carril_3[19];
int8_t  carril_3_temp[19];
int8_t  carril_4[19];
int8_t  carril_4_temp[19];
int8_t  carril_5[19] ;
int8_t  carril_5_temp[19];
int jugando = 0; //0 = menu; 1= jugando ; 2= game over | 3 = ganaste
int tipo_nave = 0;
int ran1 = 0;
int cont = 0;
int vida = 0;
int pos_star = 0;
int sel_nave =0; //0 = x_wing | 1 = halcon milenario
int buzzerPin = 40;
volatile uint8_t cont_notas= 0;
volatile uint8_t cont_notas_loss= 0;
volatile uint8_t cont_notas_win= 0;
                      //0   1     2    3    4    5    6    7    8     9    10   11 12  13  14  15   16
                      //do  do#   re   re#  mi   fa   fa# sol   sol#  la  la#  si ,do, re ,mib, sol, sol3
volatile int tonos[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,523,587,612,789, 196};
volatile int notas[] = {7,7,7,3,10,7,3,10,7,13,13,13,14,10,8,14,10,7};
volatile int perder[] = {1,7,4,9,11,9,8,10,8,4,2,4};
volatile int ganar[] = {16,0,2,3,5,3,16,16,0,2,4,16,3,0,9,5};
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);
void desplegar_nave(int i);
void clear_mapa(void);
void fondo_estrella(void);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);


extern uint8_t fondo[];
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);              //definimos las entradas y salidas
  pinMode(BLUE_LED, OUTPUT);              //definimos las entradas y salidas
  pinMode(RED_LED, OUTPUT);              //definimos las entradas y salidas
  pinMode(PF_4, INPUT);
  pinMode(PD_7, INPUT);
  pinMode(PF_1, INPUT);
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PF_4), blink, FALLING); //iniciamos interrupcion 1 con flanco negativo
  attachInterrupt(digitalPinToInterrupt(PD_7), &blink2, FALLING); //iniciamos interrupcion 1 con flanco negativo
  attachInterrupt(digitalPinToInterrupt(PF_1), &blink3, FALLING); //iniciamos interrupcion 1 con flanco negativo
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  configureTimer1A();     //iniciamos protocolo de interrupcion
  LCD_Clear(0x0F);
  
  clear_mapa();
  
  FillRect(0, 0, 319, 206, 0x421b);
  String text1 = "starwars arcade";
  LCD_Print(text1, 20, 100, 2, 0xffff, 0x421b);
//LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
    
  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  //LCD_Bitmap(0, 0, 320, 240, fondo);
  fondo_estrella();   
    H_line(0, 52, 300, 0xFF);
  
        
    
}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  while(jugando==0){      //70-100
    
  vida = 0;
  clear_mapa();
  LCD_Sprite(286, 70, 32,32,x_wing, 3, vida, 0, 0);
  LCD_Sprite(286, 100, 32,32,halcon_milenario, 3, vida, 0, 0);
//  LCD_Sprite(286, 130, 32,32,a_wing, 3, vida, 0, 0);
  FillRect(40, 70, 200, 190, 0x00);
  if(cont == 0){
  String text1 = "----------->";
  LCD_Print(text1, 40, 80, 2, 0xffff, 0x421b);
  sel_nave =0;
    }
  if(cont == 1){      //100-130
  String text1 = "----------->";
  LCD_Print(text1, 40, 110, 2, 0xffff, 0x421b);
  sel_nave =1;
  }
  if(cont == 2){      //130-160
  String text1 = "----------->";
  LCD_Print(text1, 40, 140, 2, 0xffff, 0x421b);
  sel_nave =2;
  }
  if(cont == 3){      //160-190
  String text1 = "----------->";
  LCD_Print(text1, 40, 170, 2, 0xffff, 0x421b);
  }
  if(cont == 4){      //190
  String text1 = "----------->";
  LCD_Print(text1, 40, 200, 2, 0xffff, 0x421b);
  }
  
  delay(150);
  }
  while(jugando == 1){
    
    LCD_Sprite(pos_star, 0, 48,48,death_star, 1, 0, 0, 0);
    pos_star = pos_star+4;
    if(pos_star > 280){
      jugando = 3;
    }
//----------------------------------------mueve el estado de los proyectiles para generar movimiento----------------------------
  for(int i =0;i < 19; i++){
    carril_1_temp[i] = carril_1[i-1];
    carril_2_temp[i] = carril_2[i-1];
    carril_3_temp[i] = carril_3[i-1];
    carril_4_temp[i] = carril_4[i-1];
    carril_5_temp[i] = carril_5[i-1];
  }
  for(int i =0;i < 19; i++){
    carril_1[i] = carril_1_temp[i];
    carril_2[i] = carril_2_temp[i];
    carril_3[i] = carril_3_temp[i];
    carril_4[i] = carril_4_temp[i];
    carril_5[i] = carril_5_temp[i];
  }
  //-----------------------------------------------vidas del jugador---------------------------------------
  if(carril_1[18]==1 && cont == 0){
    vida++;
  }
  if(carril_2[18]==1 && cont == 1){
    vida++;
  }
  if(carril_3[18]==1 && cont == 2){
    vida++;
  }
  if(carril_4[18]==1 && cont == 3){
    vida++;
  }
  if(carril_5[18]==1 && cont == 4){
    vida++;
  }
  if(vida==3){
  jugando = 2;
  }
  //----------------------------------------------generación automatica de enemigos-------------------------------------------------
  ran1 = random(0, 100);
  if (ran1 > 90 && carril_3[1] == 0){
    carril_1[0] = 1;
  }
  ran1 = random(0, 100);
  if (ran1 > 90 && carril_2[1] == 0){
    carril_2[0] = 1;
  }
  ran1 = random(0, 100);
  if (ran1 > 90 && carril_1[1] == 0){
    carril_3[0] = 1;
  }
  ran1 = random(0, 100);
  if (ran1 > 90 && carril_4[1] == 0){
    carril_4[0] = 1;
  }
  ran1 = random(0, 100);
  if (ran1 > 90 && carril_5[1] == 0){
    carril_5[0] = 1;
  }

  //-----------------------------------estado de los disparos---------------------------------------------
  for(int i =0;i < 18; i++){
    switch (carril_1[i]) {
  case 0:
    LCD_Sprite(i*16, 80, 20,20,estrella, 1, 0, 0, 0);
    break;
  case 1:
      LCD_Bitmap(i*16, 80, 16, 16, tile2);
    break;
}
    switch (carril_2[i]) {
  case 0:
    LCD_Sprite(i*16, 110, 20,20,estrella, 1, 0, 0, 0);
    break;
  case 1:
      LCD_Bitmap(i*16, 110, 16, 16, tile2);
    break;
}
    switch (carril_3[i]) {
  case 0:
    LCD_Sprite(i*16, 140, 20,20,estrella, 1, 0, 0, 0);
    break;
  case 1:
      LCD_Bitmap(i*16, 140, 16, 16, tile2);
    break;
}
    switch (carril_4[i]) {
  case 0:
    LCD_Sprite(i*16, 170, 20,20,estrella, 1, 0, 0, 0);
    break;
  case 1:
    LCD_Bitmap(i*16, 170, 16, 16, tile2);
    break;
}
    switch (carril_5[i]) {
  case 0:
    LCD_Sprite(i*16, 200, 20,20,estrella, 1, 0, 0, 0);
    break;
  case 1:
      LCD_Bitmap(i*16, 200, 16, 16, tile2);
    break;
}
  //----------------------------------------borrar nave para presentar el estado de la ultima fila----------------------------------------
  //para mostrar disparos een la columna de la nave pimero es necesario borrar la nave  
  }
  for(int i =0;i < 5; i++){
    FillRect(286, 70 + (i*30), 32, 32, 0x00);
    }
    //----------------------------------------se generan los disparos en la columna del jugador-------------------------
    if(carril_1[18] == 1){
      LCD_Bitmap(288, 80, 16, 16, tile2);
    }
    if(carril_2[18] == 1){
      LCD_Bitmap(288, 110, 16, 16, tile2);
    }
    if(carril_3[18] == 1){
      LCD_Bitmap(288, 140, 16, 16, tile2);
    }
    if(carril_4[18] == 1){
      LCD_Bitmap(288, 170, 16, 16, tile2);
    }
    if(carril_5[18] == 1){
      LCD_Bitmap(288, 200, 16, 16, tile2);
    }
    if(carril_1[18] == 0){
    FillRect(2886, 80, 16, 16, 0x00);
    }
    if(carril_2[18] == 0){
    FillRect(288, 110, 16, 16, 0x00);
    }
    if(carril_3[18] == 0){
    FillRect(288, 140, 16, 16, 0x00);
    }
    if(carril_4[18] == 0){
    FillRect(288, 170, 16, 16, 0x00);
    }
    if(carril_5[18] == 0){
    FillRect(288, 200, 16, 16, 0x00);
    }

    
for(int i =0;i < 5; i++){
        desplegar_nave(i);
    }
  tone(buzzerPin, tonos[notas[cont_notas]], 100);
  delay(400);
  noTone(buzzerPin); 
  cont_notas++;
  if(cont_notas == 17){
    cont_notas =0;
  }
  }
  while(jugando == 2){
    LCD_Bitmap(0, 0, 320, 240, fondo);
    while(cont_notas_loss < 12){
      tone(buzzerPin, tonos[perder[cont_notas_loss]], 200);
      delay(400);
      noTone(buzzerPin); 
      cont_notas_loss++;
    }
    fondo_estrella();   
    jugando = 0;
    cont_notas_loss = 0;
    cont_notas_win = 0;
    pos_star = 0;
  }
  while(jugando == 3){
    fondo_estrella(); 
  LCD_Sprite(100, 100, 32,32,x_wing, 3, 0, 0, 0);
  LCD_Sprite(140, 150, 32,32,halcon_milenario, 3, 0, 0, 0);
  //LCD_Sprite(100, 180, 32,32,a_wing, 3, 0, 0, 0);
  LCD_Sprite(140, 70, 48,72,medalla, 1, 0, 0, 0);
    while(cont_notas_win < 17){
      tone(buzzerPin, tonos[ganar[cont_notas_win]], 400);
      delay(500);
      noTone(buzzerPin); 
      cont_notas_win++;
    }

    fondo_estrella();   
    jugando = 0;
    pos_star = 0;
    cont_notas_loss = 0;
    cont_notas_win = 0;
  }
}
//***************************************************************************************************************************************
// Función botones
//***************************************************************************************************************************************
void blink(void) 
{
if(cont > 0){
  cont--;
}
for(int i =0;i < 5; i++){
  FillRect(286, 70 + (i*30), 32, 32, 0x00);
        desplegar_nave(i);
    }
}
void blink2(){
  if(cont < 4){
  cont++;
}
for(int i =0;i < 5; i++){
    FillRect(286, 70 + (i*30), 32, 32, 0x00);
    if(jugando == 0){
    tipo_nave = !tipo_nave;
    }
      desplegar_nave(i); 
    }
}
void blink3(){
  if(jugando==0){
  FillRect(0, 0, 320, 240, 0x00);
    fondo_estrella();
  }
    jugando = 1;
}
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función iniciar timer 1
//***************************************************************************************************************************************
void configureTimer1A(){
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // iniciamos el clock del timar
  ROM_IntMasterEnable();                            //permitimos las interrupciones
  ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC); // configuramos el periodo de trabajo  
  // La configuracion de los hrz requeridos cambia segun el cristal de oscilacion 
  // en la tiva c por ejemplo si uno quiere un periodo de 1 HZ el custom value deve ser de 8Millosnes para que 8Mhz/8M = 1hz
  ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, 4000000); // en este caso se utilizaraun custom value de 80000 para que sea 100hz

  TimerIntRegister(TIMER1_BASE, TIMER_A, &Timer1AHandler); //se inicia el prototipo de funcion 
  ROM_IntEnable(INT_TIMER1A);  // Enable Timer 1A Interrupt
  ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT); // Timer 1A Interrupt when Timeout
  ROM_TimerEnable(TIMER1_BASE, TIMER_A); // Start Timer 1A
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
/*void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
*/

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;

  if (fontSize == 1) {
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if (fontSize == 2) {
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }

  char charInput ;
  int cLength = text.length();
  Serial.println(cLength, DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength + 1];
  text.toCharArray(char_array, cLength + 1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1) {
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2) {
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}


//***************************************************************************************************************************************
// Función para desplegar la nave en la pocision deseada
//***************************************************************************************************************************************
void desplegar_nave(int i){
  if(i == cont && jugando==1)
        switch (sel_nave) {
          case 0:
             LCD_Sprite(286, 70+ (i*30), 32,32,x_wing, 3, vida, 0, 0);
          break;
          case 1:
            LCD_Sprite(286, 70+ (i*30), 32,32,halcon_milenario, 3, vida, 0, 0);
          break;/*
          case 2:
            LCD_Sprite(286, 70+ (i*30), 32,32, a_wing, 3, vida, 0, 0);
          break;*/
}     
}

//***************************************************************************************************************************************
// funcion que limpia el mapa de los disparos
//***************************************************************************************************************************************

void clear_mapa(void){
     for(int i =0;i < 19; i++){
    carril_1_temp[i] = 0;
    carril_2_temp[i] = 0;
    carril_3_temp[i] = 0;
    carril_4_temp[i] =0;
    carril_5_temp[i] = 0;
    carril_1[i] = 0;
    carril_2[i] = 0;
    carril_3[i] = 0;
    carril_4[i] = 0;
    carril_5[i] = 0;
  }
}

void Timer1AHandler(void){
  ROM_TimerIntClear(TIMER1_BASE, TIMER_A);
}

void fondo_estrella(void){
  FillRect(0, 0, 320, 240, 0x00);
  for(int x = 0; x<240 ; x++){
  for(int i=0;i < 320;i++){
    LCD_Sprite(i, x, 20,20,estrella, 1, 0, 0, 0);
    i=i+19;
  };
  x = x+19;
  delay(5);
  };
}
