#include <avr/io.h>
#include <inttypes.h>

#include "aes/aes.h"
#include "uart_debug.h"
#include "timer.h"
#include "io.h"
#include "interrupt.h"

#define N_PARITY_BITS 1
#define N_KEY_BYTES 16
#define N_CMD_BYTES 5
#define N_ATR_BYTES 4

const uint8_t atr[] = {0x3B, 0x90, 0x11, 0x00};

void t0_protocol(uint8_t *buffer, uint8_t *secret_key){
	uint8_t command_bytes[N_CMD_BYTES] = {0};
	uint8_t *param = aes128_init(secret_key);
	send_Bytes(atr, N_ATR_BYTES);
	while(1){
		// Header
		receive_Bytes(command_bytes, N_CMD_BYTES);

		// Response
		//uart_puts((char *) &command_byte, 1);
		send_Byte(command_bytes[1]);


		// Data Section
		receive_Bytes(buffer, N_KEY_BYTES);
		set_trigger();
		aes128_decrypt(buffer, param);
		reset_trigger();
		//Response
		send_Byte(0x61);
		send_Byte(0x10);

		// Get response
		receive_Bytes(command_bytes, N_CMD_BYTES);

		send_Byte(command_bytes[1]);
		send_Bytes(buffer, N_KEY_BYTES);
		send_Byte(0x90);
		send_Byte(0x00);
	}
}

int main(void){

	USART_Init();

	uint8_t buffer[N_KEY_BYTES] = {0};

	uint8_t secret_key[N_KEY_BYTES] =
	{227, 99, 151, 143, 69, 245, 44, 14,
	 164, 89, 107, 242, 206, 220, 222, 114};

	setup_Timer1();
	setup_Timer2();
	setup_trigger();
	config_rx_int();
	sei();

	disable_Timer2();
	disable_Timer1();
	disable_rx_int();
	t0_protocol(buffer, secret_key);

	return 0;
}
