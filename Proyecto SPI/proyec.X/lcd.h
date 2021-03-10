#ifndef LCD_lib
#define LCD_lib

#define _XTAL_FREQ 4000000
//definimos los puertos y pines con los que bamos a trabajar
#define LCD_PORT PORTD
#define RS       PORTEbits.RE0
#define RW       PORTEbits.RE1
#define EN       PORTEbits.RE2

#include <xc.h>
#include <stdint.h>

void lcd_init(void);

void lcd_cmd(uint8_t a);

void lcd_clear(void);

void lcd_move_cursor(uint8_t row, uint8_t col);

void lcd_write_char(uint8_t a);

void lcd_write_string(uint8_t *a);

void lcd_shift_right(void);

void lcd_shift_left(void);

void lcd_port(uint8_t a);

#endif //LCD_lib