#include "twi.h"
#include <avr/io.h>
// procedura transmisji sygna�u START
void twi_start(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

// procedura transmisji sygna�u STOP
void twi_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while ((TWCR & (1 << TWSTO)));
}

// procedura transmisji bajtu danych
void twi_write(char data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

//procedura odczytu bajtu danych
char twi_read(char ack)
{
	TWCR = ack ? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)) : ((1 << TWINT) | (1 << TWEN));
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}
