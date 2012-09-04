.include "m168def.inc"

.MACRO WRITE_REG
	ldi		r16, low(@0)
	ldi		r17, high(@0)
	ldi		r18, low(@1)
	ldi		r19, high(@1)
	rcall	lcd_wr_reg
.ENDMACRO

.MACRO DRAW_CHAR
	ldi		XL, @0
	ldi		YL, @1
	ldi		ZL, @2
	rcall	lcd_draw_char
.ENDMACRO

.MACRO SELECT_MODE
	//set the LCD to select-register mode
	sbi		portCtrl, pinRD		;RD high
	cbi		portCtrl, pinRS		;RS low
	cbi		portCtrl, pinWR		;WR low
.ENDMACRO

.MACRO WRITE_MODE
	//set the LCD to write-register mode
	sbi		portCtrl, pinRD		;RD high
	sbi		portCtrl, pinRS		;RS high
	cbi		portCtrl, pinWR		;WR low
.ENDMACRO

.equ	busHigh		= PORTB
.equ	busLow		= PORTD
.equ	portCtrl	= PORTC

.equ	pinRest = 0		;PC0
.equ	pinRS	= 1		;PC1
.equ	pinWR	= 2		;PC2
.equ	pinRD	= 3		;PC3
.equ	pinCS	= 4		;PC4
.equ	pinLED	= 5		;PC5

.cseg
.org 0

rjmp reset

delay:
	ldi		r16, 0
	ldi		r17, 0
delay0:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	dec		r16
	brne	delay0
delay1:
	dec		r16
	dec		r17
	brne	delay0

	ret

.include "control.asm"
.include "pixel.asm"

;---CHARACTER ROUTINES---

.equ CHAR_WIDTH			= 40				;number of chars on one row
.equ CHAR_HEIGHT		= 30				;number of chars in one column
.equ START_CHAR			= ' '				;first character in the font (for calculating the index)
//.equ START_BUFF_CHAR	= SRAM_START + 2400	;start of the character buffer in SRAM
//.equ END_BUFF_CHAR	= SRAM_START + 3599	;address of last character in buffer in SRAM

.equ ROW				= SRAM_START
.equ COL				= SRAM_START + 1
.equ BACKGROUND			= SRAM_START + 2
.equ FOREGROUND			= SRAM_START + 3

//color indexes
.equ BLACK				= 0
.equ WHITE				= 1
.equ RED				= 2
.equ GREEN				= 3
.equ BLUE				= 4

;INPUT: YL row, XL column, ZL ascii code of character
;DESTROYS: r16, r17, r18, r19, X, Y, Z
lcd_draw_char:
	cbi		portCtrl, pinCS
//set screen x position
	//flip column value
	//because x values flipped on screen
	ldi		r16, CHAR_WIDTH
	sub		r16, XL
	mov		XL, r16

	//multiply column by 8
	//to get x position on screen
	clr		XH
	clr		r16
	lsl		XL
	adc		XH, r16
	lsl		XL
	adc		XH, r16
	lsl		XL
	adc		XH, r16
	
	SELECT_MODE

	//tell LCD we want to write the x position
	//by switching to the x position register
	ldi		r16, $21
	out		busLow, r16
	ldi		r16, $00
	out		busHigh, r16
	sbi		portCtrl, pinWR		;WR high

	WRITE_MODE
	
	//write the x position
	out		busLow, XL
	out		busHigh, XH
	sbi		portCtrl, pinWR		;WR high

//set screen y position
	//multiply row by 8
	//to get y position on screen
	clr		YH
	clr		r16
	lsl		YL
	adc		YH, r16
	lsl		YL
	adc		YH, r16
	lsl		YL
	adc		YH, r16

	SELECT_MODE

	//tell LCD we want to write the y position
	//by switching to the y position register
	ldi		r16, $20
	out		busLow, r16
	ldi		r16, $00
	out		busHigh, r16
	sbi		portCtrl, pinWR		;WR high

	WRITE_MODE
	
	//write the y position
	out		busLow, YL
	out		busHigh, YH
	sbi		portCtrl, pinWR		;WR high

//calculating address of character image
	//chop off the invisible characters
	//ZL will contain char number in font
	subi	ZL, START_CHAR

	//multiply Z by 8
	//Z will contain index of char image in memory
	clr		ZH
	clr		r16
	lsl		ZL
	adc		ZH, r16
	lsl		ZL
	adc		ZH, r16
	lsl		ZL
	adc		ZH, r16
	
	//add Z to the position of the font in memory
	//Z will contain address of first image byte of char
	mov		r16, ZL
	mov		r17, ZH
	ldi		ZL, low(font_phat<<1)
	ldi		ZH, high(font_phat<<1)
	add		ZL, r16
	ldi		r16, 0
	adc		ZH, r16
	add		ZH, r17
lcd_draw_char1:
	//byte count
	ldi		r17, 8
lcd_draw_char2:
	//load next image byte
	lpm		r18, Z+

	SELECT_MODE

	//select the GRAM access register
	ldi		r16, $22
	out		busLow, r16
	clr		r16
	out		busHigh, r16
	sbi		portCtrl, pinWR		;WR high	

	//bit count
	ldi		r16, 8

	WRITE_MODE
lcd_draw_char3:
	//what color is pixel?
	sbrc	r18, 0
	rjmp	lcd_draw_charF
lcd_draw_charB:
	//draw background colored pixel
	cbi		portCtrl, pinWR		;WR low
	ldi		r19, $00
	out		busLow, r19
	ldi		r19, $00
	out		busHigh, r19
	sbi		portCtrl, pinWR		;WR high

	rjmp	lcd_draw_char4
lcd_draw_charF:
	//draw foreground colored pixel
	cbi		portCtrl, pinWR		;WR low
	ldi		r19, $FF
	out		busLow, r19
	ldi		r19, $FF
	out		busLow, r19
	sbi		portCtrl, pinWR		;WR high
lcd_draw_char4:
//shift next bit to draw
	lsr		r18

//done drawing byte?
	dec		r16
	brne	lcd_draw_char3

//move to next line
	SELECT_MODE

	//select the x position register
	ldi		r16, $21
	out		busLow, r16
	clr		r16
	out		busHigh, r16
	sbi		portCtrl, pinWR		;WR high

	WRITE_MODE

	//write the x position
	out		busLow, XL
	out		busHigh, XH
	sbi		portCtrl, pinWR		;WR high

	//increment the Y position
	ld		r16, Y+

	SELECT_MODE

	//select the y position register
	ldi		r16, $20
	out		busLow, r16
	clr		r16
	out		busHigh, r16
	sbi		portCtrl, pinWR		;WR high

	WRITE_MODE

	//write the y position
	out		busLow, YL
	out		busHigh, YH
	sbi		portCtrl, pinWR		;WR high

//done drawing character?
	dec		r17
	brne	lcd_draw_char2

	//tell the LCD we don't want to talk to it for now
	sbi		portCtrl, pinCS		;CS high

	ret

reset:
	//pin mode
	ldi		r16, $FF
	out		DDRB, r16
	out		DDRC, r16
	out		DDRD, r16

	rcall	lcd_init

forever:
	ldi		ZL, $00
	ldi		ZH, $00
	rcall	lcd_fill

	sbi		portCtrl, pinLED

	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay

	DRAW_CHAR 0, 0, '*'
	DRAW_CHAR 0, 2, '*'
	DRAW_CHAR 13, 0, '*'
	DRAW_CHAR 13, 2, '*'

	DRAW_CHAR 0, 1, '['

	DRAW_CHAR 1, 1, 'H'
	DRAW_CHAR 2, 1, 'E'
	DRAW_CHAR 3, 1, 'L'
	DRAW_CHAR 4, 1, 'L'
	DRAW_CHAR 5, 1, 'O'

	DRAW_CHAR 7, 1, 'W'
	DRAW_CHAR 8, 1, 'O'
	DRAW_CHAR 9, 1, 'R'
	DRAW_CHAR 10, 1, 'L'
	DRAW_CHAR 11, 1, 'D'
	DRAW_CHAR 12, 1, '!'

	DRAW_CHAR 13, 1, ']'

	DRAW_CHAR 1, 0, '-'
	DRAW_CHAR 2, 0, '-'
	DRAW_CHAR 3, 0, '-'
	DRAW_CHAR 4, 0, '-'
	DRAW_CHAR 5, 0, '-'
	DRAW_CHAR 6, 0, '-'
	DRAW_CHAR 7, 0, '-'
	DRAW_CHAR 8, 0, '-'
	DRAW_CHAR 9, 0, '-'
	DRAW_CHAR 10, 0, '-'
	DRAW_CHAR 11, 0, '-'
	DRAW_CHAR 12, 0, '-'

	DRAW_CHAR 1, 2, '-'
	DRAW_CHAR 2, 2, '-'
	DRAW_CHAR 3, 2, '-'
	DRAW_CHAR 4, 2, '-'
	DRAW_CHAR 5, 2, '-'
	DRAW_CHAR 6, 2, '-'
	DRAW_CHAR 7, 2, '-'
	DRAW_CHAR 8, 2, '-'
	DRAW_CHAR 9, 2, '-'
	DRAW_CHAR 10, 2, '-'
	DRAW_CHAR 11, 2, '-'
	DRAW_CHAR 12, 2, '-'

	cbi		portCtrl, pinLED

	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay
	rcall	delay

	rjmp	forever

.include "font_phat.asm"
