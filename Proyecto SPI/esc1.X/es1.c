
//******************************************************************************
// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config MCLRE = OFF      
#pragma config CP = OFF         
#pragma config CPD = OFF        
#pragma config BOREN = OFF      
#pragma config IESO = OFF       
#pragma config FCMEN = OFF      
#pragma config LVP = OFF        

// CONFIG2
#pragma config BOR4V = BOR40V   
#pragma config WRT = OFF        

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//******************************************************************************
// Includes
//******************************************************************************


#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "SPIe1.h"

//******************************************************************************
// Defines
//******************************************************************************

#define _XTAL_FREQ 4000000

#define tmr_preload 6;

//******************************************************************************
// Variables
//******************************************************************************

uint8_t adc_data1 = 0;
uint8_t adc_data2 = 0;

bool    adc_flag     = false;

//******************************************************************************
// function declarations
//******************************************************************************

void setup(void);
void canal_12(void);
void ADC_1(void);

//******************************************************************************
// interrupcion
//******************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){
       SSPBUF=adc_data1;
       SSPIF = 0;         
    }
}

//******************************************************************************
// Main
//******************************************************************************

void main(void)
{
    setup();
    while(1) 
    {
        if (ADCON0bits.GO_DONE == 0) {
            ADC_1();
        }
        
    }
}

void canal_12(void){
    //SELECIONAMOS EL CANAL AN10
    //an10 1010
    ADCON0bits.CHS3 = 1;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 1;
    ADCON0bits.CHS0 = 0;
}
void ADC_1(void){
    adc_data1 = ADRESH;
    ADCON0bits.GO_DONE =1;
    PIR1bits.ADIF = 0;
}
//******************************************************************************
// Configuration
//******************************************************************************
void setup(void)
{
    ANSEL  = 0x00;
    ANSELH = 0x00;
    
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x80;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    // interrupts
    IOCB = 0x00;
    INTCONbits.RBIE = 0;
    INTCONbits.T0IE = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    OSCCON = 0b01100001;

    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;

    TMR0 = tmr_preload;

    INTCONbits.T0IF = 0;

    ///CONFIGURACION DEL ADC 
    ANSELHbits.ANS10 = 1;
    ANSELHbits.ANS12 = 1;
    
    //SELECIONAMOS EL CANAL 
    
    canal_12();

    //PINES DE REFERENCIA 
    ADCON1bits.VCFG0 = 0; //VDD
    ADCON1bits.VCFG1 = 0; //VSS
    
    //JUSTIFICACION IZQUIERDA 
    ADCON1bits.ADFM = 0;


    //PARA CONVERSION DEL ADC
    ADCON0bits.GO_DONE = 1;

    //PARA HABILITAR ADC 
    ADCON0bits.ADON = 1;

    //ENCENDER BANDERA DEL ADC
    INTCONbits.GIE = 1;//INTERRUPCIONES GLOBALES
    PIE1bits.ADIE = 0;
    PIE1bits.ADIE = 1;// interrupciones del ADC
    TRISAbits.TRISA5 = 1;
    spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    return;
}