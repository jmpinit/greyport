/*#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "Editor.h"
#include "Screen.h"
#include "Keyboard.h"
#include "Serial.h"

#define TRUE 0xFF
#define FALSE 0x00

#define PROG_LENGTH 64

const char HEX_TABLE[] PROGMEM = "0123456789ABCDEF";

//parameter types
#define TYPE_CMD	0
#define TYPE_NONE	1
#define TYPE_LOW	2
#define TYPE_HIGH	3
#define TYPE_VAR	4
#define TYPE_FUNC	5

//instruction text
const char TOK_NOP[] PROGMEM		= "NOP";

const char TOK_GOTO[] PROGMEM 		= "GOTO";
const char TOK_SET[] PROGMEM 		= "SET";

const char TOK_ADD[] PROGMEM 		= "ADD";
const char TOK_SUB[] PROGMEM 		= "SUB";
const char TOK_MULT[] PROGMEM 		= "MULT";
const char TOK_DIV[] PROGMEM 		= "DIV";

const char TOK_SKEQ[] PROGMEM 		= "SKEQ";
const char TOK_SKGR[] PROGMEM 		= "SKGR";
const char TOK_SKLT[] PROGMEM 		= "SKLT";
const char TOK_SKGREQ[] PROGMEM 	= "SKGREQ";
const char TOK_SKLTEQ[] PROGMEM 	= "SKLTEQ";

const char TOK_EXIT[] PROGMEM 		= "EXIT";

const char TOK_FUNCTION[] PROGMEM 	= "FUNC";

//function text
const char TOK_DELAY[] PROGMEM 		= "DELAY";
const char TOK_CLEAR[] PROGMEM 		= "CLEAR";
const char TOK_PRINT[] PROGMEM 		= "PRINT";

const char *CMD_TEXT_TABLE[] PROGMEM = {
	TOK_NOP,
	TOK_GOTO,
	TOK_SET,
	TOK_ADD,
	TOK_SUB,
	TOK_MULT,
	TOK_DIV,
	TOK_SKEQ,
	TOK_SKGR,
	TOK_SKLT,
	TOK_SKGREQ,
	TOK_SKLTEQ,
	TOK_EXIT,
	TOK_FUNCTION
};

const char *FUNC_TEXT_TABLE[] PROGMEM = {
	TOK_DELAY,
	TOK_CLEAR,
	TOK_PRINT
};

const unsigned char TOKEN_FORMAT[] PROGMEM = {
	TYPE_NONE, TYPE_NONE,	//NOP
	TYPE_HIGH, TYPE_LOW,	//GOTO
	TYPE_VAR, TYPE_VAR,		//SET
	TYPE_VAR, TYPE_VAR,		//ADD
	TYPE_VAR, TYPE_VAR,		//SUB
	TYPE_VAR, TYPE_VAR,		//MULT
	TYPE_VAR, TYPE_VAR,		//DIV
	TYPE_VAR, TYPE_VAR,		//SKEQ
	TYPE_VAR, TYPE_VAR,		//SKGR
	TYPE_VAR, TYPE_VAR,		//SKLT
	TYPE_VAR, TYPE_VAR,		//SKGREQ
	TYPE_VAR, TYPE_VAR,		//SKLTEQ
	TYPE_NONE, TYPE_NONE,	//EXIT
	TYPE_FUNC, TYPE_NONE	//FUNCTION
};

unsigned char program[PROG_LENGTH];

unsigned int render_pos;
unsigned int position;
unsigned char part;
unsigned char last_jump;

void editor_run(void)
{
	program[0] = 3;
	program[1] = 1;
	program[2] = 2;
	program[3] = 0;
	program[4] = 0;

	screen_clear(0x00);
	
	position = 0;
	part = 0;
	
	render();
	
	while(1)
	{
		char key = key_get_key();
		
		if(key==KEY_SHIFT)		//left
		{
			if(part>0)
			{
				part--;
				render();
			}
		} else if(key=='w')		//up
		{
			if(position>0)
			{
				position--;
				part = 0;
				
				if(position<render_pos)
				{
					render_pos--;
				}
				
				render();
			}
		} else if(key=='s')		//right
		{
			switch(part)
			{
				case 0:
					if(get_type(program[position], 0)!=TYPE_NONE)
					{
						part++;
						render();
					}
					break;
				case 1:
					if(get_type(program[position], 1)!=TYPE_NONE)
					{
						part++;
						render();
					}
					break;
			}
		} else if(key==' ')		//down
		{
			if(position<PROG_LENGTH-1)
			{
				position++;
				part = 0;
				
				if(position>=render_pos+6)
				{
					render_pos++;
				}
				
				render();
			}
		} else if(key=='>'&&render_pos<63)
		{
			inc_line();
			render();
		} else if(key=='<'&&render_pos>0)
		{
			dec_line();
			render();
		} else {
			unsigned char type = get_type(position, part);
			
			if(type==TYPE_CMD)
			{
			
			} else if(type==TYPE_LOW||type==TYPE_HIGH)
			{
				if((key>='0'&&key<='9')||(key>='a'&&key<='f'))
				{
				
				}
			} else if(type==TYPE_VAR)
			{
				if(key>='a'&&key<='z')
				{
				
				}
			} else if(type==TYPE_FUNC)
			{
			
			}
		}
	}
}

unsigned char list_functions()
{
	return 0;
}

void inc_line()
{
	unsigned char instruction = program[position];
	
	if(get_type(instruction, 0)==TYPE_NONE)
	{
		last_jump = 1;
	} else if(get_type(instruction, 1)==TYPE_NONE)
	{
		last_jump = 2;
	} else {
		last_jump = 3;
	}
	
	position += last_jump;
}

void dec_line()
{
	position -= last_jump;
}

unsigned char get_type(unsigned char instruction, unsigned char param)
{
	if(param==0||param==1)
		return pgm_read_word(&TOKEN_FORMAT[instruction*2+param]);
	else
		return TYPE_NONE;
}

void insert_tok(int pos, unsigned char val)
{

}

void delete_tok(int pos)
{

}

void render(void)
{
	screen_clear(0x00);
	
	int pos_save = position;
	
	for(int i=0; i<6; i++)
	{
		int rel_pos = render_pos+i;
		unsigned char instruction = program[rel_pos];
		unsigned char param1 = program[rel_pos+1];
		unsigned char param2 = program[rel_pos+2];
		
		
		if(rel_pos==pos_save&&part==0) { invert = TRUE; } else { invert = FALSE; }
		print_tok(instruction, TYPE_CMD);									//print command
		if(rel_pos==pos_save&&part==1) { invert = TRUE; } else { invert = FALSE; }
		print_tok(param1, pgm_read_word(&TOKEN_FORMAT[instruction*2]));		//print first parameter
		if(rel_pos==pos_save&&part==2) { invert = TRUE; } else { invert = FALSE; }
		print_tok(param2, pgm_read_word(&TOKEN_FORMAT[instruction*2+1]));	//print second parameter
		
		inc_line();
		
		if(i!=5) { screen_newline(); }
	}
	
	position = pos_save;
}

void print_tok(unsigned char val, unsigned char type)
{
	switch(type)
	{
		case TYPE_CMD:
			print_cmd(val); 
			screen_print_char(' ');
			break;
		case TYPE_HIGH:
			screen_print_char('<');
			print_hex(val);
			break;
		case TYPE_LOW:
			print_hex(val); 
			screen_print_char('>');
			screen_print_char(' ');
			break;
		case TYPE_VAR:
			screen_print_char('[');
			print_var(val);
			screen_print_char(']');
			screen_print_char(' ');
			break;
		case TYPE_FUNC:
			print_func(val);
			break;
	}
}

void print_cmd(unsigned char cmd)
{
	int addr = 0;
	char c;
	while((c=pgm_read_byte(pgm_read_word(&CMD_TEXT_TABLE[cmd])+addr)))
	{
		screen_print_char(c);
		addr++;
	}
}

void print_hex(unsigned char val)
{
	screen_print_char(pgm_read_word(&HEX_TABLE[val>>4]));
	screen_print_char(pgm_read_word(&HEX_TABLE[val&0x0F]));
}

void print_var(unsigned char var)
{
	var += 65; //put it into the ASCII alphabet range
	screen_print_char(var);
}

void print_func(unsigned char func)
{
	int addr = 0;
	char c;
	while((c=pgm_read_byte(pgm_read_word(&FUNC_TEXT_TABLE[func])+addr)))
	{
		screen_print_char(c);
		addr++;
	}
}*/