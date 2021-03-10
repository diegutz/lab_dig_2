
#include <xc.h>
#include "I2C.h"
#include "MPU6050.h"

#include "UART.h"  // for debugging serial terminal
#include <stdio.h>

void MPU6050_Init(void)         //iniciamos la comunicaci�n de mpu6050
{
  __delay_ms(100);              //esperamos para la iniciaci�n
  I2C_Master_Init();            //iniciamos la comunicaci�n I2C
 
  // Setting The Sample (Data) Rate
  I2C_Start(0xD0);              //enviamos la informaci�n del registro deseado
  I2C_Master_Write(SMPLRT_DIV); //seteamos la velocidad de transmisi�n de datos
  I2C_Master_Write(0x07);
  I2C_Master_Stop();
 
  // Setting The Clock Source
  I2C_Start(0xD0);
  I2C_Master_Write(PWR_MGMT_1);
  I2C_Master_Write(0x01);
  I2C_Master_Stop();
 
  // Configure The DLPF
  I2C_Start(0xD0);
  I2C_Master_Write(CONFIG);
  I2C_Master_Write(0x00);
  I2C_Master_Stop();
 
  // Configure The ACCEL (FSR= +-2g)
  I2C_Start(0xD0);
  I2C_Master_Write(ACCEL_CONFIG);
  I2C_Master_Write(0x00);
  I2C_Master_Stop();
 
  // Configure The GYRO (FSR= +-2000d/s)
  I2C_Start(0xD0);
  I2C_Master_Write(GYRO_CONFIG);
  I2C_Master_Write(0x18);
  I2C_Master_Stop();
 
  // Enable Data Ready Interrupts
  I2C_Start(0xD0);
  I2C_Master_Write(INT_ENABLE);
  I2C_Master_Write(0x01);
  I2C_Master_Stop();
}

void MPU6050_Read(void)
{
  char buffer[40];
  int Ax,Ay,Az,T,Gx,Gy,Gz;
  // Prepare For Reading, Starting From ACCEL_XOUT_H
  I2C_Start(0xD0);
  I2C_Master_Write(ACCEL_XOUT_H);
  I2C_Master_Stop();
  I2C_Start(0xD1);
  Ax = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Ay = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Az = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  T  = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gx = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gy = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gz = ((int)I2C_Read(0)<<8) | (int)I2C_Read(1);
  I2C_Master_Stop();
 
  sprintf(buffer,"Ax = %d    ",Ax);
  UART_Write_String(buffer);
 
  sprintf(buffer," Ay = %d    ",Ay);
  UART_Write_String(buffer);
 
  sprintf(buffer," Az = %d    ",Az);
  UART_Write_String(buffer);
 
  sprintf(buffer," T = %d  ",T);
  UART_Write_String(buffer);
 
  sprintf(buffer," Gx = %d    ",Gx);
  UART_Write_String(buffer);
 
  sprintf(buffer," Gy = %d    ",Gy);
  UART_Write_String(buffer);
 
  sprintf(buffer," Gz = %d\r\n",Gz);
  UART_Write_String(buffer);
}