/**
	@file aes.c
	@brief This file contains the AES-decryption algorithm and the key initialization.
*/

// OPTIONS: -O3 -std=gnu99
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"
#include "aes_functions.h"
#include "../compile_options.h"

#define AES128_COMPLETE_ROUNDS 9


#ifdef NOP
	#include "hiding.h"
#endif

/** 
 *   This function implementes the aes-decryption.\n
     The function starts with the initial round where the functions ark(),srows() and subbytes() are  	   performed executed.\n
     Followed by 9 rounds of ark(), mcol(), srows() and subbytes() the algorithm is completed.\n
     Transfer parameters: 
	- buffer: encrypted ciphertext
	- param: key, needed for decryption
 */
void aes128_decrypt(void *buffer, void *param)
{
	// Cast Pointers for code readability
	uint8_t * int_buffer = (uint8_t *)buffer;
	uint8_t * int_param = (uint8_t *)param + 160;

	#ifdef MASK
	mask_ciphertext(buffer);
	#endif

	// Init Round
	ark(int_buffer, int_param);
	//uart_puts(buffer, 16);

	// ShiftRows
	srows(int_buffer);

	// SubBytes
	subbytes(int_buffer);

	// 9 Rounds
	for (uint8_t i = 0; i < AES128_COMPLETE_ROUNDS; i++) {


		// AddRoundKey
		int_param = int_param - 16;
		ark(int_buffer, int_param);


		// MixColumns
		mcol(int_buffer);

		#ifdef MASK
		remask(int_buffer);
		#endif

		// ShiftRows
		srows(int_buffer);

		// SubBytes
		subbytes(buffer);
		

	}

#ifdef NOP
	no_operations();
#endif
	// Last round
	// AddRoundKey
	int_param = int_param - 16;
	ark(int_buffer, int_param);
	#ifdef MASK
	unmask_plaintext(buffer);
	#endif
}

/**
	This function initializes the keys needed for the aes-algorithm.\n
	Optional it returns the keys in a masked version.\n
	Transfer-parameters:
	- key: encryption key 
*/

// Key initialization
void *aes128_init(void *key) {
#ifdef MASK
	mask_init();
#endif
	uint8_t *key_copy = (uint8_t *)key_init(key);
	return (void *)key_copy;
}
