
#include "aes_functions.h"
#include "aes_tables.h"


#define AES128_NUM_OF_BYTES 16

uint8_t xtime(uint8_t s) {
	uint8_t x = s << 1;
	if (s & 0x80)
		x ^= 0x1b;
	return x;
}

void srows(uint8_t *int_buffer) {
	uint8_t help;
	// 2nd row
	help = int_buffer[13];
	for(uint8_t i = 13; i > 3; i-=4) {
		int_buffer[i] = int_buffer[i-4];
	}
	//int_buffer[5] = int_buffer[1];
	//int_buffer[9] = int_buffer[5];
	//int_buffer[13] = int_buffer[9];
	int_buffer[1] = help;

	// 3rd row
	help = int_buffer[2];
	int_buffer[2] = int_buffer[10];
	int_buffer[10] = help;
	help = int_buffer[6];
	int_buffer[6] = int_buffer[14];
	int_buffer[14] = help;

	// 4th row
	help = int_buffer[3];
	for(uint8_t i = 3; i < 12; i+=4) {
		int_buffer[i] = int_buffer[i+4];
	}
	//int_buffer[11] = int_buffer[15];
	//int_buffer[7] = int_buffer[11];
	//int_buffer[3] = int_buffer[7];
	int_buffer[15] = help;

}

void mcol(uint8_t *int_buffer) {
	for (uint8_t i = 0; i < 15; i+=4) {
		uint8_t b0_old = int_buffer[0+i];
		uint8_t b1_old = int_buffer[1+i];
		uint8_t b2_old = int_buffer[2+i];
		uint8_t b3_old = int_buffer[3+i];
		int_buffer[0 + i] = mult_14[b0_old] ^ mult_11[b1_old] ^ mult_13[b2_old] ^ mult_9[b3_old];
		int_buffer[1 + i] = mult_9[b0_old] ^ mult_14[b1_old] ^ mult_11[b2_old] ^ mult_13[b3_old];
		int_buffer[2 + i] = mult_13[b0_old] ^ mult_9[b1_old] ^ mult_14[b2_old] ^ mult_11[b3_old];
		int_buffer[3 + i] = mult_11[b0_old] ^ mult_13[b1_old] ^ mult_9[b2_old] ^ mult_14[b3_old];
	}
}

void ark(uint8_t *int_buffer, uint8_t *int_param) {
	for (uint8_t i = 0; i < 16; i++)
		int_buffer[i] ^= int_param[i];
}

void subbytes(uint8_t * buffer){
	for(uint8_t j = 0; j < AES128_NUM_OF_BYTES; j++) {
		buffer[j] = inv_sbox[buffer[j]];
	}
}

void expand_key(uint8_t * buffer, uint8_t r){
	// Rotate least word and apply S-Box
	uint8_t g[4] = {buffer[12], buffer[13],
			buffer[14], buffer[15]};
	uint8_t help = g[0];
	for (uint8_t i = 0; i < 3; i++)
		g[i] = sbox[g[i+1]];

	g[3] = sbox[help];

	// Add Round Coefficient
	g[0] ^= r;


	// XOR
	for (uint8_t i = 0; i < 4; i++) {
		buffer[i] ^= g[i];
		buffer[i+4] ^= buffer[i];
		buffer[i+8] ^= buffer[i+4];
		buffer[i+12] ^= buffer[i+8];
	}

}
