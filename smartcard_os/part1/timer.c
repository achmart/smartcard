/*
 * timer.c
 *
 *  Created on: May 24, 2018
 *      Author: ga76jid
 */

#include <avr/io.h>
#include "timer.h"

void setup_Timer1(void){
	// setup counter
	TCCR1B |= (1 << CS10); // no prescaler
	// 372 = 0x174
	OCR1AH = 0x01;
	OCR1AL = 0x74;

	// 186 = 372/2 = 0xBA
	//OCR1AH = 0x00;
	//OCR1AL = 0xBA;
	// Clear on Compare Match
	TCCR1B |= (1 << WGM12);
}

void inline enable_Timer1(void){
	TIMSK1 |= (1 << OCF1A);
}

void inline disable_Timer1(void){
	TIMSK1 &= ~(1 << OCF1A);
}

void reset_Timer1(void){
	TCNT1H = 0x00;
	TCNT1L = 0x00;
}

void setup_Timer2(void){
	TCCR2B |= (1 << CS20); // no prescale

	OCR2A = 124;

	TCCR2A |= (1 << WGM01);
}

void inline enable_Timer2(void){
	TIMSK2 |= (1 << OCIE2A);
}

void inline disable_Timer2(void){
	TIMSK2 &= ~(1 << OCIE2A);
}

void inline reset_Timer2(void){
	TCNT2 = 0x00;
}
