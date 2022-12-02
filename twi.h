#include <avr/io.h>

#define ACK 1
#define NOACK 0

// procedura transmisji sygna³u START
void twi_start(void);
// procedura transmisji sygna³u STOP
void twi_stop(void);
// procedura transmisji bajtu danych
void twi_write(char data);
//procedura odczytu bajtu danych
char twi_read(char ack);
