//---------------------------------------------//
// File: ILI9331.c

//--------------- Required Headers ------------//
#include "ILI9331.h"
#include "font.h"
#include "delay.h"
#include "tu.h"

void LCD_WR_DATA16(unsigned int data)
{
	LCD_RD_H();
	LCD_RS_H();
  LCD_CS_L();  
  LCD_WR_L();
	 
  DATAH_LCD_PORT = data >> 8;
  DATAL_LCD_PORT = data & 0x00FF; 

  LCD_WR_H();
  LCD_CS_H();
  
}
void LCD_WR_REG16(unsigned int index)
{
  LCD_RD_H();
	LCD_RS_L();
  LCD_CS_L();  
  LCD_WR_L();

  DATAH_LCD_PORT = index >> 8; 
	DATAL_LCD_PORT = index & 0x00FF; 

  LCD_WR_H();
  LCD_CS_H();
}

void LCD_WR_REG(unsigned int index,unsigned int data)
{
	LCD_WR_REG16(index);
  LCD_WR_DATA16(data);
}


void LCD_Init(void)
{
	// Init data pins
	DATA_OUTPUT();

	// Init control pins
	LCD_BL_DDR |= 1<<LCD_BL_BIT;
	LCD_CS_DDR |= 1<<LCD_CS_BIT;
	LCD_RS_DDR |= 1<<LCD_RS_BIT;
	LCD_WR_DDR |= 1<<LCD_WR_BIT;
	LCD_RD_DDR |= 1<<LCD_RD_BIT;
	LCD_RST_DDR|= 1<<LCD_RST_BIT;

	LCD_RST_L();
	delay_ms(10);
	LCD_RST_H();
	delay_ms(50); 

	// Start init sequence 
	LCD_WR_REG(0x00E3, 0x3008);        
	LCD_WR_REG(0x00E7, 0x0012);
	LCD_WR_REG(0x00EF, 0x1231); // set the internal timing;    
 	LCD_WR_REG(0x0000, 0x0001); // start fosc
 	LCD_WR_REG(0x0001, 0x0100); // set SS bit
 	LCD_WR_REG(0x0002, 0x0700); // set 1 line inversion

	#if   ID_AM==000       
		LCD_WR_REG(0x0003, 0x1000);
	#elif ID_AM==001        
		LCD_WR_REG(0x0003, 0x1008);      
	#elif ID_AM==010  
		LCD_WR_REG(0x0003, 0x1010);        
	#elif ID_AM==011
		LCD_WR_REG(0x0003, 0x1018);
	#elif ID_AM==100  
		LCD_WR_REG(0x0003, 0x1020);      
	#elif ID_AM==101  
		LCD_WR_REG(0x0003, 0x1028);      
	#elif ID_AM==110  
		LCD_WR_REG(0x0003, 0x1030);      
	#elif ID_AM==111  
		LCD_WR_REG(0x0003, 0x1038);
	#endif    
  
	LCD_WR_REG(0x0004, 0x0000); // Resize register
  LCD_WR_REG(0x0008, 0x0202); // set the back porch and front porch
  LCD_WR_REG(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
  LCD_WR_REG(0x000A, 0x0000); // FMARK function
  LCD_WR_REG(0x000C, 0x0000); // RGB interface setting
  LCD_WR_REG(0x000D, 0x0000); // Frame marker Position
  LCD_WR_REG(0x000F, 0x0000); // RGB interface polarity

	//Power On sequence //
  LCD_WR_REG(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
  LCD_WR_REG(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
  LCD_WR_REG(0x0012, 0x0000); // VREG1OUT voltage
  LCD_WR_REG(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	delay_ms(200);

	LCD_WR_REG(0x0010, 0x1490); // SAP, BT[3:0], AP, DSTB, SLP, STB
  LCD_WR_REG(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
	delay_ms(50);

  LCD_WR_REG(0x0012, 0x001C); // VREG1OUT voltage
	delay_ms(50);

  LCD_WR_REG(0x0013, 0x1A00); // VDV[4:0] for VCOM amplitude
	LCD_WR_REG(0x0029, 0x0025); // Set VCM[5:0] for VCOMH
  LCD_WR_REG(0x002B, 0x000C); // Set Frame Rate
	delay_ms(50);

	// GRAM Setup    
	#if   ID_AM==000         
		LCD_WR_REG(0x0020, 0x00ef);
		LCD_WR_REG(0x0021, 0x013f);      
	#elif ID_AM==001
		LCD_WR_REG(0x0020, 0x00ef);
		LCD_WR_REG(0x0021, 0x013f);      
	#elif ID_AM==010
		LCD_WR_REG(0x0020, 0x0000);
		LCD_WR_REG(0x0021, 0x013f);      
	#elif ID_AM==011
		LCD_WR_REG(0x0020, 0x0000);
		LCD_WR_REG(0x0021, 0x013f);       
	#elif ID_AM==100
		LCD_WR_REG(0x0020, 0x00ef);
		LCD_WR_REG(0x0021, 0x0000);      
	#elif ID_AM==101  
		LCD_WR_REG(0x0020, 0x00ef);
		LCD_WR_REG(0x0021, 0x0000);      
	#elif ID_AM==110
		LCD_WR_REG(0x0020, 0x0000);
		LCD_WR_REG(0x0021, 0x0000);      
	#elif ID_AM==111
		LCD_WR_REG(0x0020, 0x0000);
		LCD_WR_REG(0x0021, 0x0000);         
	#endif

	// Adjust the Gamma Curve
	LCD_WR_REG(0x0030, 0x0107);
	LCD_WR_REG(0x0031, 0x0306);
	LCD_WR_REG(0x0032, 0x0207);
	LCD_WR_REG(0x0035, 0x0206);
	LCD_WR_REG(0x0036, 0x0408);
	LCD_WR_REG(0x0037, 0x0106);
	LCD_WR_REG(0x0038, 0x0102);
	LCD_WR_REG(0x0039, 0x0207);
	LCD_WR_REG(0x003C, 0x0504);
	LCD_WR_REG(0x003D, 0x0503);
		 
	// Set GRAM area
  LCD_WR_REG(0x0050, 0x0000); // Horizontal GRAM Start Address
  LCD_WR_REG(0x0051, 0x00EF); // Horizontal GRAM End Address
 	LCD_WR_REG(0x0052, 0x0000); // Vertical GRAM Start Address
  LCD_WR_REG(0x0053, 0x013F); // Vertical GRAM Start Address
  LCD_WR_REG(0x0060, 0xA700); // Gate Scan Line 0xA700
  LCD_WR_REG(0x0061, 0x0001); // NDL, VLE, REV
  LCD_WR_REG(0x006A, 0x0000); // set scrolling line

	// Partial Display Control
  LCD_WR_REG(0x0080, 0x0000);
  LCD_WR_REG(0x0081, 0x0000);
  LCD_WR_REG(0x0082, 0x0000);
  LCD_WR_REG(0x0083, 0x0000);
  LCD_WR_REG(0x0084, 0x0000);
  LCD_WR_REG(0x0085, 0x0000);

	// Panel Control
  LCD_WR_REG(0x0090, 0x0010);
  LCD_WR_REG(0x0092, 0x0600);

	// Turn Display ON 
  LCD_WR_REG(0x0007, 0x0133); // 262K color and display ON

	LCD_Clear(BLACK); // Clear screen
	LCD_BL_H(); // Turn backlight on
}

void LCD_Clear(unsigned int Color)
{
 	unsigned long index = 0;
  
  LCD_WR_REG(0x0020, 0x0000);
  LCD_WR_REG(0x0021, 0x0000);      

  LCD_WR_REG(0x0050, 0x0000);
  LCD_WR_REG(0x0051, 239);
  LCD_WR_REG(0x0052, 0x0000);
  LCD_WR_REG(0x0053, 319);
  LCD_WR_REG16(0x0022);
  
  LCD_RD_H();
  LCD_RS_H();
  LCD_CS_L();   
  
	for(index = 0; index < 76800; index++)
  {
  	LCD_WR_L();
  	DATAH_LCD_PORT = Color >> 8; 
  	DATAL_LCD_PORT = Color & 0x00FF; 
    LCD_WR_H();   
  } 
	
	LCD_CS_H();
}

void LCD_write_english(char data,unsigned int color,unsigned int xcolor)
{
  unsigned char avl,i,n;
	
	LCD_WR_REG16(0x0022);  
  for (i=0;i<16;i++)
  { 
		avl=pgm_read_byte( &english_16x8[data-32][i]);
	  for (n=0;n<8;n++)
	  {
	  	if(avl&0x80)
				LCD_WR_DATA16(color);
      else
				LCD_WR_DATA16(xcolor);     
	    avl<<=1;
	  }
	}
}

void LCD_write_english_string(unsigned int x,unsigned int y,char *s,unsigned int color,unsigned int xcolor)
{
	unsigned int k=0;
	
	while (*s) 
  {
  	Lcd_SetBox(y,x+k,15,8);
    LCD_write_english( *s,color,xcolor);
    k=k+8;
    s++;
  }
}  

void LCD_write_SUM(unsigned int x,unsigned int y,unsigned char SUM,unsigned int color,unsigned int xcolor)//Ð´Êý×Ö
{
	unsigned char avl,i,n;
  
	Lcd_SetBox(y,x,15,8);
  LCD_WR_REG16(0x0022);  
  for (i=0;i<16;i++)
  { 
  	avl=pgm_read_byte( &english_16x8[SUM+16][i]);
	  for (n=0;n<8;n++)
	  {
	  	if(avl&0x80)
				LCD_WR_DATA16(color);
      else
				LCD_WR_DATA16(xcolor);  
	    avl<<=1;
	  }
	}
}

void Lcd_SetBox(unsigned int xStart,unsigned int yStart,unsigned int xLong,unsigned int yLong)
{ 
	#if ID_AM==000    
		LCD_SetCursor(xStart+xLong-1,312-yStart+yLong-1);
	#elif ID_AM==001
		LCD_SetCursor(xStart+xLong-1,312-yStart+yLong-1); 
	#elif ID_AM==010
		LCD_SetCursor(xStart,312-yStart+yLong-1);    
	#elif ID_AM==011 
		LCD_SetCursor(xStart,312-yStart+yLong-1);     
	#elif ID_AM==100
		LCD_SetCursor(xStart+xLong-1,312-yStart);          
	#elif ID_AM==101
		LCD_SetCursor(xStart+xLong-1,312-yStart);        
	#elif ID_AM==110
		LCD_SetCursor(xStart,312-yStart);  
	#elif ID_AM==111
		LCD_SetCursor(xStart,312-yStart);  	    
	#endif
     
	LCD_WR_REG(0x0050,xStart);
	LCD_WR_REG(0x0051,xStart+xLong-1);
	LCD_WR_REG(0x0052,312-yStart);
	LCD_WR_REG(0x0053,312-yStart+yLong-1);
}

void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos)
{
  LCD_WR_REG(0x20, Xpos);
  LCD_WR_REG(0x21, Ypos);
}

void GUI_Point(unsigned int x, unsigned int y,unsigned int color) 
{
	LCD_WR_REG(0x0020,x);
  LCD_WR_REG(0x0021,y);

	LCD_WR_REG16(0x0022); 
	LCD_WR_DATA16(color);    
}

void Test(void)
{       
	unsigned long n,i;
  
	LCD_WR_REG(0x0020,0x0000);
  LCD_WR_REG(0x0021,0x0000); 
  for(i=0;i<7;i++)      
  {               
		LCD_WR_REG(0x0050,0x00);
	  LCD_WR_REG(0x0051,239);
	  LCD_WR_REG(0x0052,0x00);
	  LCD_WR_REG(0x0053,319);
           
    LCD_WR_REG16(0x0022);       
    for(n=0;n<76800;n++)
    {               
			if(i==0)
				LCD_WR_DATA16(BLUE);  
      if(i==1)
				LCD_WR_DATA16(RED); 
      if(i==2)
				LCD_WR_DATA16(GREEN);
      if(i==3)
				LCD_WR_DATA16(CYAN); 
      if(i==4)
				LCD_WR_DATA16(MAGENTA); 
      if(i==5)
				LCD_WR_DATA16(YELLOW);				  
      if(i==6)
				LCD_WR_DATA16(BLACK); 
		}    
    delay_ms(100);                   
	}       
}

void Test_1(void)
{
	unsigned int i;
  // unsigned char  val;
  LCD_WR_REG(0x0020,20);
  LCD_WR_REG(0x0021,105);    

	// Start a new window 20,207 to 105,207
  LCD_WR_REG(0x0050,20);
  LCD_WR_REG(0x0051,207);
  LCD_WR_REG(0x0052,105);
  LCD_WR_REG(0x0053,207);
  LCD_WR_REG16(0x0022);
    	
  LCD_RD_H();
  LCD_RS_H();
  LCD_CS_L(); 

  for (i=0;i<21218;)
  { 
  	LCD_WR_L();
    DATAH_LCD_PORT = pgm_read_byte(&gImage[i++]);
    DATAL_LCD_PORT = pgm_read_byte(&gImage[i++]);
    LCD_WR_H(); 
	}

  LCD_CS_H();
}