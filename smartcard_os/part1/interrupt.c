/*
 * interrupt.c
 *
 *  Created on: May 24, 2018
 *      Author: ga76jid
 */

#include "interrupt.h"

void config_rx_int(void){
	PCMSK1 |= (1 << PCINT14);
}

void enable_rx_int(void){
	PCICR |= (1 << PCIE1);
}

void disable_rx_int(void){
	PCICR &= ~(1 << PCIE1);
}

ISR (TIMER1_COMPA_vect){
	if(tx_request){
		if(tx_bit)
			PORTB |= (1 << PINB6);
		else
			PORTB &= ~(1 << PINB6);
	}
	tx_request = 0;
}

ISR (TIMER2_COMPA_vect){
	if(rx_active)
		rx_done = 1;
}

ISR (PCINT1_vect){
	if(!(PINB & (1 << PINB6))){
		_delay_us(56);
		enable_Timer2();
		reset_Timer2();
		rx_active = 1;
		disable_rx_int();
	}
}
