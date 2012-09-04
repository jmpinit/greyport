#include <avr/io.h>
#include <util/delay.h>

#include "Screen.h"
#include "Keyboard.h"
#include "Serial.h"

//instruction defs
#define GOTO		1	//high 8 bits of addr, low 8 bits of addr

#define SET			2	//var, 16 bit val

#define ADD			3	//var src1, var src2, var dest
#define SUB			4	//var src1, var src2, var dest
#define MULT		5	//var src1, var src2, var dest
#define DIV			6	//var src1, var src2, var dest

#define IF_EQ		7	//var src1, var src2
#define IF_GR		8	//var src1, var src2
#define IF_LESS		9	//var src1, var src2
#define IF_GREQ		10	//var src1, var src2
#define IF_LESSEQ	11	//var src1, var src2

#define FUNCTION	12	//function

#define EXIT		13

//function defs
#define DELAY		0	//var time
#define CLEAR		1	//clears the screen
#define PRINT		2	//character
#define PRINT_VAR	3	//byte val
#define DIRA		4	//var state
#define DIRB		5	//var state
#define DIRC		6	//var state
#define DIRD		7	//var state
#define OUTA		8	//var state
#define OUTB		9	//var state
#define OUTC		10	//var state
#define OUTD		11	//var state
#define INA			12	//var dest
#define INB			13	//var dest
#define INC			14	//var dest
#define IND			15	//var dest

unsigned char* mem;
unsigned int length;

int variables[26];

void bas_run(unsigned char* p, unsigned int l)
{
	mem = p;
	length = l;
	
	//initialize the variables
	for(unsigned int i=0; i<26; i++)
	{
		variables[i] = 0;
	}

	int pc = 0;
	
	while(pc<length)
	{
		unsigned char instruction = mem[pc];
		
		switch(instruction)
		{
			case GOTO:
				pc = (unsigned int)(mem[pc+1]<<8)|mem[pc+2];
				break;
				
			case SET:
				variables[mem[pc+1]] = (unsigned int)(mem[pc+2]<<8)|mem[pc+3];
				pc += 4;
				break;
				
			case ADD:
				variables[mem[pc+1]] = variables[mem[pc+2]]+variables[mem[pc+3]];
				pc += 4;
				break;
				
			case SUB:
				variables[mem[pc+1]] = variables[mem[pc+2]]-variables[mem[pc+3]];
				pc += 4;
				break;
				
			case MULT:
				variables[mem[pc+1]] = variables[mem[pc+2]]*variables[mem[pc+3]];
				pc += 4;
				break;
				
			case DIV:
				variables[mem[pc+1]] = variables[mem[pc+2]]/variables[mem[pc+3]];
				pc += 4;
				break;
				
			case IF_EQ:
				if(variables[mem[pc+1]]==variables[mem[pc+2]])
				{
					pc += 4;
				} else {
					pc += 3;
				}
				break;
				
			case IF_GR:
				if(variables[mem[pc+1]]>variables[mem[pc+2]])
				{
					pc += 4;
				} else {
					pc += 3;
				}
				break;
			
			case IF_LESS:
				if(variables[mem[pc+1]]<variables[mem[pc+2]])
				{
					pc += 4;
				} else {
					pc += 3;
				}
				break;
				
			case IF_GREQ:
				if(variables[mem[pc+1]]>=variables[mem[pc+2]])
				{
					pc += 4;
				} else {
					pc += 3;
				}
				break;
				
			case IF_LESSEQ:
				if(variables[mem[pc+1]]<=variables[mem[pc+2]])
				{
					pc += 4;
				} else {
					pc += 3;
				}
				break;
				
			case EXIT:
				pc = length;
				break;
				
			case FUNCTION:
				pc++;
				switch(mem[pc])
				{
					case DELAY:
						_delay_ms(variables[mem[pc+1]]);
						pc += 2;
						break;
						
					case PRINT:
						screen_print_char((char)mem[pc+1]);
						pc += 2;
						break;
						
					case PRINT_VAR:
						//screen_print_hex(variables[0]);
						screen_print_hex((unsigned char)(variables[mem[pc+1]]>>8));
						screen_print_hex((unsigned char)(variables[mem[pc+1]]&0x00FF));
						screen_newline();
						pc += 2;
						break;
					
					case CLEAR:
						screen_clear(0x00);
						pc++;
						break;
						
					case DIRA:
						DDRA = variables[mem[pc+1]];
						pc += 2;
						break;
					case DIRB:
						DDRB = variables[mem[pc+1]];
						pc += 2;
						break;
					case DIRC:
						DDRC = variables[mem[pc+1]];
						pc += 2;
						break;
					case DIRD:
						DDRD = variables[mem[pc+1]];
						pc += 2;
						break;
					
					case OUTA:
						PORTA = variables[mem[pc+1]];
						pc += 2;
						break;
					case OUTB:
						PORTB = variables[mem[pc+1]];
						pc += 2;
						break;
					case OUTC:
						PORTC = variables[mem[pc+1]];
						pc += 2;
						break;
					case OUTD:
						PORTD = variables[mem[pc+1]];
						pc += 2;
						break;
						
					case INA:
						variables[mem[pc+1]] = PORTA;
						pc += 2;
						break;
					case INB:
						variables[mem[pc+1]] = PORTB;
						pc += 2;
						break;
					case INC:
						variables[mem[pc+1]] = PORTC;
						pc += 2;
						break;
					case IND:
						variables[mem[pc+1]] = PORTD;
						pc += 2;
						break;
					
					default:
						pc++;
						break;
				}
				break;
				
			default:
				pc++;
				break;
		}
	}
}