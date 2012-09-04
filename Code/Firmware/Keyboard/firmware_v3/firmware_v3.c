#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "firmware_v2.h"	

#define BUFFER_SIZE	16

#define	PORT_ROW PORTD
#define	PIN_ROW0	2			//Row 0 pin is PD2
#define	PIN_ROW1	3			//Row 1 pin is PD3
#define	PIN_ROW2	4			//Row 2 pin is PD4
#define	PIN_ROW3	5			//Row 3 pin is PD5

#define	PIN_COL0	6			//Col 0 pin is PD6
#define	PIN_COL1	7			//Col 1 pin is PD7
#define	PIN_COL2	0			//Col 2 pin is PB0
#define	PIN_COL3	1			//Col 3 pin is PB1
#define	PIN_COL4	2			//Col 4 pin is PB2
#define	PIN_COL5	6			//Col 5 pin is PB6
#define	PIN_COL6	7			//Col 6 pin is PB7
#define	PIN_COL7	0			//Col 7 pin is PC0
#define	PIN_COL8	1			//Col 8 pin is PC1
#define	PIN_COL9	2			//Col 9 pin is PC2
#define	PIN_COL10	3			//Col 10 pin is PC3
#define	PIN_COL11	4			//Col 11 pin is PC4
#define	PIN_COL12	5			//Col 12 pin is PC5

//unsigned char keys[52];
unsigned int rows[4];
unsigned char state[7];

char key_buff[BUFFER_SIZE];
int write_ptr;
int read_ptr;

int main(void)
{
	init_UART(51);
	
	//port direction initialization
	DDRD = (1<<PIN_ROW0)|(1<<PIN_ROW1)|(1<<PIN_ROW2)|(1<<PIN_ROW3);t

	//port state initialization
	PORT_ROW = (1<<PIN_ROW0)|(1<<PIN_ROW1)|(1<<PIN_ROW2)|(1<<PIN_ROW3);
	
	for(;;)
	{
		if(keypressed())
		{
			read_keyboard();
			
			
			
			pack();
			write_key_buff(get_code(get_key()));
			
			/*
			for(int r=0; r<7; r++)
			{	
				for(unsigned char bit=0; bit<8; bit++)
				{
					if((state[r]&(1<<bit))>0)
					{
						transmit_byte('1');
					} else {
						transmit_byte('0');
					}
				}
				transmit_byte('|');
			}
			transmit_byte(13);
			*/
		}
	}

	return(0);
}

char read_key_buff(void)
{
	if(write_ptr==read_ptr) { return -1; }
	
	char data = key_buff[read_ptr];
	
	read_ptr++;
	if(read_ptr>=BUFFER_SIZE)
	{
		read_ptr = 0;
	}
	
	return data;
}

void write_key_buff(char data)
{
	key_buff[write_ptr] = data;
	
	write_ptr++;
	if(write_ptr>=BUFFER_SIZE)
	{
		write_ptr = 0;
	}
	
	if(write_ptr==read_ptr)
	{
		read_ptr++;
		if(read_ptr>=BUFFER_SIZE)
		{
			read_ptr = 0;
		}
	}
}

char get_code(int index)
{
	return pgm_read_byte(&keymap[index]);
}

int get_key(void)
{
	for(int byte=0; byte<7; byte++)
	{
		for(unsigned char bit=0; bit<8; bit++)
		{
			if((state[byte]&(1<<bit))==0)
			{
				return byte*8+bit;
			}
		}
	}
	
	return -1;
}

void send_key(void)
{
	char key;
	if((key=read_key_buff())!=-1)
	{
		transmit_byte(key);
	} else {
		transmit_byte(0);
	}
}

void pack(void)
{
	state[0] = rows[0]&0x00FF;
	state[1] = ((rows[0]&0x1F00)>>8)|((rows[1]&0x0007)<<5);
	state[2] = (rows[1]&0x07FF)>>3;
	state[3] = ((rows[1]&0x1800)>>11)|((rows[2]&0x003F)<<2);
	state[4] = ((rows[2]&0x1FC0)>>6)|((rows[3]&0x0001)<<7);
	state[5] = (rows[3]&0x01FE)>>1;
	state[6] = (rows[3]&0x1E00)>>9;
}

void send_bin_state(void)
{
	for(int i=0; i<7; i++)
	{
		transmit_byte(state[i]);
	}
}

void send_ascii_state(void)
{
	for(int r=0; r<4; r++)
	{	
		for(unsigned char bit=0; bit<13; bit++)
		{
			if((rows[r]&(1<<bit))>0)
			{
				transmit_byte('0');
			} else {
				transmit_byte('1');
			}
		}
		transmit_byte('|');
	}
	transmit_byte(13);
}

unsigned char keypressed(void)
{
	//row 0
	PORT_ROW = (0<<PIN_ROW0)|(1<<PIN_ROW1)|(1<<PIN_ROW2)|(1<<PIN_ROW3);
	
	if(bit_is_clear(PIND, PIN_COL0)){ return 0xFF; }	//column 0
	if(bit_is_clear(PIND, PIN_COL1)){ return 0xFF; }	//column 1
	if(bit_is_clear(PINB, PIN_COL2)){ return 0xFF; }	//column 2
	if(bit_is_clear(PINB, PIN_COL3)){ return 0xFF; }	//column 3
	if(bit_is_clear(PINB, PIN_COL4)){ return 0xFF; }	//column 4
	if(bit_is_clear(PINB, PIN_COL5)){ return 0xFF; }	//column 5
	if(bit_is_clear(PINB, PIN_COL6)){ return 0xFF; }	//column 6
	if(bit_is_clear(PINC, PIN_COL7)){ return 0xFF; }	//column 7
	if(bit_is_clear(PINC, PIN_COL8)){ return 0xFF; }	//column 8
	if(bit_is_clear(PINC, PIN_COL9)){ return 0xFF; }	//column 9
	if(bit_is_clear(PINC, PIN_COL10)){ return 0xFF; }	//column 10
	if(bit_is_clear(PINC, PIN_COL11)){ return 0xFF; }	//column 11
	if(bit_is_clear(PINC, PIN_COL12)){ return 0xFF; }	//column 12
	
	//row 1
	PORT_ROW = (1<<PIN_ROW0)|(0<<PIN_ROW1)|(1<<PIN_ROW2)|(1<<PIN_ROW3);
	
	if(bit_is_clear(PIND, PIN_COL0)){ return 0xFF; }	//column 0
	if(bit_is_clear(PIND, PIN_COL1)){ return 0xFF; }	//column 1
	if(bit_is_clear(PINB, PIN_COL2)){ return 0xFF; }	//column 2
	if(bit_is_clear(PINB, PIN_COL3)){ return 0xFF; }	//column 3
	if(bit_is_clear(PINB, PIN_COL4)){ return 0xFF; }	//column 4
	if(bit_is_clear(PINB, PIN_COL5)){ return 0xFF; }	//column 5
	if(bit_is_clear(PINB, PIN_COL6)){ return 0xFF; }	//column 6
	if(bit_is_clear(PINC, PIN_COL7)){ return 0xFF; }	//column 7
	if(bit_is_clear(PINC, PIN_COL8)){ return 0xFF; }	//column 8
	if(bit_is_clear(PINC, PIN_COL9)){ return 0xFF; }	//column 9
	if(bit_is_clear(PINC, PIN_COL10)){ return 0xFF; }	//column 10
	if(bit_is_clear(PINC, PIN_COL11)){ return 0xFF; }	//column 11
	if(bit_is_clear(PINC, PIN_COL12)){ return 0xFF; }	//column 12
	
	//row 2
	PORT_ROW = (1<<PIN_ROW0)|(1<<PIN_ROW1)|(0<<PIN_ROW2)|(1<<PIN_ROW3);
	
	if(bit_is_clear(PIND, PIN_COL0)){ return 0xFF; }	//column 0
	if(bit_is_clear(PIND, PIN_COL1)){ return 0xFF; }	//column 1
	if(bit_is_clear(PINB, PIN_COL2)){ return 0xFF; }	//column 2
	if(bit_is_clear(PINB, PIN_COL3)){ return 0xFF; }	//column 3
	if(bit_is_clear(PINB, PIN_COL4)){ return 0xFF; }	//column 4
	if(bit_is_clear(PINB, PIN_COL5)){ return 0xFF; }	//column 5
	if(bit_is_clear(PINB, PIN_COL6)){ return 0xFF; }	//column 6
	if(bit_is_clear(PINC, PIN_COL7)){ return 0xFF; }	//column 7
	if(bit_is_clear(PINC, PIN_COL8)){ return 0xFF; }	//column 8
	if(bit_is_clear(PINC, PIN_COL9)){ return 0xFF; }	//column 9
	if(bit_is_clear(PINC, PIN_COL10)){ return 0xFF; }	//column 10
	if(bit_is_clear(PINC, PIN_COL11)){ return 0xFF; }	//column 11
	if(bit_is_clear(PINC, PIN_COL12)){ return 0xFF; }	//column 12
	
	//row 3
	PORT_ROW = (1<<PIN_ROW0)|(1<<PIN_ROW1)|(1<<PIN_ROW2)|(0<<PIN_ROW3);
	
	if(bit_is_clear(PIND, PIN_COL0)){ return 0xFF; }	//column 0
	if(bit_is_clear(PIND, PIN_COL1)){ return 0xFF; }	//column 1
	if(bit_is_clear(PINB, PIN_COL2)){ return 0xFF; }	//column 2
	if(bit_is_clear(PINB, PIN_COL3)){ return 0xFF; }	//column 3
	if(bit_is_clear(PINB, PIN_COL4)){ return 0xFF; }	//column 4
	if(bit_is_clear(PINB, PIN_COL5)){ return 0xFF; }	//column 5
	if(bit_is_clear(PINB, PIN_COL6)){ return 0xFF; }	//column 6
	if(bit_is_clear(PINC, PIN_COL7)){ return 0xFF; }	//column 7
	if(bit_is_clear(PINC, PIN_COL8)){ return 0xFF; }	//column 8
	if(bit_is_clear(PINC, PIN_COL9)){ return 0xFF; }	//column 9
	if(bit_is_clear(PINC, PIN_COL10)){ return 0xFF; }	//column 10
	if(bit_is_clear(PINC, PIN_COL11)){ return 0xFF; }	//column 11
	if(bit_is_clear(PINC, PIN_COL12)){ return 0xFF; }	//column 12
	
	return 0;
}

unsigned char pin_on(unsigned int port, unsigned char pin)
{
	if(bit_is_clear(port, pin))
	{
		return 0;
	} else {
		return 1;
	}
}

void read_keyboard(void)
{
	PORT_ROW = 0xFF;
	
	//row 0
	PORT_ROW &= ~(1<<PIN_ROW0);
	
	_delay_us(1);
	rows[0] =	(pin_on(PIND, PIN_COL0)|(pin_on(PIND, PIN_COL1)<<1)
				|(pin_on(PINB, PIN_COL2)<<2)|(pin_on(PINB, PIN_COL3)<<3)
				|(pin_on(PINB, PIN_COL4)<<4)|(pin_on(PINB, PIN_COL5)<<5)
				|(pin_on(PINB, PIN_COL6)<<6)|(pin_on(PINC, PIN_COL7)<<7)
				|(pin_on(PINC, PIN_COL8)<<8)|(pin_on(PINC, PIN_COL9)<<9)
				|(pin_on(PINC, PIN_COL10)<<10)|(pin_on(PINC, PIN_COL11)<<11)
				|(pin_on(PINC, PIN_COL12)<<12));
	
	PORT_ROW |= 1<<PIN_ROW0;
	
	//row 1
	PORT_ROW &= ~(1<<PIN_ROW1);
	
	_delay_us(1);
	rows[1] =	((PIND&(1<<PIN_COL0))>>PIN_COL0)|(((PIND&(1<<PIN_COL1))>>PIN_COL1)<<1)
				|(((PINB&(1<<PIN_COL2))>>PIN_COL2)<<2)|(((PINB&(1<<PIN_COL3))>>PIN_COL3)<<3)
				|(((PINB&(1<<PIN_COL4))>>PIN_COL4)<<4)|(((PINB&(1<<PIN_COL5))>>PIN_COL5)<<5)
				|(((PINB&(1<<PIN_COL6))>>PIN_COL6)<<6)|(((PINC&(1<<PIN_COL7))>>PIN_COL7)<<7)
				|(((PINC&(1<<PIN_COL8))>>PIN_COL8)<<8)|(((PINC&(1<<PIN_COL9))>>PIN_COL9)<<9)
				|(((PINC&(1<<PIN_COL10))>>PIN_COL10)<<10)|(((PINC&(1<<PIN_COL11))>>PIN_COL11)<<11)
				|(((PINC&(1<<PIN_COL12))>>PIN_COL12)<<12);
	
	PORT_ROW |= 1<<PIN_ROW1;
	
	//row 2
	PORT_ROW &= ~(1<<PIN_ROW2);
	
	_delay_us(1);
	rows[2] =	((PIND&(1<<PIN_COL0))>>PIN_COL0)|(((PIND&(1<<PIN_COL1))>>PIN_COL1)<<1)
				|(((PINB&(1<<PIN_COL2))>>PIN_COL2)<<2)|(((PINB&(1<<PIN_COL3))>>PIN_COL3)<<3)
				|(((PINB&(1<<PIN_COL4))>>PIN_COL4)<<4)|(((PINB&(1<<PIN_COL5))>>PIN_COL5)<<5)
				|(((PINB&(1<<PIN_COL6))>>PIN_COL6)<<6)|(((PINC&(1<<PIN_COL7))>>PIN_COL7)<<7)
				|(((PINC&(1<<PIN_COL8))>>PIN_COL8)<<8)|(((PINC&(1<<PIN_COL9))>>PIN_COL9)<<9)
				|(((PINC&(1<<PIN_COL10))>>PIN_COL10)<<10)|(((PINC&(1<<PIN_COL11))>>PIN_COL11)<<11)
				|(((PINC&(1<<PIN_COL12))>>PIN_COL12)<<12);
	
	PORT_ROW |= 1<<PIN_ROW2;;
	
	//row 3
	PORT_ROW &= ~(1<<PIN_ROW3);
	
	_delay_us(1);
	rows[3] =	((PIND&(1<<PIN_COL0))>>PIN_COL0)|(((PIND&(1<<PIN_COL1))>>PIN_COL1)<<1)
				|(((PINB&(1<<PIN_COL2))>>PIN_COL2)<<2)|(((PINB&(1<<PIN_COL3))>>PIN_COL3)<<3)
				|(((PINB&(1<<PIN_COL4))>>PIN_COL4)<<4)|(((PINB&(1<<PIN_COL5))>>PIN_COL5)<<5)
				|(((PINB&(1<<PIN_COL6))>>PIN_COL6)<<6)|(((PINC&(1<<PIN_COL7))>>PIN_COL7)<<7)
				|(((PINC&(1<<PIN_COL8))>>PIN_COL8)<<8)|(((PINC&(1<<PIN_COL9))>>PIN_COL9)<<9)
				|(((PINC&(1<<PIN_COL10))>>PIN_COL10)<<10)|(((PINC&(1<<PIN_COL11))>>PIN_COL11)<<11)
				|(((PINC&(1<<PIN_COL12))>>PIN_COL12)<<12);
				
	PORT_ROW |= 1<<PIN_ROW3;
}

void init_UART(unsigned char baudrate)
{
	cli();
	
	UBRR0L = baudrate;							//set baud rate
	UBRR0H = 0;
	
	UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);	//enable rx and tx
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);			//8 data bits, 2 stop bits
	
	sei();
}

ISR(USART_RX_vect)
{
	cli();
	
	unsigned char command = UDR0;
	
	switch(command)
	{	
		case 'A':
			read_keyboard();
			break;
		case 'B':
			pack();
			break;
		case 'C':
			send_key();
			break;
		case 'D':
			send_bin_state();
			break;
		case 'E':
			send_ascii_state();
			break;
		case 'F':
			read_keyboard();
			send_bin_state();
			break;
		case 'G':
			read_keyboard();
			send_ascii_state();
			break;
	}
	
	sei();
}

void transmit_byte(unsigned char data)
{
	while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}