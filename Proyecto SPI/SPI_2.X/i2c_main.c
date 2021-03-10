// CONFIG1
#pragma config FOSC = EXTRC_CLKOUT// Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdio.h>
#include "UART.h"       //se llaman las librerias 
#include "MPU6050.h"    //la comunicacion I2c esta implicita en la libreria del MPU
//**************************Prototipos de funciones*****************************
void setup(void);
//**************************loop principal**************************************
void main(void) {
    
    
    while(1)
	{
        RC2 = ~RC2;     // Led cambia de estado
	    MPU6050_Read(); //lectura de datos del MPU6050
        __delay_ms(50); //se espera 50 ms
    }
    return;
}
//*****************************configuracion************************************
setup(){
    TRISC = 0;      // Puerto de leds como salidas
    
    PORTC = 0;      //inicialización de puertos en 0
    
    UART_TX_Init(); //inicializacion de la comunicacion uart TX
    MPU6050_Init(); //inicializacion de la libreria MPU6050
}