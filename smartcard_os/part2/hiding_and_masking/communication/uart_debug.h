/*
 * uart_debug.h
 *
 *  Created on: May 13, 2018
 *      Author: martin
 */

#ifndef UART_DEBUG_H_
#define UART_DEBUG_H_

void USART_init();
void USART_puts (char *s, int n);
void USART_transmit(unsigned char data);

#endif /* UART_DEBUG_H_ */
