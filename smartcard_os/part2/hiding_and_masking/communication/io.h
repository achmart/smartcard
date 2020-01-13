/*
 * io.h
 *
 *  Created on: May 24, 2018
 *      Author: ga76jid
 */

#ifndef IO_H_
#define IO_H_

extern volatile uint8_t tx_bit;
extern volatile uint8_t tx_request;

extern volatile uint8_t sample_counter;
extern volatile uint8_t rx_active;
extern volatile uint8_t rx_done;
extern volatile uint8_t samples[3];
extern volatile uint8_t bits_count;

void setup_trigger(void);
void set_trigger(void);
void reset_trigger(void);
void set_TX(void);
void set_RX(void);
void send_Bit(uint8_t bit);
void send_Byte(uint8_t byte);
void send_Bytes(const uint8_t * bytes, uint8_t n);

uint8_t receive_Byte(void);
void receive_Bytes(uint8_t * const bytes, uint8_t n);

#endif /* IO_H_ */
