#include <avr/io.h>
#include <avr/interrupt.h>

#define BUFF_SIZE 32

unsigned char buffer[BUFF_SIZE];
int write_ptr;
int read_ptr;

void uart_write_buff(unsigned char data);

void uart_init(unsigned int baudrate)
{
	cli();
	
	UBRRH = (unsigned char)(baudrate>>8);
	UBRRL = (unsigned char)baudrate;
	
	UCSRB = (1<<RXEN)|(1<<RXCIE)|(1<<TXEN);		//enable rx and tx
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);	//8 data bits, 2 stop bits
	
	sei();
}

void uart_tx(unsigned char data)
{
	while ( !(UCSRA & (1<<UDRE)) );
	UDR = data;
}

void uart_tx_str(char* addr)
{
	char c;
	while((c=*addr))
	{
		uart_tx(c);
		addr++;
	}
}

void uart_flush(void)
{
	read_ptr = write_ptr;
}

char uart_available(void)
{
	return !(write_ptr==read_ptr);
}

ISR(USART_RXC_vect)
{
	cli();
	uart_write_buff(UDR);
	uart_tx('r');
	sei();
}

unsigned char uart_read_buff(void)
{
	if(write_ptr==read_ptr) { return 0; }
	
	char data = buffer[read_ptr];
	
	read_ptr++;
	if(read_ptr>=BUFF_SIZE)
	{
		read_ptr = 0;
	}
	
	return data;
}

void uart_write_buff(unsigned char data)
{
	buffer[write_ptr] = data;
	
	write_ptr++;
	if(write_ptr>=BUFF_SIZE)
	{
		write_ptr = 0;
	}
	
	if(write_ptr==read_ptr)
	{
		read_ptr++;
		if(read_ptr>=BUFF_SIZE)
		{
			read_ptr = 0;
		}
	}
}