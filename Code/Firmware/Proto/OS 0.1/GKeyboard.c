#include <avr/io.h>
#include <util/delay.h>

#include "Keyboard.h"
#include "Serial.h"

char key_get_key(void)
{
	uart_tx('C');		//tell the keyboard to send the last key
	
	while(!uart_available()) {}
	
	return uart_read_buff();
}
/*
void key_store_state(void)
{
	//not implemented
}

char* key_peek_state(void)
{
	//not implemented
}

char* key_get_state(void)
{
	//not implemented
}
*/