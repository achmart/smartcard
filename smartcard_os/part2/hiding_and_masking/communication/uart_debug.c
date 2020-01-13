/**
	@file uart_debug.c
	@brief This file contains the function for the debuggig via USART.
*/


/*
 * uart_debug.c
 *
 *  Created on: May 13, 2018
 *      Author: martin
 */

#include <avr/io.h>
#include "uart_debug.h"

#define F_CPU 4800000UL
#define BAUD 9600UL
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)


/**
	This function initializes the USART, setting the baud rate & enabling the receiver and transmitter.
*/
void USART_init(){
	/* Set baud rate */
	UBRR0H = (unsigned char)(UBRR_VAL>>8);
	UBRR0L = (unsigned char)UBRR_VAL;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<TXEN0);
        UCSR0C = 3<<UCSZ00; //8N1
}

/**
	This function is used for the outputs of the USART.\n
	Transfer-parameters:
	- string: string to output
	- n: number of characters 
*/
void USART_puts(char *string, int n){
    for( uint8_t i = 0; i < n; i++){
        USART_transmit(*string);
        string++;
    }
}

/**
	This function is used for the transmission of the USART.
	Transfer-parameters:
	- data: string to output
*/
void USART_transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0))){
		;
	}
	UDR0 = data;
}
