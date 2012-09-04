;INPUT: Z color
lcd_fill:
	WRITE_REG 0x0020, 0x0000
	WRITE_REG 0x0021, 0x0000
	
	ldi		r16, $22		;we want to write to the GRAM access register
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

	dec		r27
	brne	lcd_fill1
	dec		r26
	brne	lcd_fill0

	ret
