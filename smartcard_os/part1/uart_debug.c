/*
 * uart_debug.c
 *
 *  Created on: May 13, 2018
 *      Author: martin
 */


#include <avr/io.h>
#include "uart_debug.h"

//#define F_CPU 3276800UL
#define F_CPU 4800000UL

#define BAUD 9600UL

#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)


void USART_Init()
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(UBRR_VAL>>8);
	UBRR0L = (unsigned char)UBRR_VAL;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<TXEN0);
        UCSR0C = 3<<UCSZ00; //8N1
}

void USART_Transmit(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0))){}
	UDR0 = data;
}

void uart_puts (char *s, int n)
{
	uint8_t i = 0;
    while (i < n)
    {   
        USART_Transmit(*s);
        s++;
        i++;
    }
}
