#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"

//***************************************
#ifdef DEBUG
void
__error_(char *pcfilename, uint32_t ui32Line)
{
    while(1);

}
#endif
/**
 * main.c
 */
#define XTAL 16000000
void InitUART(void);
void initGPIOF (void);
unsigned char letra;
//definimos las variables del programa
uint8_t RED;
uint8_t BLUE;
uint8_t GREEN;
uint8_t cuatro;
uint8_t bandera;
uint8_t parqueo_1;
uint8_t parqueo_2;
uint8_t parqueo_3;
uint8_t parqueo_4;
uint8_t cont_p1;
uint8_t cont_p2;
uint8_t cont_p3;
uint8_t cont_p4;
uint8_t cont_total;
//********************************prototipos de funcion*******************************
void Timer0Config(void);
void InitUART(void);
//************************************inicio Main**********************************************
int main(void)
{
    //activamos la se?al de reloj y la configuramos
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    //estas lineas configuran el reloj para que active los puertos a,e,f y c
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    //declaramos los pines que van a ser salidas y entradas
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,  GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
    //configuramos los botones para tener resistencias pull up internas
    GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_5,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    //llamamos a la configuracion del timer 0
    Timer0Config();
    //llamamos a la configuracion del UART0
    InitUART();

//*********************************loop*********************************************************
//************esta vacio porque solo nos vamos a manejar con interrupciones*********************
    while(1)
    {
        //leemos los botones y los guardamos en una variable
        parqueo_1 = GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_2);
        parqueo_2 = GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_3);
        parqueo_3 = GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_4);
        parqueo_4 = GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_5);

        if( (parqueo_1 & GPIO_PIN_2)==0){  //leemos la entrada del parqueo
        GREEN = 1;      //si el boton se preciona activa la bandera de sus leds correspondientes
        cont_p1 = 1;    //si se presiona el boton se pone en uno la bandera para contar
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4);
            }
        else{
        GREEN = 0;      //si el boton no se preciona se apaga la bandera de los led
        cont_p1 = 0;    //si no se preciona el boton el contador del parqueo se pone en 0
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
            }

        if ((parqueo_2 & GPIO_PIN_3)==0){    //leemos la entrada del parqueo
        BLUE = 1;       //si el boton se preciona activa la bandera de sus leds correspondientes
        cont_p2 = 1;    //si se presiona el boton se pone en uno la bandera para contar
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);
            }
        else{
        BLUE = 0;      //si el boton no se preciona se apaga la bandera de los led
        cont_p2 = 0;    //si no se preciona el boton el contador del parqueo se pone en 0
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
            }
        if( (parqueo_3 & GPIO_PIN_4)==0){    //leemos la entrada del parqueo
        RED = 1;        //si el boton se preciona activa la bandera de sus leds correspondientes
        cont_p3 = 1;    //si se presiona el boton se pone en uno la bandera para contar
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);
            }
        else{
        RED = 0;        //si el boton no se preciona se apaga la bandera de los led
        cont_p3 = 0;    //si no se preciona el boton el contador del parqueo se pone en 0
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
            }

        if ((parqueo_4 & GPIO_PIN_5)==0){    //leemos la entrada del parqueo
        cuatro = 1;     //si el boton se preciona activa la bandera de sus leds correspondientes
        cont_p4 = 1;    //si se presiona el boton se pone en uno la bandera para contar
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
            }
        else{
        cuatro = 0;     //si el boton no se preciona se apaga la bandera de los led
        cont_p4 = 0;    //si no se preciona el boton el contador del parqueo se pone en 0
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
            }

        //se suma todos los contadores para saber cuantos parqueos estan ocupados
        cont_total = cont_p1 + cont_p2 + cont_p3 + cont_p4;


    }
}

//configuracion del timer0
void Timer0Config(void){

    uint32_t ui32Period;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
    {
    }
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    ui32Period = ( SysCtlClockGet() ) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);


    IntEnable(INT_TIMER0A);   // Configuro las interrupciones
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // Configuro las interrupciones
    IntMasterEnable();   // Configuro las interrupciones


    TimerEnable(TIMER0_BASE, TIMER_A); // timers.
}
//inicializamos la comunicacion UART
void InitUART(void){


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Enable the GPIO Port A

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Enable the peripheral UART Module 0


    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //asignamos un baulrate al uart para la comunicacion serial
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(INT_UART0);
    UARTEnable(UART0_BASE);

    UARTCharPut(UART0_BASE,'L');
    UARTCharPut(UART0_BASE,'A');
    UARTCharPut(UART0_BASE,'B');
    UARTCharPut(UART0_BASE,'9');
    UARTCharPut(UART0_BASE,' ');

}
//*************************UART**************************************************
void UARTIntHandler(void){

    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT);  // Limpiar bandera
    letra = UARTCharGet(UART0_BASE);  // guardar la entrada en una variable

    //comparamos la entrada con las variables esperadas

}

//*************************TIMER 0**************************************************
void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //apagamos la bandera de timer 0 para que el conteo inicie nuevamente

    //el valor de la bandera alternara entre 0 y 1 con cada interrupcion paara dar la imprecion de parpadeo


    //si el valor de la bandera es uno se activa este porceso
    if (bandera == 1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
        bandera = 0;    //se cambia el valor de la bandera cambia a 0
    }
    //si el valor de la bandera es uno se activa este proceso

    else
    {
        bandera = 1;//se cambia el valor de 0 a uno
        if(RED)     //chequeamos la bandera de rojo
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);//si estamos en uno encendemos el pin rojo
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);//si esta en 0 apagamos el led
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);//si esta en 0 apagamos el led
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_PIN_7);
        }
        if(BLUE == 1)
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);//si estamos en uno encendemos el pin rojo
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);//si esta en 0 apagamos el led
        }
        else if(BLUE == 0)
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);//si estamos en uno encendemos el pin rojo
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);//si esta en 0 apagamos el led
        }
        if(GREEN == 1)
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);//si estamos en uno encendemos el pin rojo
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);//si esta en 0 apagamos el led
        }
        else if(GREEN == 0)
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);//si estamos en uno encendemos el pin rojo
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);//si esta en 0 apagamos el led
        }
        if(cuatro)
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4);//si estamos en uno encendemos el pin rojo
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);//si esta en 0 apagamos el led
        }
        else
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5);//si estamos en uno encendemos el pin rojo
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);//si esta en 0 apagamos el led
                }
    }
}
