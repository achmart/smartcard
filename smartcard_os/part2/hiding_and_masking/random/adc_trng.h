/*
 * adc_trng.h
 *
 *  Created on: Jun 15, 2018
 *      Author: ga94kep
 */

#ifndef ADC_TRNG_H_
#define ADC_TRNG_H_

#include <stdint.h>

#define set_lsb(a,k)     ( a |= (1 << (k&31)) )   // function that sets the lsb of a to 1 at certain position k
#define clear_lsb(a,k)   ( a &= ~(1 << (k&31)) ) // function that clears the lsb of a to 0 at a certain position k
#define get_lsb(a) ((a)&1) //function that gets the lsb of a

void ADC_Init(void);      // function that initializes ADC
uint16_t ADC_Read(void); // function that reads ADC

#endif /* ADC_TRNG_H_ */
