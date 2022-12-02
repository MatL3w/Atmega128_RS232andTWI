#include "lcdvtwi.h"

char lcdvtwi_status = 0X00;

void lcdvtwi_write_status(void)
{
	twi_start();
	twi_write(LCDVTWI_ADDR);
	twi_write(lcdvtwi_status);
	twi_stop();
}

void lcdvtwi_data_part(char data_part)
{
	lcdvtwi_status &= ~LCDVTWI_DB4 & ~LCDVTWI_DB5 & ~LCDVTWI_DB6 & ~LCDVTWI_DB7;
	if(data_part & 0x01) lcdvtwi_status |= LCDVTWI_DB4;
	if(data_part & 0x02) lcdvtwi_status |= LCDVTWI_DB5;
	if(data_part & 0x04) lcdvtwi_status |= LCDVTWI_DB6;
	if(data_part & 0x08) lcdvtwi_status |= LCDVTWI_DB7;
}

void lcdvtwi_write(char data)
{
	lcdvtwi_e_hi();
	lcdvtwi_data_part(data >> 4);
	lcdvtwi_write_status();
	lcdvtwi_e_lo();
	lcdvtwi_write_status();
	lcdvtwi_e_hi();
	lcdvtwi_data_part(data);
	lcdvtwi_write_status();
	lcdvtwi_e_lo();
	lcdvtwi_write_status();
	_delay_ms(2);
}

void lcdvtwi_write_instruction(char instruction)
{
	lcdvtwi_rw_lo();
	lcdvtwi_rs_lo();
	lcdvtwi_write(instruction);
}

void lcdvtwi_write_data(char data)
{
	lcdvtwi_rs_hi();
	lcdvtwi_write(data);
}

void lcdvtwi_write_text(char *text)
{
	while(*text) lcdvtwi_write_data(*text++);
}

void lcdvtwi_clear(void)
{
	lcdvtwi_write_instruction(0x01);
}

void lcdvtwi_set_cursor(char x, char y)
{
	lcdvtwi_write_instruction(HD44780_DDRAM_SET | (x + (0x40 * y)));
}

void lcdvtwi_led_on(void)
{
	lcdvtwi_led_hi();
	lcdvtwi_write_status();
}

void lcdvtwi_led_off(void)
{
	lcdvtwi_led_lo();
	lcdvtwi_write_status();
}

void lcdvtwi_init(void)
{
	char i;
	_delay_ms(15);
	for(i = 0; i < 3; i++)
	{
		lcdvtwi_data_part(0x03);
		lcdvtwi_e_hi();
		lcdvtwi_write_status();
		
		lcdvtwi_e_lo();
		lcdvtwi_write_status();
		_delay_ms(4);
	}
	lcdvtwi_data_part(0x02);
	lcdvtwi_e_hi();
	lcdvtwi_write_status();
	
	lcdvtwi_e_lo();
	lcdvtwi_write_status();
	_delay_ms(1);
	
	lcdvtwi_write_instruction(HD44780_FUNCTION_SET | HD44780_FONT5x10 | HD44780_TWO_LINE | HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
	lcdvtwi_write_instruction(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wy³¹czenie wyswietlacza
	lcdvtwi_write_instruction(HD44780_CLEAR); // czyszczenie zawartosæi pamieci DDRAM
	_delay_ms(2);
	lcdvtwi_write_instruction(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);// inkrementaja adresu i przesuwanie kursora
	lcdvtwi_write_instruction(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK); // w³¹cz LCD, bez kursora i mrugania
}
