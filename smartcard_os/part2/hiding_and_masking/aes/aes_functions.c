/**
 *	@file aes_functions.c
 *	@brief This file includes all functions needed for the cryptographic part.
*/



#include "aes_functions.h"
#include "aes_tables.h"
#include "./../random/random.h"
#include <stdlib.h>
#include <string.h>

#define AES128_NUM_OF_BYTES 16

#ifdef NOP
#include "hiding.h"
#endif

#ifdef SHUFFLE
#include "hiding.h"
#endif

#ifdef MASK
// input masks

uint8_t /**input mask for 1. column*/ m1_i=0;
uint8_t	/**input mask for 2. column*/ m2_i=0; 
uint8_t	/**input mask for 3. column*/ m3_i=0; 
uint8_t	/**input mask for 4. column*/ m4_i=0;
uint8_t	/**input mask for S-Box*/ m_i=0;
uint8_t	/**output mask for S-Box*/ mp_i = 0;

// transformed masks
uint8_t /**transformed mask for 1. column*/ m1_t=0; 
uint8_t	/**transformed mask for 2. column*/ m2_t=0; 
uint8_t	/**transformed mask for 3. column*/ m3_t=0; 
uint8_t	/**transformed mask for 4. column*/ m4_t = 0;
#endif

/**
 *	This function implements the Add-Round-Key operation.\n 
 *	The current state is xored with a round-specific key.\n
 *	Transfer-parameters:
 *	- state: current state
 *	- key: specific round key
 */
void ark(uint8_t *state, const uint8_t *key) {
	for (uint8_t i = 0; i < 16; i++)
		state[i] ^= key[i];
}


/**
 *	This function implements the Key-Expansion.\n
 *	Using the key of the last round, a new round key is generated.\n
 *	Transfer-parameters: 
 *	- key_buffer: last round key
 *	- round_constant: specific round constant 
 */
void expand_key(uint8_t * key_buffer, uint8_t round_constant){
	// Rotate least word and apply S-Box
	uint8_t g[4] = {key_buffer[12], key_buffer[13],
			key_buffer[14], key_buffer[15]};
	uint8_t help = g[0];
	for (uint8_t i = 0; i < 3; i++)
		g[i] = sbox[g[i+1]];
	g[3] = sbox[help];

	// Add Round Coefficient
	g[0] ^= round_constant;

	// Cascaded XOR
	for (uint8_t i = 0; i < 4; i++) {
		key_buffer[i] ^= g[i];
		key_buffer[i+4] ^= key_buffer[i];
		key_buffer[i+8] ^= key_buffer[i+4];
		key_buffer[i+12] ^= key_buffer[i+8];
	}
}

/**
 *	This function calculates apriori all needed keys for the decryption using the expand_key() 
 * 	function.\n
 *	Transfer-parameters:
 *	- key: initial secret key  
 */
void *key_init(void *key){
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
#ifdef MASK
	for(int i = 0; i < 11; i++){
		mask_key(key_copy + 16*i);
	}
#endif
	return key_copy;
}

#ifdef MASK

/**
 *	This function implements the masking of the ciphertext. The ciphertext gets masked with mx_t, mp_i and 		m_i.\n
	Transfer-parameter:
	- cipher: ciphertext
*/
void mask_ciphertext(uint8_t *cipher){
	for(uint8_t i = 0; i < AES128_NUM_OF_BYTES; i++){
			if(i%4 == 0)
				cipher[i] ^= m1_t ^ mp_i ^ m_i;
			else if(i%4 == 1)
				cipher[i] ^= m2_t ^ mp_i ^ m_i;
			else if(i%4 == 2)
				cipher[i] ^= m3_t ^ mp_i ^ m_i;
			else
				cipher[i] ^= m4_t ^ mp_i ^ m_i;

	}
}


/**
 *	This function randomly generates the input and output masks needed for the Boolean masking.\n
	Moreover the masked S-Box is computed.
*/
void mask_init(void){
	// Generate input and output masks
	//init_rand();

	m_i  = get_rand();
	mp_i = get_rand();
	m1_t = get_rand();
	m2_t = get_rand();
	m3_t = get_rand();
	m4_t = get_rand();

	// MixColumn
	m1_i = mult_14[m1_t] ^ mult_11[m2_t] ^ mult_13[m3_t] ^ mult_9[m4_t];
	m2_i = mult_9[m1_t]  ^ mult_14[m2_t] ^ mult_11[m3_t] ^ mult_13[m4_t];
	m3_i = mult_13[m1_t] ^ mult_9[m2_t]  ^ mult_14[m3_t] ^ mult_11[m4_t];
	m4_i = mult_11[m1_t] ^ mult_13[m2_t] ^ mult_9[m3_t]  ^ mult_14[m4_t];

	// compute masked sbox Sica. 5b.page 12
	for(uint16_t i = 0; i < 256; i++){
		inv_sbox_masked[(i ^ mp_i)] = (inv_sbox[i] ^ m_i);
	}
}


/**
 *	This function masks the current key using the input masks and the transformed masks.\n
	Transfer-parameters:
	- key_buffer: current round key
*/
void mask_key(uint8_t *key_buffer){
	for(uint8_t i = 0; i < AES128_NUM_OF_BYTES; i++){
			if(i%4 == 0)
				key_buffer[i] ^= m1_t ^ m_i;
			else if(i%4 == 1)
				key_buffer[i] ^= m2_t ^ m_i;
			else if(i%4 == 2)
				key_buffer[i] ^= m3_t ^ m_i;
			else
				key_buffer[i] ^= m4_t ^ m_i;
		}
}

/*
void masked_subbytes(uint8_t * state){
	for(uint8_t j = 0; j < AES128_NUM_OF_BYTES; j++){
#ifdef NOP
		no_operations();
#endif

#ifdef SHUFFLE
		uint8_t rand_array[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
		shuff_operations(rand_array);
		state[rand_array[j]] = inv_sbox_masked[state[rand_array[j]]];
#else
		state[j] = inv_sbox_masked[state[j]];
#endif
	}


	#ifdef SHUFFLE
	uint8_t rand_array[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	shuff_operations(rand_array);
	for(uint8_t j = 0; j < AES128_NUM_OF_BYTES; j++) {
		state[rand_array[j]] = inv_sbox_masked[state[rand_array[j]]];

	}
    #else
	for(uint8_t j = 0; j < AES128_NUM_OF_BYTES; j++) {
		state[j] = inv_sbox_masked[state[j]];
	}
	#endif

}
*/

/**
 *	This function unmasks the plaintext.\n
	Transfer-parameter:
	- state: current state
*/
void unmask_plaintext(uint8_t * state){
	for(uint8_t i = 0; i < AES128_NUM_OF_BYTES; i++){
			if(i%4 == 0)
				state[i] ^= m1_t;
			else if(i%4 == 1)
				state[i] ^= m2_t;
			else if(i%4 == 2)
				state[i] ^= m3_t;
			else
				state[i] ^= m4_t;
		}
}

/**
 *	This function is used to remask the current state with m1_i and mp_i.\n
	Transfer-parameter:
	- state: current state
*/

void remask(uint8_t *state){
	for(uint8_t i = 0; i < AES128_NUM_OF_BYTES; i++){
			if(i%4 == 0)
				state[i] ^= m1_i ^ mp_i;
			else if(i%4 == 1)
				state[i] ^= m2_i ^ mp_i;
			else if(i%4 == 2)
				state[i] ^= m3_i ^ mp_i;
			else
				state[i] ^= m4_i ^ mp_i;
		}
}


#endif

/**
 *	This function implements the inverse MixColumns-operation for the AES-decryption algorithm.\n
	The MixColumns-operation is a GF(2⁸)-linear transformation mixing each column of the state.\n
	This function is implementet using different look-up-tables for the for multiplications in the Galois 		field.\n
	Transfer-parameter: 
	- state: current state
 */
void mcol(uint8_t *state) {
	for (uint8_t i = 0; i < 15; i+=4) {
		uint8_t b0_old = state[0+i];
		uint8_t b1_old = state[1+i];
		uint8_t b2_old = state[2+i];
		uint8_t b3_old = state[3+i];
		state[0 + i] = mult_14[b0_old] ^ mult_11[b1_old] ^ mult_13[b2_old] ^ mult_9[b3_old];
		state[1 + i] = mult_9[b0_old] ^ mult_14[b1_old] ^ mult_11[b2_old] ^ mult_13[b3_old];
		state[2 + i] = mult_13[b0_old] ^ mult_9[b1_old] ^ mult_14[b2_old] ^ mult_11[b3_old];
		state[3 + i] = mult_11[b0_old] ^ mult_13[b1_old] ^ mult_9[b2_old] ^ mult_14[b3_old];
	}
}

/**
 *	This function implements the inverse of the ShiftRows-operation of the AES-encryption algorithm.\n
	The rows of the state get rotated to the right with the offsets 0,1,2 and 3, respectively.\n
	Transfer-parameter: 
	- state: current state
 */
void srows(uint8_t *state) {
	uint8_t help;
	// 2nd row
	help = state[13];
	for(uint8_t i = 13; i > 3; i-=4) {
		state[i] = state[i-4];
	}
	state[1] = help;

	// 3rd row
	help = state[2];
	state[2] = state[10];
	state[10] = help;
	help = state[6];
	state[6] = state[14];
	state[14] = help;

	// 4th row
	help = state[3];
	for(uint8_t i = 3; i < 12; i+=4) {
		state[i] = state[i+4];
	}
	state[15] = help;
}
/**
 *	This function implements the inverted SubBytes-operation using the inverse SBOX.\n
	Each byte gets substituted by the appropriate SBOX-value.\n
	Transfer-parameter: 
	- state: current state
 */
void subbytes(uint8_t * state){
#ifdef SHUFFLE
	uint8_t rand_array[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	shuff_operations(rand_array);
#endif

	for(uint8_t j = 0; j < AES128_NUM_OF_BYTES; j++){
#ifdef NOP
		no_operations();
#endif

#ifdef SHUFFLE
	#ifdef MASK
		state[rand_array[j]] = inv_sbox_masked[state[rand_array[j]]];
	#else
		state[rand_array[j]] = inv_sbox[state[rand_array[j]]];
	#endif
#else
	#ifdef MASK
		state[j] = inv_sbox_masked[state[j]];
	#else
		state[j] = inv_sbox[state[j]];
	#endif
#endif
	}
	/*
#ifdef SHUFFLE
	uint8_t rand_array[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	shuff_operations(rand_array);
	for(uint8_t j = 0; j < AES128_NUM_OF_BYTES; j++) {
		state[rand_array[j]] = inv_sbox[state[rand_array[j]]];
	}
#else
	for(uint8_t j = 0; j < AES128_NUM_OF_BYTES; j++) {
		state[j] = inv_sbox[state[j]];
	}
#endif
*/
}

/**
 *	This function implements the xtime-function which is needed for the normal MixColumns-operation and for 	the Key Expansion.\n
	Transfer-parameter: 
	- s: variable to multiply by x
 */
uint8_t xtime(uint8_t s) {
	uint8_t x = s << 1;
	if (s & 0x80)
		x ^= 0x1b;
	return x;
}
