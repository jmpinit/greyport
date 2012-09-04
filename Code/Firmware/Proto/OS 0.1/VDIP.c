#include <avr/io.h>
#include <util/delay.h>

#include "Disk.h"
#include "Serial.h"

#define NOP asm("nop")

/*
void disk_initialize()
{
	uart_init(51);	//9600 baud
	
	//VDIP ASCII # mode
	uart_tx_str("IPA");
	uart_tx(13);
	
	while(!uart_rx()==13){NOP;}
	
	//shortened commands
	uart_tx_str("SCS");
	uart_tx(13);
	
	while(!uart_rx()==13){NOP;}
}

unsigned char disk_ready()
{
	uart_tx(13);
	
	char ans = uart_rx();
	
	if(ans=='>')
	{
		return 0xFF;
	}
	
	return 0;
}

void disk_open_writer(char* file)
{
	uart_tx_str("OPW ");
	uart_tx_str(file);
	uart_tx(13);
	
	while(!uart_rx()==13){NOP;}
}

void disk_open_reader(char* file)
{
	uart_tx_str("OPR ");
	uart_tx_str(file);
	uart_tx(13);
	
	while(!uart_rx()==13){NOP;}
}

void disk_close(char* file)
{
	uart_tx_str("CLF ");
	uart_tx_str(file);
	uart_tx(13);
	
	while(!uart_rx()==13){NOP;}
}

char* disk_read_file(char* file, int length)
{

}

void disk_write_file(char* file, char* data)
{

}
*/