/**
	@file interrupt.c
	@brief This file contains all interrupts.
*/


/*
 * interrupt.c
 *
 *  Created on: May 24, 2018
 *      Author: ga76jid
 */

#include <util/delay.h>
#include "interrupt.h"
#include "io.h"
#include "timer.h"


/**
	This function activates interrupts on the provided pin.
*/

void config_rx_int(void){
	PCMSK1 |= (1 << PCINT14);
}

/**
	This function deactivates interrupts on the provided pin.
*/
void disable_rx_int(void){
	PCICR &= ~(1 << PCIE1);
}

/**
	This ISR sets the transmission pin according to the value of the tx_bit.

*/

ISR (TIMER1_COMPA_vect){
	if(tx_request){
		if(tx_bit)
			PORTB |= (1 << PINB6);
		else
			PORTB &= ~(1 << PINB6);
	}
	tx_request = 0;
}

void enable_rx_int(void){
	PCICR |= (1 << PCIE1);
}

/**
	This ISR indicates indicates the receive method if a new bit can be read.
*/

ISR (TIMER2_COMPA_vect){
	if(rx_active)
		rx_done = 1;
}

/**
	This ISR recognizes the start bit of the UART.
*/
ISR (PCINT1_vect){
	if(!(PINB & (1 << PINB6))){
		_delay_us(56);
		enable_Timer2();
		reset_Timer2();
		rx_active = 1;
		disable_rx_int();
	}
}
