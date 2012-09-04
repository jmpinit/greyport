#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "Screen.h"
#include "Keyboard.h"
#include "Hex_Edit.h"

#define TRUE 0xFF
#define FALSE 0x00

unsigned int position;
unsigned int render_pos;

unsigned char* mem;
unsigned int length;

void hex_edit_run(unsigned char* p, unsigned int l)
{
	mem = p;
	length = l;
	/*
	mem[0] = 
	mem[1] = 
	mem[2] = 
	mem[3] = 
	mem[4] = 
	mem[5] = 
	mem[6] = 
	mem[7] = 
	mem[8] = 
	mem[9] = 
	mem[10] = 
	mem[11] = 
	mem[12] = 
	mem[13] = 
	mem[14] = 
	mem[15] = 
	mem[16] = 
	mem[17] = 
	mem[18] = 
	*/
	position = 0;
	render_pos = 0;
	
	render();
	
	char key = 0;
	while(key!=KEY_CTRL)
	{
		key = key_get_key();
		
		if(key=='>'&&position<(length-1))
		{
			position++;
			
			if(position>render_pos+5)
			{
				render_pos++;
			}
			
			render();
		} else if(key=='<'&&position>0)
		{
			position--;
			
			if(position<render_pos)
			{
				render_pos--;
			}
			
			render();
		} else if(key==13)
		{
			mem[position] = input_hex();
			render();
		}
	}
}

void render(void)
{
	screen_clear(0x00);
	
	for(int row=0; row<6; row++)
	{
		screen_print_hex(render_pos+row);
		screen_print_string(" | ");
		
		if((render_pos+row)==position) { invert = TRUE; }
		screen_print_hex(mem[render_pos+row]);
		invert = FALSE;
		
		if(row!=5) { screen_newline(); }
	}
}

unsigned char input_hex()
{
	char key = 0;
	unsigned char index = 0;
	unsigned char input[2];
	input[0] = 0;
	input[1] = 0;
	
	screen_set_pos(5, position-render_pos);
	screen_print_hex((input[0]<<4)|input[1]);
	screen_print_char('<');
	
	while(key!=13)
	{
		key = key_get_key();
		
		if(key>=48&&key<=57)
		{
			if(index<2)
			{
				input[index] = key-48;
				
				index++;
				
				//screen_clear(0x00);
				screen_set_pos(5, position-render_pos);
				screen_print_hex((input[0]<<4)|input[1]);
				//screen_set_pos(index, 1);
				//screen_print_char('^');
			}
		} else if(key>=97&&key<=102)
		{
			if(index<2)
			{
				input[index] = key-97+10;
				
				index++;
				
				screen_set_pos(5, position-render_pos);
				screen_print_hex((input[0]<<4)|input[1]);
			}
		} else if(key==KEY_BACKSPACE)
		{
			if(index>0)
			{
				index--;
				input[index] = 0;
				
				screen_set_pos(5, position-render_pos);
				screen_print_hex((input[0]<<4)|input[1]);
			}
		}
	}
	
	return (input[0]<<4)|input[1];
}