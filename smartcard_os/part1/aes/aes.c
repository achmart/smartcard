// OPTIONS: -O3 -std=gnu99
#include "aes.h"
#include "aes_functions.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define AES128_COMPLETE_ROUNDS 9


// Key initialisation
void *aes128_init(void *key) {
	// 12*16 byte for keys
	uint8_t * int_key = (uint8_t *)key;
	uint8_t *key_copy = calloc(176, 1);
	uint8_t r = 0x01;
	memcpy(key_copy, key, 16);
	for(int i = 1; i < 11; i++ )
	{
		// Function G
		expand_key(int_key, r);
		r = xtime(r);

		memcpy(key_copy + 16*i, int_key, 16);
	}
	return key_copy;
}

void aes128_decrypt(void *buffer, void *param)
{
	// Cast Pointers for code readability
	uint8_t * int_buffer = (uint8_t *)buffer;
	uint8_t * int_param = (uint8_t *)param + 160;

	// Init Round
	ark(int_buffer, int_param);

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

		// ShiftRows
		srows(int_buffer);

		// SubBytes
		subbytes(int_buffer);
	}
	// Last round
	// AddRoundKey
	int_param = int_param - 16;
	ark(int_buffer, int_param);


}
