/**
	@file random.c
	@brief This file contains the Pseudo Random Number Generator.
*/


/*
 * random.c
 *
 *  Created on: Jun 7, 2018
 *      Author: ga76jid
 */
#include <inttypes.h>
#include "random.h"
#include "adc_trng.h"

uint8_t a,b,c,x = 0;


/**
	This function initializes the PRNG.
*/

void init_rand(void){
	//XOR new entropy into key state
	a = a^get_true_rand();
	b = b^get_true_rand();
	c = c^get_true_rand();

	get_rand();
}

/**
	This function returns a pseudorandom number.
*/

uint8_t get_rand(void){
	/*
	 * Example:
	 */
	x++;               //x is incremented every round and is not affected by any other variable
	a = (a^c^x);       //note the mix of addition and XOR
	b = (b+a);         //And the use of very few instructions
	c = (c+((b>>1)^a));  //the right shift is to ensure that high-order bits from b can affect
	return(c);          //low order bits of other variables
}

uint8_t get_true_rand(void){
	uint8_t i = 0;
	uint8_t rand1;
	uint8_t rand2;
	uint8_t random = 0;
	while (i < 8){
		// get two random bits
		 rand1 = ADC_Read() & 0x01;
		 rand2 = ADC_Read() & 0x01;
		if ((rand1 == 1) && (rand2 == 0)){
			random = (random << 1) + 1;
			i++;
		} else if((rand1) == 0 && (rand2 == 1)){
			random = (random << 1) + 0;
			i++;
		}
	}
	return random;
}
