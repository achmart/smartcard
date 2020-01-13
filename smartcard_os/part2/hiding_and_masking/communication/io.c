/*
 * io.c
 *
 *  Created on: May 24, 2018
 *      Author: ga76jid
 */

#include <avr/io.h>
#include <util/delay.h>
#include <util/parity.h>
#include "interrupt.h"
#include "timer.h"
#include "io.h"
#include "stdlib.h"
#include "uart_debug.h"

#define N_DATA_BITS 8
#define N_STOP_BITS 2
#define SIZE_BYTE   8

volatile uint8_t tx_bit = 1;
volatile uint8_t tx_request = 0;

volatile uint8_t sample_counter = 0;
volatile uint8_t rx_active = 0;
volatile uint8_t rx_done = 0;
volatile uint8_t samples[3] = {0};
volatile uint8_t bits_count = 0;
volatile uint8_t error_detect;

void inline setup_trigger(void){
	DDRB |= (1 << PINB4);
	reset_trigger();
}

void inline set_trigger(void){
	PORTB |= (1 << PINB4);
}

void inline reset_trigger(void){
	PORTB &= ~(1 << PINB4);
}

void inline set_TX(void){
	// set PB6 as output
	DDRB |= (1 << PINB6);
	// set PB6 high
	PORTB |= (1 << PINB6);
}

void inline set_RX(void){
	DDRB &= ~(1 << PINB6);
}

void send_Bit(uint8_t bit){
	tx_request = 1;
	tx_bit = bit;
	// wait until ISR has changed port
	while(tx_request);
}

void send_Byte(uint8_t byte){


	disable_rx_int();
	enable_Timer1();
	set_TX();
	_delay_us(20);

	reset_Timer1();

	// Start bit
	send_Bit(0);
	// data Bits
	for(uint8_t i = 0; i < N_DATA_BITS; i++)
		send_Bit((byte >> i) & 0x01);
	// Parity Bit
	send_Bit(parity_even_bit(byte));
	// Stop Bit(s)
	for(uint8_t i = 0; i < N_STOP_BITS; i++)
		send_Bit(1);


	set_RX();
	_delay_us(20);
	enable_rx_int();
	disable_Timer1();

}


void send_Bytes(const uint8_t * bytes, uint8_t n){
	for(uint8_t i = 0; i < n; i++){
		Resend :
		send_Byte(bytes[i]);
		//wait for half an etu
		_delay_us(56);
		if (((PINB >> PINB6)&1) != 1){
				//wait for an etu
				_delay_us(113);
				//Resend the same byte if an error is detected
				goto Resend;
		}
	}
}

uint8_t receive_Byte(void){
	uint8_t byte= 0;
	uint8_t bit= 0;

	ReceiveAgain:
	byte= 0;
	bit= 0;
	error_detect=0;
	while(1){
		while(rx_active){
			if(rx_done){
				rx_done = 0;
				if(sample_counter == 3){
					bit = ((samples[0]+samples[1]+samples[2]) > 1);

					if((bits_count == SIZE_BYTE) & (parity_even_bit(byte)!= bit)){
						error_detect=1;
						break;

					}

					if(bits_count < SIZE_BYTE)
						byte |= (bit << bits_count);
						bits_count++;
						sample_counter = 0;

					if(bits_count == SIZE_BYTE+N_STOP_BITS){
						disable_Timer2();
						rx_active = 0;
						enable_rx_int();
						break;
					}
				}
				if(PINB & (1 << PINB6))
					samples[sample_counter] = 1;
				else
					samples[sample_counter] = 0;
				sample_counter++;
			}
		}
		if(bits_count == SIZE_BYTE+N_STOP_BITS){
			bits_count = 0;
			break;
		}
		else if (error_detect==1){
			bits_count = 0;
			break;
		}
	}

	//in case of an error detected
	if (error_detect==1){

		//set the line as output
		DDRB |= (1 << PINB6);
		//send 1
		PORTB |= (1 << PINB6);
		//wait for half an etu
		_delay_us(56);
		//send0
		PORTB &= ~(1 << PINB6);
		//wait for an etu
		_delay_us(113);
		//send1
		PORTB |= (1 << PINB6);
		//wait for half an etu
		_delay_us(56);
		goto ReceiveAgain;

	}
		return byte;

}

void receive_Bytes(uint8_t * const bytes, uint8_t n){
	for(uint8_t i = 0; i < n; i++){
		bytes[i] = receive_Byte();
	}
}
