/**
	@file adc_trng.c
	@brief This file contains the initialization of the ADC.
*/


/*
 * adc_trng.c
 *
 *  Created on: Jun 15, 2018
 *      Author: ga94kep
 */

#include <stdint.h>
#include <avr/io.h>
#include "adc_trng.h"


/**
	This function initializes the ADC.
*/

void ADC_Init(void)
{
	ADMUX = 0b01001111;
	// define the division frequency factor between the XTAL frequency and the input clock to the ADC
	ADCSRA = (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);                 // activate ADC by setting ADEN ( enable to 1)

	ADCSRA |= (1 << ADSC);                  // one ADC conversion
	while (ADCSRA & (1 << ADSC));         // wait for result
	(void) ADCW;
}


/**
	This function return an ADC-conversion result.
*/
uint16_t ADC_Read(void)
{
	ADCSRA |= (1 << ADSC);            // single conversion
	while (ADCSRA & (1 << ADSC));   // wait for result
	return ADCW ;
}

