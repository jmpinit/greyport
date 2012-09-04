#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

#define BUS_LOW		PORTD
#define BUS_HIGH	PORTB

#define PORT_CTRL PORTC
#define PIN_RESET	0
#define PIN_RS		1
#define PIN_WR		2
#define PIN_RD		3
#define PIN_CS		4

//set the GRAM address for the LCD
void lcd_set_pos(int x, int y)
{
	lcd_set_xpos(x);
	lcd_set_ypos(y);
}

//set the GRAM x position for the LCD
void lcd_set_xpos(int x)
{
	lcd_write_reg(REG_XPOS, x);
}

//set the GRAM y position for the LCD
void lcd_set_ypos(int y)
{
	lcd_write_reg(REG_YPOS, y);
}

//draw a pixel with the specified color at the cursor
void lcd_write_gram(int color)
{
	lcd_write_reg(REG_GRAM, color);
}

/*
//4 pixels on screen for every GRAM address
//reduces effective screen resolution to 160x120
void pixelMode() {}

//1 pixel on screen for every GRAM address
void charMode() {}
*/

void lcd_write_data(int val)
{
	PORT_CTRL |= (1<<PIN_RD)|(1<<PIN_RS);
	PORT_CTRL &= ~((1<<PIN_CS)|(1<<PIN_WR));
	
	BUS_LOW		= val&0x00FF;
	BUS_HIGH	= val>>8;
	
	PORT_CTRL |= (1<<PIN_CS)|(1<<PIN_WR);
}

void lcd_set_reg(int index)
{
	PORT_CTRL |= (1<<PIN_RD);
	PORT_CTRL &= ~((1<<PIN_CS)|(1<<PIN_RS)|(1<<PIN_WR));
	
	BUS_LOW		= index&0x00FF;
	BUS_HIGH	= index>>8;
	
	PORT_CTRL |= (1<<PIN_CS)|(1<<PIN_WR);
}

void lcd_write_reg(int index, int val)
{
	lcd_set_reg(index);
	lcd_write_data(val);
}

void lcd_initialize()
{
	//set port direction
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	
	PORT_CTRL |= (1<<PIN_RESET);
	_delay_ms(100);

	PORT_CTRL &= ~(1<<PIN_RESET);
	_delay_ms(100);

	PORT_CTRL = (1<<PIN_RESET)|(1<<PIN_CS)|(1<<PIN_RD)|(1<<PIN_WR);
	_delay_ms(100);
	
	lcd_write_reg(0x00E3, 0x3008);
	lcd_write_reg(0x00E7, 0x0012);
	lcd_write_reg(0x00EF, 0x1231); // set the internal timing;    
	lcd_write_reg(0x0000, 0x0001); // start fosc
	lcd_write_reg(0x0001, 0x0100); // set SS bit
	lcd_write_reg(0x0002, 0x0700); // set 1 line inversion
	lcd_write_reg(0x0003, 0x1018); // gram direction

	lcd_write_reg(0x0004, 0x0000); // Resize register
	lcd_write_reg(0x0008, 0x0202); // set the back porch and front porch
	lcd_write_reg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	lcd_write_reg(0x000A, 0x0000); // FMARK function
	lcd_write_reg(0x000C, 0x0000); // RGB interface setting
	lcd_write_reg(0x000D, 0x0000); // Frame marker Position
	lcd_write_reg(0x000F, 0x0000); // RGB interface polarity

	//Power On sequence //
	lcd_write_reg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	lcd_write_reg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	lcd_write_reg(0x0012, 0x0000); // VREG1OUT voltage
	lcd_write_reg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	_delay_ms(200);

	lcd_write_reg(0x0010, 0x1490); // SAP, BT[3:0], AP, DSTB, SLP, STB
	lcd_write_reg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
	_delay_ms(100);

	lcd_write_reg(0x0012, 0x001C); // VREG1OUT voltage
	_delay_ms(100);

	lcd_write_reg(0x0013, 0x1A00); // VDV[4:0] for VCOM amplitude
	lcd_write_reg(0x0029, 0x0025); // Set VCM[5:0] for VCOMH
	lcd_write_reg(0x002B, 0x000C); // Set Frame Rate
	_delay_ms(100);

	lcd_write_reg(0x0020, 0x0000); // GRAM horizontal
	lcd_write_reg(0x0021, 0x0000); // GRAM vertical

	// Adjust the Gamma Curve
	lcd_write_reg(0x0030, 0x0107);
	lcd_write_reg(0x0031, 0x0306);
	lcd_write_reg(0x0032, 0x0207);
	lcd_write_reg(0x0035, 0x0206);
	lcd_write_reg(0x0036, 0x0408);
	lcd_write_reg(0x0037, 0x0106);
	lcd_write_reg(0x0038, 0x0102);
	lcd_write_reg(0x0039, 0x0207);
	lcd_write_reg(0x003C, 0x0504);
	lcd_write_reg(0x003D, 0x0503);

	// Set GRAM area
	lcd_write_reg(0x0050, 0x0000); // Horizontal GRAM Start Address
	lcd_write_reg(0x0051, 0x00EF); // Horizontal GRAM End Address
	lcd_write_reg(0x0052, 0x0000); // Vertical GRAM Start Address
	lcd_write_reg(0x0053, 0x013F); // Vertical GRAM Start Address
	lcd_write_reg(0x0060, 0xA700); // Gate Scan Line 0xA700
	lcd_write_reg(0x0061, 0x0001); // NDL, VLE, REV
	lcd_write_reg(0x006A, 0x0000); // set scrolling line

	// Partial Display Control
	lcd_write_reg(0x0080, 0x0000);
	lcd_write_reg(0x0081, 0x0000);
	lcd_write_reg(0x0082, 0x0000);
	lcd_write_reg(0x0083, 0x0000);
	lcd_write_reg(0x0084, 0x0000);
	lcd_write_reg(0x0085, 0x0000);

	// Panel Control
	lcd_write_reg(0x0090, 0x0010);
	lcd_write_reg(0x0092, 0x0600);

	// Turn Display ON 
	lcd_write_reg(0x0007, 0x0133); // 262K color and display ON
}