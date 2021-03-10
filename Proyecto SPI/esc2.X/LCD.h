//LCD Functions Developed by electroSome
#ifndef LCD_H
#define	LCD_H

#ifndef  _XTAL_FREQ
#define  _XTAL_FREQ 8000000
#endif  

#ifndef  RS
#define RS RD2
#endif

#ifndef EN
#define EN RD3
#endif

#ifndef  D4
#define D4 RD4
#endif

#ifndef  D5
#define D5 RD5
#endif

#ifndef  D6
#define D6 RD6
#endif

#ifndef  D7
#define D7 RD7
#endif


#include <xc.h> 

void Lcd_Port(char a);

void Lcd_Cmd(char a);

void Lcd_Clear(void);

void Lcd_Set_Cursor(char a, char b);

void Lcd_Init(void);

void Lcd_Write_Char(char a);

void Lcd_Write_String(char *a);

void Lcd_Shift_Right(void);

void Lcd_Shift_Left(void);
#endif
