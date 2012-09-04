#include <inttypes.h>
#include <avr/pgmspace.h>

#include "LCD.h"
#include "characters.h"
#include "font.h"

#define CHAR_WIDTH  38
#define CHAR_HEIGHT 29

#define START_CHAR	32 //the ascii value of the first character in the font
;
uint8_t row			= 1;		//cursor row position
uint8_t col			= 1;		//cursor column position
uint8_t background;	//index of background color value in palette
uint8_t foreground;	//index of foreground color value in palette

//draw a character to the screen
void draw_char(uint8_t row, uint8_t col, char c)
{
	//charBuff[row*CHAR_WIDTH+col] = c;

	int x = (CHAR_WIDTH-col)*8;						//find the LCD x position from the col
	int y = row*8;						//find the LCD y position from the row
	int index = (c - START_CHAR)*8;	//find the start index of the character to draw from the font
	
	for(int column=0; column<8; column++)
	{
		lcd_set_pos(x, y+column);
		for(int bit=0; bit<8; bit++)
		{
			//draw a pixel on the screen for the bit in the column within the character
			//GRAM address auto-increments
			if(((pgm_read_byte(&font[index+column])>>bit)&0x01)==0)
			{
				lcd_write_gram(0x0000);
			} else {
				lcd_write_gram(0xFFFF);
			}
		}
	}
}

/*
//put character at position in char buffer without drawing to the screen
void putChar(uint8_t row, uint8_t col, char c)
{
	charBuff[row*CHAR_WIDTH+col] = c;
}
*/

//print character on screen and automatically increment/wrap cursor
void print_char(uint8_t c)
{
	draw_char(row, col, c);
	
	col++;
	if(col>CHAR_WIDTH)
	{
		col = 1;
		row++;
	}
}

void print_string(uint8_t *addr)
{
	char c;
	while((c=*addr))
	{
		print_char(c);
		addr++;
	}
}

/*
//draw the characters in the buffer to the screen
void drawCharBuff()
{
	for(int y=0; y<CHAR_WIDTH; y++)
	{
		for(int x=0; x<CHAR_HEIGHT; x++)
		{
			drawChar(x, y, charBuff[y*CHAR_WIDTH+x]);
		}
	}
}

//get the ascii value of the char at the position in the buffer
char getChar(uint8_t row, uint8_t col)
{
	return charBuff[row*CHAR_WIDTH+col];
}
*/