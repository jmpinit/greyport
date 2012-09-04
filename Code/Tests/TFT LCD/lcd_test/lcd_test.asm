.include "m168def.inc"

.MACRO WRITE_REG
	ldi		r16, low(@0)
	ldi		r17, high(@0)
	ldi		r18, low(@1)
	ldi		r19, high(@1)
	rcall	lcd_wr_reg
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

lcd_init:
	//reset LCD
	sbi		portCtrl, pinRest
	rcall	delay

	cbi		portCtrl, pinRest
	rcall	delay

	ldi		r16, (1<<pinRest)|(1<<pinCS)|(1<<pinRD)|(1<<pinWR)
	out		portCtrl, r16
	rcall	delay
	
	//start initial sequence
	.include	"startup.asm"
	/*
	.include	"sequence_start.asm"
	.include	"sequence_power_on.asm"
	.include	"sequence_gamma.asm"
	.include	"sequence_set_gram.asm"
	.include	"sequence_disp_ctrl.asm"
	.include	"sequence_panel_ctrl.asm"
	*/

	sbi		portCtrl, pinCS		;CS high

	ret

/*
;INPUT r16 low byte, r17 high byte
lcd_write_bus:
	out		busLow, r16			;write low byte on bus
	out		busHigh, r17		;write high byte on bus
	
	cbi		portCtrl, pinWR		;WR low
	nop
	nop
	nop
	nop
	sbi		portCtrl, pinWR		;WR high

	ret
*/

;INPUT: r16 index low, r17 index high
lcd_wr_reg16:
	sbi		portCtrl, pinRD		;RD high
	cbi		portCtrl, pinRS		;RS low
	cbi		portCtrl, pinCS		;CS low
	cbi		portCtrl, pinWR		;WR low

	out		busLow, r16			;write low byte on bus
	out		busHigh, r17		;write high byte on bus

	sbi		portCtrl, pinWR		;WR high
	sbi		portCtrl, pinCS		;CS high
	
	ret	

;INPUT: r16 data low, r17 data high
lcd_wr_data16:
	sbi		portCtrl, pinRD		;RD high
	sbi		portCtrl, pinRS		;RS high
	cbi		portCtrl, pinCS		;CS low
	cbi		portCtrl, pinWR		;WR low

	out		busLow, r16			;write low byte on bus
	out		busHigh, r17		;write high byte on bus

	sbi		portCtrl, pinWR		;WR high
	sbi		portCtrl, pinCS		;CS high

	ret

;INPUT: r16 index low, r17 index high, r18 data low, r19 data high
lcd_wr_reg:
	rcall	lcd_wr_reg16
	mov		r16, r18
	mov		r17, r19
	rcall	lcd_wr_data16
	ret

/*
;INPUT r16 low byte, r17 high byte
lcd_write_cmd:
	cbi		portCtrl, pinRS		;RS low

	rcall	lcd_write_bus

	ret

;INPUT r16 low byte, r17 high byte
lcd_write_data:
	sbi		portCtrl, pinRS		;RS high

	rcall	lcd_write_bus

	ret
*/

;INPUT r18 x1 low byte, r19 x1 high byte, r20 y1 low byte, r21 y1 high byte, r22 x2 low byte, r23 x2 high byte, r24 y2 low byte, r25 y2 high byte
//.include	"routine_address_set.asm"

;INPUT: Z color
lcd_fill:
	WRITE_REG 0x0020, 0x0000
	WRITE_REG 0x0021, 0x0000

	//WRITE_REG 0x0050, 0x0000
	//WRITE_REG 0x0051, 239
	//WRITE_REG 0x0052, 0x0000
	//WRITE_REG 0x0053, 319
	ldi		r16, $22
	ldi		r17, $00
	rcall	lcd_wr_reg16

	sbi		portCtrl, pinRD	;RD high
	sbi		portCtrl, pinRS	;RS high
	cbi		portCtrl, pinCS	;CS low

	ldi		r26, 160		;white-out first half
lcd_fill0:
	ldi		r27, 240
lcd_fill1:
	cbi		portCtrl, pinWR
	out		busLow, ZL
	out		busHigh, ZH
	sbi		portCtrl, pinWR

	cbi		portCtrl, pinWR
	out		busLow, ZL
	out		busHigh, ZH
	sbi		portCtrl, pinWR

	//rcall	delay

	dec		r27
	brne	lcd_fill1
	dec		r26
	brne	lcd_fill0

	ret

;INPUT: ZL ascii code
lcd_put_char:
	subi	ZL, 32				;calculate address of character
	clr		ZH					;multiply by 6
	mov		r16, YL
	mov		r17, YH
	movw	Y, Z
	add		ZL, YL
	adc		ZH, YH
	add		ZL, YL
	adc		ZH, YH
	add		ZL, YL
	adc		ZH, YH
	add		ZL, YL
	adc		ZH, YH
	add		ZL, YL
	adc		ZH, YH
	mov		YL, r16
	mov		YH, r17

	mov		r16, ZL				;add to offset
	mov		r17, ZH
	ldi		ZL, low(font_basic<<1)
	ldi		ZH, high(font_basic<<1)
	add		ZL, r16
	ldi		r16, 0
	adc		ZH, r16
	add		ZH, r17

lcd_draw_char:
	sbi		portCtrl, pinRD	;RD high
	sbi		portCtrl, pinRS	;RS high
	cbi		portCtrl, pinCS	;CS low

	ldi		r20, 36
lcd_draw_char0:
	lpm		r22, Z+			;load next sprite byte

	ldi		r21, 8
lcd_draw_char1:
	sbrs	r22, 0			;skip to white if 1
	rjmp	lcd_draw_char2

	ldi		r16, $22		;black
	ldi		r17, $00
	ldi		r18, $00
	ldi		r19, $00
	rcall	lcd_wr_reg

	jmp		lcd_draw_char3

lcd_draw_char2:
	ldi		r16, $22		;white
	ldi		r17, $00
	ldi		r18, $FF
	ldi		r19, $FF
	rcall	lcd_wr_reg

lcd_draw_char3:
	lsr		r22				;get next bit

	dec		r21
	brne	lcd_draw_char1

	ldi		r16, $21
	ldi		r17, $00
	mov		r18, r20
	ldi		r19, $00
	rcall	lcd_wr_reg

	dec		r20
	brne	lcd_draw_char0

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

	ldi		ZL, 'A'
	rcall	lcd_put_char

	//ldi		ZL, $00
	//ldi		ZH, $00
	//rcall	lcd_fill
	
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
