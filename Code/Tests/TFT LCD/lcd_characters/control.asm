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

	sbi		portCtrl, pinCS		;CS high

	ret

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
