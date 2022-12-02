/*
 * mkuart.h
 *
 *  Created on: 2010-09-04
 *       Autor: Miros³aw Kardaœ
 */

#ifndef MKUART_H_1
#define MKUART_H_1


#define UART_BAUD1 9600		// tu definiujemy interesuj¹c¹ nas prêdkoœæ
#define __UBRR1 ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1) // obliczamy UBRR dla U2X=0

// definicje na potrzeby RS485
#define UART_DE_PORT1 PORTD
#define UART_DE_DIR1 DDRD
#define UART_DE_BIT1 (1<<PD2)

#define UART_DE_ODBIERANIE1  UART_DE_PORT1 |= UART_DE_BIT1
#define UART_DE_NADAWANIE1  UART_DE_PORT1 &= ~UART_DE_BIT1


#define UART_RX_BUF_SIZE1 32 // definiujemy bufor o rozmiarze 32 bajtów
// definiujemy maskê dla naszego bufora
#define UART_RX_BUF_MASK1 ( UART_RX_BUF_SIZE1 - 1)

#define UART_TX_BUF_SIZE1 16 // definiujemy bufor o rozmiarze 16 bajtów
// definiujemy maskê dla naszego bufora
#define UART_TX_BUF_MASK1 ( UART_TX_BUF_SIZE1 - 1)




// deklaracje funkcji publicznych

void USART_Init1( uint16_t baud );

char uart_getc1(void);
void uart_putc1( char data );
void uart_puts1(char *s);
void uart_putint1(int value, int radix);

#endif /* MKUART_H_ */
