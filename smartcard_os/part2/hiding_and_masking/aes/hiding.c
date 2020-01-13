/**
 *	@file hiding.c
 *	@brief This file conatains the hiding countermeasures (NOPs, Shuffling).
*/

/* hiding.c
 *
 *  Created on: Jun 15, 2018
 *      Author: ga94kep
 */

//#include "hiding.h"
//#include "adc_trng.h"
//#include "aes.h"
#include "./../random/random.h"
//#include "stdint.h"
//#include "stdlib.h"



//NO operations

/**
 *	This function is used to generate a random number of nops.
*/
void no_operations(void)
{
	int random_number;
    int i;
    random_number =get_rand() & 0x0f;

    for(i=0; i<random_number; i++)
    {
    	//Nop operation in c
    	//https://en.wikipedia.org/wiki/NOP
	//https://stackoverflow.com/questions/24359632/how-does-asmnop-works
    	 asm volatile ("nop");
    }
}

//Shuffling
// Fisher Yates Algorithm: https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm

/**
 *	This function implements the Fisher_Yates shuffle.\n
	Transfer-parameter:
	- shuff_tab: array to shuffle
*/
void shuff_operations(uint8_t *shuff_tab){

	int i;
	int intermediate;
	int rand_index;
	for (i = 16; i > 0; i--)
	{

		rand_index= (get_rand() %i);

		intermediate = shuff_tab[i-1];
		shuff_tab[i-1] = shuff_tab[rand_index];
		shuff_tab[rand_index] = intermediate;
	}
}
