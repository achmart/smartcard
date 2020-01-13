/*
 * uart_debug.h
 *
 *  Created on: May 13, 2018
 *      Author: martin
 */

#ifndef UART_DEBUG_H_
#define UART_DEBUG_H_

void USART_Init();
void USART_Transmit(unsigned char data);
void uart_puts (char *s, int n);


#endif /* UART_DEBUG_H_ */
