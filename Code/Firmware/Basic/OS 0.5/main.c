#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "LCD.h"
#include "pixels.h"
#include "characters.h"

// Inline assembly: The nop = do nothing for one clock cycle.
#define nop()  __asm__ __volatile__("nop")

int main(void)
{
	//start the LCD
	lcd_initialize();
    
    for(;;)
    {
		lcd_clear(0x0000);
		
		print_string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		
		PORTC |= (1<<5);
		_delay_ms(10000);
		
		lcd_clear(0xFFFF);
		
		PORTC &= ~(1<<5);
		_delay_ms(1000);
    }
    
    // Never reached.
    return(0);
}