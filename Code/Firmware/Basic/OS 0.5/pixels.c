#include "LCD.h"
#include "pixels.h"

#define PX_WIDTH  320
#define PX_HEIGHT 240

void lcd_clear(unsigned int color)
{
	lcd_set_pos(0, 0);
	lcd_set_reg(REG_GRAM);
	for(int y=0; y<PX_HEIGHT; y++)
	{
		for(int x=0; x<PX_WIDTH; x++)
		{
			lcd_write_data(color);
		}
	}
	lcd_set_pos(0, 0);
}

/*
uint_8 pxBuff[PX_WIDTH*PX_HEIGHT];

int palette[16];

uint_8 background;	//index of background color value in palette
uint_8 foreground;	//index of foreground color value in palette

//draw a pixel of the color specified by the palette index at the position
//disregards buffer
void drawPixel(uint_8 x, uint_8 y, uint_8 index)
{
	int color = palette[index];

	setPos(x, y);
	writeGRAM(color);
}

//draw pixel with foreground color to screen and to buffer
void pixelOn(uint_8 x, uint_8 y)
{
	SET_BIT(pxBuff[y*PX_WIDTH+x], y-y/8);
	setPos(x, y);
	writeGRAM(foreground);
}

//draw pixel with background color to screen and to buffer
void pixelOff(uint_8 x, uint_8 y)
{
	CLR_BIT(pxBuff[y*PX_WIDTH+x], y-y/8);
	setPos(x, y);
	writeGRAM(background);
}

void drawBuff()
{
	for(int y=0; y<PX_WIDTH; y++)
	{
		for(int x=0; x<PX_WIDTH; x++)
		{
			if(GET_BIT(pxBuff[y*PX_WIDTH+x])==1)
			{
				writeGRAM(palette[foreground]);
			} else {
				writeGRAM(palette[background]);
			}
		}
	}
}
*/