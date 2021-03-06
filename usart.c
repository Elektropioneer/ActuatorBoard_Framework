/**********************************************************************************
* File Name			: usart.c
* Description       : This .c file is initing and declaring functions for working
* 						with usart
**********************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

/****************************************************************************************************************************************
Inicijalizacija USART1 modula:
-predajnik
-asinhroni
-1 stop bit
-bez paritet bita
Ali ga koristimo i za arduino komunikaciju za koju moramo imati:
-8bit
-2 stop bit
-bez paritet
-9600baud
-asynchron
****************************************************************************************************************************************/
void UART1_Init(unsigned long baud, char isrOnOff)
{
	unsigned int temp;

	UCSR1A = 0;
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);


	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10); //sa 1stop bit


	if(isrOnOff == UART_ISR_ON)
	{
		UCSR1B |= ((1 << RXCIE1) | (1 << TXCIE1));
		SREG |= 0x80;
	}

	temp = 0.5 + ((double)F_CPU/(baud * 16.0f) - 1);

	UBRR1H = temp >> 8;
	UBRR1L = temp;
}


#define TX1_BUFFER_SIZE 	64
static volatile unsigned char tx1_buffer[TX1_BUFFER_SIZE];
static volatile unsigned char tx1_wr_index=0;
static volatile unsigned char tx1_counter=0;
static volatile unsigned char tx1_rd_index=0;

#define RX1_BUFFER_SIZE 	64
static volatile unsigned char rx1_buffer[64];
static volatile unsigned char rx1_wr_index = 0;
static volatile unsigned char rx1_counter = 0;
static volatile unsigned char rx1_rd_index = 0;

/****************************************************************************************************************************************
Transmit interrupt USART1 modula
****************************************************************************************************************************************/
//#error "Linker error- idiotic programming"
ISR(USART1_TX_vect)
{
	if(tx1_counter)
	{
		tx1_counter--;
		UDR1=tx1_buffer[tx1_rd_index];
		if(++tx1_rd_index == TX1_BUFFER_SIZE)
			tx1_rd_index = 0;
	}
}

ISR(USART1_RX_vect)
{
	unsigned char status;
	status = UCSR1A;

	if(!(status & (FRAME_ERROR | DATA_OVERRUN | PARITY_ERROR)))
	{
		rx1_buffer[rx1_wr_index] = UDR1;
		if(++rx1_wr_index == RX1_BUFFER_SIZE)
		rx1_wr_index = 0;

		rx1_counter++;
	}
}

void altUart0Write(unsigned char data)
{
	while( !(UCSR0A & (1 << UDRE0)));

	UDR0 = data;
}

void altUart1Write(unsigned char data)
{
	while ( !( UCSR1A & (1 << UDRE1)) );
	UDR1 = data;
}
/****************************************************************************************************************************************
Slanje podataka USART1 modulom
**
**************************************************************************************************************************************/

void UART1_Write(unsigned char data)
{
	if( (UCSR1B >> TXCIE1) & 0x01)
	{
		while(tx1_counter == TX1_BUFFER_SIZE);

		cli();

		if(tx1_counter || (UCSR1A & 0x20) == 0)
		{
			tx1_buffer[tx1_wr_index] = data;

			if(++tx1_wr_index == TX1_BUFFER_SIZE)
				tx1_wr_index = 0;

			tx1_counter++;
		}
		else
			UDR1 = data;

		sei();
	}
	else
		altUart1Write(data);
}

unsigned char UART1_Read(void)
{
	unsigned char data;

	if((UCSR1B >> RXCIE1) & 0x01)
	{
		while(rx1_counter == 0);

		data = rx1_buffer[rx1_rd_index];

		if(++rx1_rd_index == RX1_BUFFER_SIZE)
			rx1_rd_index = 0;

		rx1_counter--;
	}
	else
	{
		while( !((UCSR1A >> RXC1) & 0x01));
		data = UDR1;
	}

	return data;
}

