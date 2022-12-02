/*
 * main.c
 *
 *  Created on: 17 paü 2016
 *      Author: Lecik
 */
#include <avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>
#include "twi.h"
#include "lcdvtwi.h"
#include "mkuart.h"
#include "mkuart2.h"

//*************************************Funkcje*****************************************
void i2cSetBitrate(); 								// ustawienie prÍdkoúci TWI
void KOMENDY();										// przeszukuje bufor glowny odbiorczy w celu odnalezienia komend
void BUFOR();										// przekazje kolejny znak z bufora odbiorczego Uarta do bufora glownego
void CLEARbufor();									// czyúci bufor odbiorczy
void UARTget();										// funkcja przekazania znaku z urzπdzenie zewnÍtrznego do aplikacji
void CLEARmonitor();								// czyúci monitor
//************************************ zmienne*******************************************
volatile char bufor[2];
volatile char znakZbufora;
volatile char znakZbufora1;
volatile int  monitor;
volatile char adresy[64];
volatile char adresTWI;
volatile int wysylajPoTWI=0;
volatile int odbierajPoTWi=0;
volatile char tablicaDanychTWI[200];
volatile char tablicaADRESYTWI[200];
volatile int odczytTablicyTWI=0;
volatile int indeksTablicyTWi=0;
volatile int indeksAdresyTWi=0;


int main (void)
{
		USART_Init( __UBRR );						// Inicjalizacjia UART do komunikacji z aplikacjπ

		USART_Init1( __UBRR1 );						// Inicjalizacja UART do komunikacji z urzπdzeniem zewnetrznym

		i2cSetBitrate( 100 );						// ustawienie prÍdkoúci TWI

		sei();										// globalne odblokowanie przerwaÒ


		lcdvtwi_init();								//inicjalizacja wyúwietlacza
		lcdvtwi_led_on();
		lcdvtwi_clear();
		lcdvtwi_set_cursor(0,0 );
		lcdvtwi_write_text("Rq:");					//**
		lcdvtwi_write(OSCCAL);


	while(1)										// pÍtla g≥Ûwna programu
	{

		BUFOR(); 									//dodawnie nowego znaku z Rx0
		KOMENDY();									// sprawdzanie czy znaki tworza 'komende'
		UARTget();									// przekaznie znaku z Rx1 na Tx0 ( z urzadzenia zewnetrznego  na aplikacje)

	}


	}
void UARTget()										// funkcja przekazania znaku z urzπdzenie zewnÍtrznego do aplikacji
{
	znakZbufora1 = uart_getc1();
	if(znakZbufora1)
	{
		uart_putc(znakZbufora1);
		znakZbufora1=0;
	}
}
void CLEARmonitor()									// czyúci monitor
{
	if(monitor>12)
	{
		lcdvtwi_clear();
		lcdvtwi_write_text("RE:");
		monitor=0;
	}
}
void CLEARbufor()									// czyúci bufor odbiorczy
{
	bufor[1]=0;
	bufor[0]=0;
}
void KOMENDY()
{
	if(bufor[1]== 'R')								// dla [R] i [bufor[0]], 'bufor[0]' jako dana zostaje wyslana przez RS232
	{

		monitor +=2;
		CLEARmonitor();
		lcdvtwi_write(bufor[1]);
		lcdvtwi_write(bufor[0]);


		uart_putc1(bufor[0]);

		CLEARbufor();
	}
	if(bufor[1]== 'A')								// dla [A] i [bufor[0]], 'bufor[0]' zostaje zapisany jako adres ukladu w interfejsie TWI, natepnie po tym zmienia sie wartosc flagi 'wysylaj po TWI'
	{
		monitor +=2;

		CLEARmonitor();

		lcdvtwi_write(bufor[0]);

		adresTWI = bufor[0];

		wysylajPoTWI =1;

		CLEARbufor();
	}
	if(bufor[1]== 'S')								// dla [S] i [bufor[0]], 'bufor[0]' zostaje zapisany jako adres ukladu w interfejsie TWI, natepnie po tym zmienia sie wartosc flagi 'odbieraj po TWI'
		{
			monitor +=2;

			CLEARmonitor();

			lcdvtwi_write('r');

			lcdvtwi_write(bufor[0]);

			adresTWI = bufor[0];

			odbierajPoTWi=1;

			CLEARbufor();
		}

	if(bufor[1]=='X' && bufor[0]=='T')				//dla [X] i [T], sprawdza jakie adresy maja urzadzenia podpiete do szyny i w pojedynczych bajtach wysyla je na TX0(do aplikacji)
	{
		for(int i=0;i<200;i++)
		{
			tablicaADRESYTWI[i]=0;
		}
		indeksAdresyTWi=0;
		lcdvtwi_write(bufor[1]);
		lcdvtwi_write(bufor[0]);
		for(int i = 0; i<127; i+=2 )
		{
			_delay_ms(10);
			twi_start();
			twi_write(i);

			if(TWSR==0x18)
			{
				tablicaADRESYTWI[indeksAdresyTWi]=i;
				indeksAdresyTWi++;
			}
			twi_stop();
		}
		for(int i=0;i<indeksAdresyTWi;i++)
		{
			lcdvtwi_write(tablicaADRESYTWI[i]);
			uart_putc(tablicaADRESYTWI[i]);
		}

		uart_putc('c');
		CLEARbufor();
	}
	if(wysylajPoTWI && bufor[1] == 'T')				//dla [T] i [bufor[0]]  jesli wczensiej dostalo
	{
		twi_start();
		twi_write(adresTWI);
		twi_write(bufor[0]);
		twi_stop();
		wysylajPoTWI=0;
		adresTWI=0;
		CLEARbufor();
	}
	if(odbierajPoTWi && bufor[1] == 'D')			//dla [D] i[D], wysy≥a do aplikacji danπ ktÛrπ odebra≥o
		{
		char szybka;
		monitor +=1;
			twi_start();
			twi_write(adresTWI);
			twi_start();
			twi_write(adresTWI +1);
			szybka =twi_read(0);
			twi_stop();
			odbierajPoTWi=0;
			adresTWI=0;
			uart_putc(szybka);
			lcdvtwi_write(szybka);
			CLEARbufor();
			CLEARmonitor();
		}
	if(bufor[1]=='W')							//komenda dla WYSYLANIA CIAGLEGO W TWI
		{
		adresTWI = bufor[0];
		odczytTablicyTWI =1;
		CLEARbufor();
		}
	if(odczytTablicyTWI && bufor[1]=='$')
	{
		if(indeksTablicyTWi<=200)
		{
			tablicaDanychTWI[indeksTablicyTWi]=bufor[0];
			lcdvtwi_write(bufor[0]);
			indeksTablicyTWi++;
			CLEARbufor();
		}
	}
	if(bufor[1]=='c' && bufor[0] == 'c')
	{
		lcdvtwi_write(indeksTablicyTWi);
		odczytTablicyTWI=0;
		twi_start();
		twi_write(adresTWI);
		for(int i =0;i<indeksTablicyTWi;i++)
		{
		twi_write(tablicaDanychTWI[i]);
		tablicaDanychTWI[i]=0;
		}
		twi_stop();
		indeksTablicyTWi=0;
		CLEARbufor();
	}

}
void BUFOR()
{
	znakZbufora=uart_getc();
	if(znakZbufora)
	{
		bufor[1]=bufor[0];
		bufor[0]=znakZbufora;
		znakZbufora=0;
	}

}
void i2cSetBitrate(uint16_t bitrateKHz) {

	uint8_t bitrate_div;

	bitrate_div = ((F_CPU/1000l)/bitrateKHz);
	if(bitrate_div >= 16)
		bitrate_div = (bitrate_div-16)/2;

	TWBR = bitrate_div;
}
ISR( TWI_vect )
{
	switch (TWSR)
	{


	}



}
