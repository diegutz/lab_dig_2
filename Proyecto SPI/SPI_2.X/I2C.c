
#include <xc.h>
#include "I2C.h"


//---------------[ I2C Routines ]-------------------
//--------------------------------------------------

void I2C_Master_Init()      //inicializacion de la comunicacion i2c
{
  SSPCON  = 0x28;           //iniciamos los registros de i2c
  SSPCON2 = 0x00;
  SSPSTAT = 0x00;
  SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;
  SCL_D = 1;                //inicializamos SCL(trisc3)
  SDA_D = 1;                //inicializamos SDA(trisc4)
}

void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));   //se espera las banderas de I2C
}

void I2C_Master_Start()     //iniciamos el master
{
    I2C_Master_Wait();      //esperamos  la bandera
    SEN = 1;                //inicia la comunicacion    
}

void I2C_Start(char add)    //comienza i2c y manda datos
{
    I2C_Master_Wait();      //esperamos la bandera
    SEN = 1;                //empezamos la comunicación
    I2C_Master_Write(add);  
}

void I2C_Master_RepeatedStart()     //iniciamos nuevamente la comunicación
{
    I2C_Master_Wait();              //esperamos la bandera
    RSEN = 1;                       //iniciamos comunicacion
}

void I2C_Master_Stop()      //detener la comunicacion
{
    I2C_Master_Wait();      //esperamos la bandera
    PEN = 1;                //terminamos la comunicación
}

void I2C_ACK(void)
{
	ACKDT = 0;			// 0 -> ACK
    ACKEN = 1;			// mandamos ACK
    while(ACKEN);
}

void I2C_NACK(void)
{
	ACKDT = 1;			// 1 -> NACK
	ACKEN = 1;			// mandamos NACK
    while(ACKEN);
}

unsigned char I2C_Master_Write(unsigned char data)
{
    I2C_Master_Wait();  //esperamos la bandera
    SSPBUF = data;      //cargamos un dato al bus de envios 
    while(!SSPIF);      //esperamos la bandera
	SSPIF = 0;          //limpiamos la bandera de envio
    return ACKSTAT;
}

unsigned char I2C_Read_Byte(void)
{
    I2C_Master_Wait();//esperamos la bandera
    RCEN = 1;		  // Iniciamos la comunicación
	while(!SSPIF);	  //esperamos la bandera
	SSPIF = 0;		  // Limpiamos la bandera de envio
    I2C_Master_Wait();//esperamos la bandera
    return SSPBUF;	  // retornamos la informacion enviada
}

unsigned char I2C_Read(unsigned char ACK_NACK)
{  
   
    unsigned char Data;     //creamos una variable para la ocación
    RCEN = 1;               //iniciamos lectura
    while(!BF);             //esperamos la bandera
    Data = SSPBUF;          //montamos el bus de lectura en la variable
    if(ACK_NACK==0)         
        I2C_ACK();            
    else
        I2C_NACK();     
    while(!SSPIF);          //esperamos la bandera      
    SSPIF=0;                //terminamos la comunicación
    return Data;            //retornamos la varable 
}



