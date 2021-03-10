#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "SPIe2.h"

//******************************************************************************
//CONFIGURACION
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_CLKOUT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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

#define _XTAL_FREQ 4000000

//******************************************************************************
//VARIABLES
//******************************************************************************
int ban_1;          //banderas para el anti rebote de cada una de los botones
int ban_2;
int cont1;          //contador de pulsos de cada boton

//******************************************************************************
//PROTOTIPOS DE FUNCIONES
//******************************************************************************
void setup(void);       //Funcion para definir la configuracion inicial
void avanza_1(void);    //la funcion avanza chequea el valor del contador
void fin(void);         //verifica si algun jugador llego a la meta para finali-
                        //zar la carrera


void __interrupt() isr(void){ 
   if(PIR1bits.SSPIF == 1){
       SSPBUF=cont1;
       SSPIF = 0;         
    }
}
//******************************************************************************
//CICLO PRINCIPAL
//******************************************************************************

void main(void) {

    setup();

    //**************************************************************************
    //LOOP PRINCIPAL
    //**************************************************************************

    while (1) {
        

        if (PORTBbits.RB1 == 0) {   //si el boton esta desactivado coloca la 
            ban_1 = 1;              //bandera para permitir el aumento
        }
        if (PORTBbits.RB1 == 1 && ban_1 == 1) { //chequea el boton y la bandera
            ban_1 = 0;      //tanto la bandera como el boton deben estar en la
            cont1++;        //configuracion correcta para funcionar y 
        }                   //aumentar el contador 1
        
        if (PORTBbits.RB2 == 0) {   //si el boton esta desactivado coloca la
            ban_2 = 1;              //bandera para permitir el aumento
        }
        if (PORTBbits.RB2 == 1 && ban_2 == 1) {//chequea el boton y la bandera
            ban_2 = 0;       //tanto la bandera como el boton deben estar en la
            cont1--;         //configuracion correcta para funcionar y 
        }                    //aumentar el contador 2
        
        avanza_1();         //verifica cual es el valor del contador 1
        fin();              //chequea si un jugador ya gano


    }
}

//******************************************************************************
//CONFIGURACION
//******************************************************************************

void setup(void) {
    TRISE = 0; // todos las salidas del puerto E estan en OUTPUT
    PORTE = 0; // Todos los puertos de E empiezan apagados
    TRISC = 0; // TODO C esta en OUTPUT
    PORTC = 0; // TODO C empieza apagado
    TRISA = 0; // TODO A OUTPUT
    PORTA = 0; // TODA A APAGADO
    TRISD = 0; // TODO D OUTPUT
    PORTD = 0; // TODO D EMPIEZA APAGADO
    ANSEL = 0; // PARA NO USARLO COMO ANALOGICO
    ANSELH = 0; // PARA NO USARLO COMO ANALOGICO
    PORTBbits.RB0 = 1; //Puerto RB0 como input
    PORTBbits.RB1 = 1; 
    TRISAbits.TRISA5 = 1;
    spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

//******************************************************************************
//FUNCIONES
//******************************************************************************

void avanza_1(void) {
    PORTD = cont1;
}

void fin(void) {
    if (cont1 >= 255) {   //si el contador 1 finaliza empieza la secuencia de fin
        cont1 = 0;
    }
}
