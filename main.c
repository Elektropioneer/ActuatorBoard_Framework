
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "dynamixel.h"
#include "ax.h"
#include "usart.h"

int main() {

	dynamixel_init(DYNAMIXEL_BAUDRATE);				// init dynamixel with 250kbps
	UART1_Init(UART1_BAUD, UART_ISR_ON);			// init uart with 9600 baud






	return 0;
}

