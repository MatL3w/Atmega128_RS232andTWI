/*
 * mkuart.c
 *
 *  Created on: 2010-09-04
 *       Autor: Mirosław Kardaś
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#include "mkuart2.h"


// definiujemy w ko�cu nasz bufor UART_RxBuf
volatile char UART_RxBuf1[UART_RX_BUF_SIZE1];
// definiujemy indeksy okre�laj�ce ilo�� danych w buforze
volatile uint8_t UART_RxHead1; // indeks oznaczaj�cy �g�ow� w�a�
volatile uint8_t UART_RxTail1; // indeks oznaczaj�cy �ogon w�a�



// definiujemy w ko�cu nasz bufor UART_RxBuf
volatile char UART_TxBuf1[UART_TX_BUF_SIZE1];
// definiujemy indeksy okre�laj�ce ilo�� danych w buforze
volatile uint8_t UART_TxHead1; // indeks oznaczaj�cy �g�ow� w�a�
volatile uint8_t UART_TxTail1; // indeks oznaczaj�cy �ogon w�a�






void USART_Init1( uint16_t baud ) {
	/* Ustawienie pr�dko�ci */
	UBRR1H = (uint8_t)(baud>>8);
	UBRR1L = (uint8_t)baud;
	/* Za��czenie nadajnika I odbiornika */
	UCSR1B = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	/* Ustawienie format ramki: 8bit�w danych, 1 bit stopu */
	UCSR1C = (3<<UCSZ0);


}

// procedura obs�ugi przerwania Tx Complete, gdy zostanie op�niony UDR
// kompilacja gdy u�ywamy RS485
#ifdef UART_DE_PORT1
ISR( USART1_TX_vect ) {
  UART_DE_PORT1 &= ~UART_DE_BIT1;	// zablokuj nadajnik RS485
}
#endif


// definiujemy funkcj� dodaj�c� jeden bajtdoz bufora cyklicznego
void uart_putc1( char data ) {
	uint8_t tmp_head;

    tmp_head  = (UART_TxHead1 + 1) & UART_TX_BUF_MASK1;

          // p�tla oczekuje je�eli brak miejsca w buforze cyklicznym na kolejne znaki
    while ( tmp_head == UART_TxTail1 ){}

    UART_TxBuf1[tmp_head] = data;
    UART_TxHead1 = tmp_head;

    // inicjalizujemy przerwanie wyst�puj�ce, gdy bufor jest pusty, dzi�ki
    // czemu w dalszej cz�ci wysy�aniem danych zajmie si� ju� procedura
    // obs�ugi przerwania
    UCSR1B |= (1<<UDRIE);
}


void uart_puts1(char *s)		// wysy�a �a�cuch z pami�ci RAM na UART
{
  register char c;
  while ((c = *s++)) uart_putc1(c);			// dop�ki nie napotkasz 0 wysy�aj znak
}

void uart_putint1(int value, int radix)	// wysy�a na port szeregowy tekst
{
	char string[17];			// bufor na wynik funkcji itoa
	itoa(value, string, radix);		// konwersja value na ASCII
	uart_puts1(string);			// wy�lij string na port szeregowy
}


// definiujemy procedur� obs�ugi przerwania nadawczego, pobieraj�c� dane z bufora cyklicznego
ISR( USART1_UDRE_vect)  {
    // sprawdzamy czy indeksy s� r�ne
    if ( UART_TxHead1 != UART_TxTail1 ) {
    	// obliczamy i zapami�tujemy nowy indeks ogona w�a (mo�e si� zr�wna� z g�ow�)
    	UART_TxTail1 = (UART_TxTail1 + 1) & UART_TX_BUF_MASK1;
    	// zwracamy bajt pobrany z bufora  jako rezultat funkcji
    	UDR1 = UART_TxBuf1[UART_TxTail1];
    } else {
	// zerujemy flag� przerwania wyst�puj�cego gdy bufor pusty
	UCSR1B &= ~(1<<UDRIE);
    }
}


// definiujemy funkcj� pobieraj�c� jeden bajt z bufora cyklicznego
char uart_getc1(void) {
    // sprawdzamy czy indeksy s� r�wne
    if ( UART_RxHead1 == UART_RxTail1 ) return 0;

    // obliczamy i zapami�tujemy nowy indeks �ogona w�a� (mo�e si� zr�wna� z g�ow�)
    UART_RxTail1 = (UART_RxTail1 + 1) & UART_RX_BUF_MASK1;
    // zwracamy bajt pobrany z bufora  jako rezultat funkcji
    return UART_RxBuf1[UART_RxTail1];
}

// definiujemy procedur� obs�ugi przerwania odbiorczego, zapisuj�c� dane do bufora cyklicznego
ISR( USART1_RX_vect ) {
    uint8_t tmp_head;
    char data;

    data = UDR1; //pobieramy natychmiast bajt danych z bufora sprz�towego

    // obliczamy nowy indeks �g�owy w�a�
    tmp_head = ( UART_RxHead1 + 1) & UART_RX_BUF_MASK1;

    // sprawdzamy, czy w�� nie zacznie zjada� w�asnego ogona
    if ( tmp_head == UART_RxTail1 ) {
    	// tutaj mo�emy w jaki� wygodny dla nas spos�b obs�u�y�  b��d spowodowany
    	// pr�b� nadpisania danych w buforze, mog�oby doj�� do sytuacji gdzie
    	// nasz w�� zacz��by zjada� w�asny ogon
    } else {
	UART_RxHead1 = tmp_head; 		// zapami�tujemy nowy indeks
	UART_RxBuf1[tmp_head] = data; 	// wpisujemy odebrany bajt do bufora
    }
}

