/*
 * aes_functions.h
 *
 *  Created on: May 24, 2018
 *      Author: ga76jid
 */

#ifndef AES_FUNCTIONS_H_
#define AES_FUNCTIONS_H_

#include <inttypes.h>

void ark(uint8_t *state, const uint8_t *key);
void expand_key(uint8_t * key_buffer, uint8_t round_constant);
void *key_init(void *key);
void mask_ciphertext(uint8_t *cipher);
void mask_init(void);
void mask_key(uint8_t *buffer);
void masked_subbytes(uint8_t * buffer);
void mcol(uint8_t *int_buffer);
void remask(uint8_t *buffer);
void srows(uint8_t *int_buffer);
void subbytes(uint8_t * buffer);
void unmask_plaintext(uint8_t * buffer);
uint8_t xtime(uint8_t s);

#endif /* AES_FUNCTIONS_H_ */
