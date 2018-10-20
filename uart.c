
 /* 
 /  UART FIFO implementation is from the FatFS AVR sample code
 /  Copyright (C) 2016, ChaN, all right reserved.
 /
 / * This software is a free software and there is NO WARRANTY.
 / * No restriction on use. You can use, modify and redistribute it for
 /   any purpose as you like UNDER YOUR RESPONSIBILITY.
 / * Redistributions of source code must retain the above copyright notice.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#define	UART_BUFF		128


typedef struct {
	uint16_t	wi, ri, ct;
	uint8_t buff[UART_BUFF];
} FIFO;
static
volatile FIFO TxFifo, RxFifo;



/* Initialize UART */

void uart_init (uint32_t bps)
{
	uint16_t n;


	UCSR0B = 0;

	RxFifo.ct = 0; RxFifo.ri = 0; RxFifo.wi = 0;
	TxFifo.ct = 0; TxFifo.ri = 0; TxFifo.wi = 0;

	n = F_CPU / bps / 8;
	UBRR0L = (n >> 1) + (n & 1) - 1;
	UCSR0B = _BV(RXEN0)|_BV(RXCIE0)|_BV(TXEN0);
}


/* Get a received character */

uint16_t uart_test (void)
{
	return RxFifo.ct;
}


uint8_t uart_getc (void)
{
	uint8_t d, i;


	while (RxFifo.ct == 0) ;

	i = RxFifo.ri;
	d = RxFifo.buff[i];
	cli();
	RxFifo.ct--;
	sei();
	RxFifo.ri = (i + 1) % sizeof RxFifo.buff;

	return d;
}


/* Put a character to transmit */

void uart_putc (uint8_t d)
{
	uint8_t i;


	while (TxFifo.ct >= sizeof TxFifo.buff) ;

	i = TxFifo.wi;
	TxFifo.buff[i] = d;
	cli();
	TxFifo.ct++;
	UCSR0B = _BV(RXEN0)|_BV(RXCIE0)|_BV(TXEN0)|_BV(UDRIE0);
	sei();
	TxFifo.wi = (i + 1) % sizeof TxFifo.buff;
}


/* UART RXC interrupt */

ISR(USART_RX_vect)
{
	uint8_t d, n, i;


	d = UDR0;
	n = RxFifo.ct;
	if (n < sizeof RxFifo.buff) {
		RxFifo.ct = ++n;
		i = RxFifo.wi;
		RxFifo.buff[i] = d;
		RxFifo.wi = (i + 1) % sizeof RxFifo.buff;
	}
}


/* UART UDRE interrupt */

ISR(USART_UDRE_vect)
{
	uint8_t n, i;


	n = TxFifo.ct;
	if (n) {
		TxFifo.ct = --n;
		i = TxFifo.ri;
		UDR0 = TxFifo.buff[i];
		TxFifo.ri = (i + 1) % sizeof TxFifo.buff;
	}
	if (n == 0) UCSR0B = _BV(RXEN0)|_BV(RXCIE0)|_BV(TXEN0);
}

