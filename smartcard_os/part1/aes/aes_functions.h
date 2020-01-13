/*
 * aes_functions.h
 *
 *  Created on: May 24, 2018
 *      Author: ga76jid
 */

#ifndef AES_FUNCTIONS_H_
#define AES_FUNCTIONS_H_

#include <inttypes.h>


uint8_t xtime(uint8_t s);
void srows(uint8_t *int_buffer);
void mcol(uint8_t *int_buffer);
void ark(uint8_t *int_buffer, uint8_t *int_param);
void subbytes(uint8_t * buffer);
void expand_key(uint8_t * buffer, uint8_t r);

#endif /* AES_FUNCTIONS_H_ */
