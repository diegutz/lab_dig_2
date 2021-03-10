
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
#include "lcd.h"
#include "eusart.h"
#include "SPI.h"

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
uint8_t cont      = 0;
uint8_t uart_data = 0;
uint8_t uart_cont = 0;
uint8_t str_pos   = 0;

int ban_an1;
int ban_an2;
int ss;

// IMPORTANT
// for som reason if both string have the same length
// the las char of str_pot_b overwrites the first char of str_pot_a
char* str_pot_a[6];
char* str_pot_b[5];

bool    adc_flag     = false;
bool    eusart_flag  = false;

//******************************************************************************
// function declarations
//******************************************************************************

void setup(void);
void adc_logic(void);
void display(void);
void canal_10(void);
void canal_12(void);
void ADC_E1(void);
void ADC_E3(void);

//******************************************************************************
// Main
//******************************************************************************

void main(void)
{
    setup();
    
    PORTAbits.RA0 = 1;
    PORTAbits.RA1 = 1;
    PORTAbits.RA2 = 1;
    while(1) 
    {
        PORTAbits.RA0 = 0;
        __delay_ms(5);
        adc_data1 = spiRead();
        __delay_ms(5);
        PORTAbits.RA0 = 1;
        PORTAbits.RA1 = 0;
        __delay_ms(5);
        cont = spiRead();
        __delay_ms(5);
        PORTAbits.RA1 = 1;
        PORTAbits.RA2 = 0;
        __delay_ms(5);
        adc_data2 = spiRead();
        __delay_ms(5);
        PORTAbits.RA2 = 1;
        sprintf(str_pot_a, "A%.3iV", adc_data1<<1);
        sprintf(str_pot_b, "B%.3iV", adc_data2<<1);

        display();

        uart_data = adc_data1;

    }
}

void __interrupt() isr(void)
{

    if (PIE1bits.TXIE && PIR1bits.TXIF)
    {
        if (eusart_flag)
        {
            TXREG = str_pot_a[str_pos];
        }
        else
        {
            TXREG = str_pot_b[str_pos];
        }

        if (str_pos == 5)
        {
            eusart_flag = !eusart_flag;
            str_pos = 0;
        }
    }

    if (PIR1bits.RCIF)
    {
        uart_data = RCREG;
    }

}

void display(void)
{
    lcd_move_cursor(0,0);
    lcd_write_string("  S1:  S2:  S3: ");
    lcd_move_cursor(1,0);

    char* str[3];

    sprintf(str, "%.3i", uart_cont);

    lcd_write_char(str_pot_a[1]);
    lcd_write_char('.');
    lcd_write_char(str_pot_a[2]);
    lcd_write_char(str_pot_a[3]);
    lcd_write_char(str_pot_a[4]);
    lcd_write_char(' ');

    lcd_write_char(str_pot_b[1]);
    lcd_write_char('.');
    lcd_write_char(str_pot_b[2]);
    lcd_write_char(str_pot_b[3]);
    lcd_write_char(str_pot_b[4]);
    lcd_write_char(' ');

    lcd_write_char(str[0]);
    lcd_write_char(str[1]);
    lcd_write_char(str[2]);
    lcd_write_char(str[3]);
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
/*
    adc_config();
    adc_isr_enable();
*/
    lcd_init();
    lcd_cmd(0x0c); // turn off cursor

    eusart_init_tx();
    eusart_enable_tx_isr();

    eusart_init_rx();
    eusart_enable_rx_isr();
        
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    return;
}