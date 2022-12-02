#include <util/delay.h>

#include "twi.h"

#define LCDVTWI_RS						(1 << 0)
#define LCDVTWI_RW						(1 << 1)
#define LCDVTWI_E						(1 << 2)
#define LCDVTWI_LED						(1 << 3)
#define LCDVTWI_DB4						(1 << 4)
#define LCDVTWI_DB5						(1 << 5)
#define LCDVTWI_DB6						(1 << 6)
#define LCDVTWI_DB7						(1 << 7)



#define lcdvtwi_rs_lo()					lcdvtwi_status &= ~LCDVTWI_RS
#define lcdvtwi_rs_hi()					lcdvtwi_status |= LCDVTWI_RS

#define lcdvtwi_rw_lo()					lcdvtwi_status &= ~LCDVTWI_RW
#define lcdvtwi_rw_hi()					lcdvtwi_status |= LCDVTWI_RW


#define lcdvtwi_e_lo()					lcdvtwi_status &= ~LCDVTWI_E
#define lcdvtwi_e_hi()					lcdvtwi_status |= LCDVTWI_E

#define lcdvtwi_led_lo()				lcdvtwi_status &= ~LCDVTWI_LED
#define lcdvtwi_led_hi()				lcdvtwi_status |= LCDVTWI_LED


#define HD44780_ENTRY_MODE				0x04
	#define HD44780_EM_SHIFT_CURSOR		0
	#define HD44780_EM_SHIFT_DISPLAY	1
	#define HD44780_EM_DECREMENT		0
	#define HD44780_EM_INCREMENT		2

#define HD44780_DISPLAY_ONOFF			0x08
	#define HD44780_DISPLAY_OFF			0
	#define HD44780_DISPLAY_ON			4
	#define HD44780_CURSOR_OFF			0
	#define HD44780_CURSOR_ON			2
	#define HD44780_CURSOR_NOBLINK		0
	#define HD44780_CURSOR_BLINK		1

#define HD44780_DISPLAY_CURSOR_SHIFT	0x10
	#define HD44780_SHIFT_CURSOR		0
	#define HD44780_SHIFT_DISPLAY		8
	#define HD44780_SHIFT_LEFT			0
	#define HD44780_SHIFT_RIGHT			4

#define HD44780_FUNCTION_SET			0x20
	#define HD44780_FONT5x7				0
	#define HD44780_FONT5x10			4
	#define HD44780_ONE_LINE			0
	#define HD44780_TWO_LINE			8
	#define HD44780_4_BIT				0
	#define HD44780_8_BIT				16

#define HD44780_CGRAM_SET				0x40

#define HD44780_DDRAM_SET				0x80

#define HD44780_CLEAR					0x01

#define LCDVTWI_ADDR					0x4E //adres uk³adu PCF8574 w tej p³ytce

char lcdvtwi_status;

void lcdvtwi_write_status(void);
void lcdvtwi_data_part(char data_part);
void lcdvtwi_write(char data);
void lcdvtwi_write_instruction(char instruction);
void lcdvtwi_write_data(char data);
void lcdvtwi_write_text(char *text);
void lcdvtwi_clear(void);
void lcdvtwi_set_cursor(char x, char y);
void lcdvtwi_led_on(void);
void lcdvtwi_led_off(void);
void lcdvtwi_init(void);
