/*
 * aes.h
 *
 *  Created on: May 13, 2018
 *      Author: martin
 */

#ifndef __AES_H__
#define __AES_H__

void aes128_decrypt(void *buffer, void *param);
void *aes128_init(void *key);

#endif
