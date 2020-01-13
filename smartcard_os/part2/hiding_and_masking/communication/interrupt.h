/*
 * interrupt.h
 *
 *  Created on: May 24, 2018
 *      Author: ga76jid
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <avr/interrupt.h>

void config_rx_int(void);
void disable_rx_int(void);
void enable_rx_int(void);

ISR (TIMER1_COMPA_vect);
ISR (TIMER2_COMPA_vect);
ISR (PCINT1_vect);

#endif /* INTERRUPT_H_ */
