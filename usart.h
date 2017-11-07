#ifndef _USART_H_INCLUDED
#define _USART_H_INCLUDED

#define UART_ISR_ON		1
#define UART_ISR_OFF	0


#define FRAME_ERROR	 (1<<4)
#define DATA_OVERRUN (1<<3)
#define PARITY_ERROR (1<<2)

#define UART1_BAUD 9600UL
#define UBRR1_VALUE (F_CPU/(16*UART1_BAUD) - 1)

void UART1_Init(unsigned long, char);
void UART1_Write(unsigned char);
unsigned char UART1_Read(void);
void altUart1Write(unsigned char);

#endif
