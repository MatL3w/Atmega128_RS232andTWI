/*
 * mkuart.c
 *
 *  Created on: 2010-09-04
 *       Autor: Miros≈Çaw Karda≈õ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#include "mkuart2.h"


// definiujemy w koÒcu nasz bufor UART_RxBuf
volatile char UART_RxBuf1[UART_RX_BUF_SIZE1];
// definiujemy indeksy okreúlajπce iloúÊ danych w buforze
volatile uint8_t UART_RxHead1; // indeks oznaczajπcy Ñg≥owÍ wÍøaî
volatile uint8_t UART_RxTail1; // indeks oznaczajπcy Ñogon wÍøaî



// definiujemy w koÒcu nasz bufor UART_RxBuf
volatile char UART_TxBuf1[UART_TX_BUF_SIZE1];
// definiujemy indeksy okreúlajπce iloúÊ danych w buforze
volatile uint8_t UART_TxHead1; // indeks oznaczajπcy Ñg≥owÍ wÍøaî
volatile uint8_t UART_TxTail1; // indeks oznaczajπcy Ñogon wÍøaî






void USART_Init1( uint16_t baud ) {
	/* Ustawienie prÍdkoúci */
	UBRR1H = (uint8_t)(baud>>8);
	UBRR1L = (uint8_t)baud;
	/* Za≥πczenie nadajnika I odbiornika */
	UCSR1B = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	/* Ustawienie format ramki: 8bitÛw danych, 1 bit stopu */
	UCSR1C = (3<<UCSZ0);


}

// procedura obs≥ugi przerwania Tx Complete, gdy zostanie opÛøniony UDR
// kompilacja gdy uøywamy RS485
#ifdef UART_DE_PORT1
ISR( USART1_TX_vect ) {
  UART_DE_PORT1 &= ~UART_DE_BIT1;	// zablokuj nadajnik RS485
}
#endif


// definiujemy funkcjÍ dodajπcπ jeden bajtdoz bufora cyklicznego
void uart_putc1( char data ) {
	uint8_t tmp_head;

    tmp_head  = (UART_TxHead1 + 1) & UART_TX_BUF_MASK1;

          // pÍtla oczekuje jeøeli brak miejsca w buforze cyklicznym na kolejne znaki
    while ( tmp_head == UART_TxTail1 ){}

    UART_TxBuf1[tmp_head] = data;
    UART_TxHead1 = tmp_head;

    // inicjalizujemy przerwanie wystÍpujπce, gdy bufor jest pusty, dziÍki
    // czemu w dalszej czÍúci wysy≥aniem danych zajmie siÍ juø procedura
    // obs≥ugi przerwania
    UCSR1B |= (1<<UDRIE);
}


void uart_puts1(char *s)		// wysy≥a ≥aÒcuch z pamiÍci RAM na UART
{
  register char c;
  while ((c = *s++)) uart_putc1(c);			// dopÛki nie napotkasz 0 wysy≥aj znak
}

void uart_putint1(int value, int radix)	// wysy≥a na port szeregowy tekst
{
	char string[17];			// bufor na wynik funkcji itoa
	itoa(value, string, radix);		// konwersja value na ASCII
	uart_puts1(string);			// wyúlij string na port szeregowy
}


// definiujemy procedurÍ obs≥ugi przerwania nadawczego, pobierajπcπ dane z bufora cyklicznego
ISR( USART1_UDRE_vect)  {
    // sprawdzamy czy indeksy sπ rÛøne
    if ( UART_TxHead1 != UART_TxTail1 ) {
    	// obliczamy i zapamiÍtujemy nowy indeks ogona wÍøa (moøe siÍ zrÛwnaÊ z g≥owπ)
    	UART_TxTail1 = (UART_TxTail1 + 1) & UART_TX_BUF_MASK1;
    	// zwracamy bajt pobrany z bufora  jako rezultat funkcji
    	UDR1 = UART_TxBuf1[UART_TxTail1];
    } else {
	// zerujemy flagÍ przerwania wystÍpujπcego gdy bufor pusty
	UCSR1B &= ~(1<<UDRIE);
    }
}


// definiujemy funkcjÍ pobierajπcπ jeden bajt z bufora cyklicznego
char uart_getc1(void) {
    // sprawdzamy czy indeksy sπ rÛwne
    if ( UART_RxHead1 == UART_RxTail1 ) return 0;

    // obliczamy i zapamiÍtujemy nowy indeks Ñogona wÍøaî (moøe siÍ zrÛwnaÊ z g≥owπ)
    UART_RxTail1 = (UART_RxTail1 + 1) & UART_RX_BUF_MASK1;
    // zwracamy bajt pobrany z bufora  jako rezultat funkcji
    return UART_RxBuf1[UART_RxTail1];
}

// definiujemy procedurÍ obs≥ugi przerwania odbiorczego, zapisujπcπ dane do bufora cyklicznego
ISR( USART1_RX_vect ) {
    uint8_t tmp_head;
    char data;

    data = UDR1; //pobieramy natychmiast bajt danych z bufora sprzÍtowego

    // obliczamy nowy indeks Ñg≥owy wÍøaî
    tmp_head = ( UART_RxHead1 + 1) & UART_RX_BUF_MASK1;

    // sprawdzamy, czy wπø nie zacznie zjadaÊ w≥asnego ogona
    if ( tmp_head == UART_RxTail1 ) {
    	// tutaj moøemy w jakiú wygodny dla nas sposÛb obs≥uøyÊ  b≥πd spowodowany
    	// prÛbπ nadpisania danych w buforze, mog≥oby dojúÊ do sytuacji gdzie
    	// nasz wπø zaczπ≥by zjadaÊ w≥asny ogon
    } else {
	UART_RxHead1 = tmp_head; 		// zapamiÍtujemy nowy indeks
	UART_RxBuf1[tmp_head] = data; 	// wpisujemy odebrany bajt do bufora
    }
}

