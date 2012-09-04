#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "Screen.h"
#include "Keyboard.h"
#include "Serial.h"
#include "Hex_Edit.h"

//#include "logo.h"


void bas_run(unsigned char* program, unsigned int length);

typedef unsigned char boolean;
typedef unsigned char byte;

#define TRUE	0xFF
#define FALSE	0x00

#define CMD_SIZE 32

#define PROG_LENGTH 64
unsigned char program[PROG_LENGTH];
unsigned char test_prog[] = {
0x02, 0x00, 0xFF, 0xFF,
0x0C, 0x04, 0x00,
0x02, 0x01, 0x00, 0x01,
0x02, 0x02, 0x00, 0x02,
0x02, 0x03, 0x03, 0x20,
0x0C, 0x08, 0x01,
0x0C, 0x00, 0x03,
0x0C, 0x08, 0x02,
0x0C, 0x00, 0x03,
0x01, 0x00, 0x13 };

//commands
const char CMD_REBOOT[] PROGMEM = "reboot";
const char CMD_CLEAR[] PROGMEM = "clear";
const char CMD_RUN[] PROGMEM = "run";
const char CMD_EDITOR[] PROGMEM = "edit";

char command[CMD_SIZE];

void boot(void)
{
	//start the LCD
	screen_initialize();
    screen_clear(0x00);
	
	//start the UART hardware
	uart_init(51);
	
	//clear program RAM
	for(unsigned int i=0; i<34; i++)
	{
		program[i] = test_prog[i];
	}
}

void get_command()
{
	boolean shift = FALSE;
	
	byte index = 0;
	char key = 0;
	while(key==0||key!=13)
	{
		key = key_get_key();
		
		if(key!=0&&index<CMD_SIZE)
		{
			if(key<=122&&key>=97)
			{
				if(shift)
				{
					command[index] = key-32;
				} else {
					command[index] = key;
				}
				
				screen_print_char(command[index]);
				index++;
			} else if(key==KEY_BACKSPACE)
			{
				if(index>0)
				{
					index--;
					command[index] = 0;
					screen_backspace();
					screen_print_char(' ');
					screen_backspace();
				}
			} else if(key==KEY_SHIFT)
			{
				if(shift)
				{
					shift = FALSE;
				} else {
					shift = TRUE;
				}
			} else if(key==KEY_CTRL)
			{
				//do nothing for now
			} else if(key==KEY_ALT)
			{
				//do nothing for now
			} else if(key!=13) {
				command[index] = key;
				screen_print_char(command[index]);
				index++;
			}
		}
	}
	
	screen_newline();
}

boolean str_cmp(char* str1, const char* str2)
{
	for(;;)
	{
		if(pgm_read_byte(str2)==*str1)
		{
			if((pgm_read_byte(str2)==0)&&(*str1==0))
			{
				return TRUE;
			} else {
				str1++;
				str2++;
			}
		} else {
			return FALSE;
		}
	}
}

void parse_command()
{
	if(str_cmp(command, CMD_REBOOT))
	{
		boot();
	} else if(str_cmp(command, CMD_CLEAR))
	{
		screen_clear(0xFF);
	
		for(int i=0; i<PROG_LENGTH; i++)
		{
			program[i] = 255;
		}
	
		screen_clear(0x00);
	} else if(str_cmp(command, CMD_RUN))
	{
		bas_run(program, PROG_LENGTH);
		screen_newline();
	} else if(str_cmp(command, CMD_EDITOR))
	{
		//editor_run();
		hex_edit_run(program, PROG_LENGTH);
		screen_clear(0x00);
	} else {
		screen_print_string("bad command");
		screen_newline();
	}
	
	for(unsigned char i=0; i<CMD_SIZE; i++)
	{
		command[i] = 0;
	}
	
	screen_clear_shadow();
	
	screen_print_char('>');
}

int main(void)
{
	boot();
	
	screen_print_char('>');
	
    for(;;)
    {
		get_command();
		parse_command();
    }
    
    // Never reached.
    return(0);
}