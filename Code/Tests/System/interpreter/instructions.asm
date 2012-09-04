instr_vectors:
	rjmp	instr_ADD		;0x00	
	rjmp	instr_ADC		;0x01	
	rjmp	instr_SUB		;0x02	
	rjmp	instr_AND		;0x03	
	rjmp	instr_OR		;0x04	
	rjmp	instr_EOR		;0x05	
	rjmp	instr_SBR		;0x06	
	rjmp	instr_CBR		;0x07	
	rjmp	instr_INC		;0x08	<3
	rjmp	instr_DEC		;0x09	<4
	rjmp	instr_TST		;0x0A	
	rjmp	instr_CLR		;0x0B	
	rjmp	instr_SER		;0x0C	
	rjmp	instr_RJMPF		;0x0D	<2
	rjmp	instr_RJMPB		;0x0E
	rjmp	instr_JMP		;0x0F	
	rjmp	instr_RCALL		;0x10	
	rjmp	instr_CALL		;0x11	
	rjmp	instr_RET		;0x12	
	rjmp	instr_CPSE		;0x13	
	rjmp	instr_CP		;0x14	
	rjmp	instr_SBRC		;0x15	
	rjmp	instr_SBRS		;0x16	
	rjmp	instr_BREQ		;0x17	
	rjmp	instr_BRNE		;0x18	
	rjmp	instr_BRCS		;0x19	
	rjmp	instr_BRCC		;0x1A	
	rjmp	instr_BRVS		;0x1B	
	rjmp	instr_BRVC		;0x1C	
	rjmp	instr_LSL		;0x1D	
	rjmp	instr_LSR		;0x1E	
	rjmp	instr_ROL		;0x1F	
	rjmp	instr_ROR		;0x20	
	rjmp	instr_SEC		;0x21	
	rjmp	instr_CLC		;0x22	
	rjmp	instr_SEN		;0x23	
	rjmp	instr_CLN		;0x24	
	rjmp	instr_SEZ		;0x25	
	rjmp	instr_CLZ		;0x26	
	rjmp	instr_MOV		;0x27	<5
	rjmp	instr_LDI		;0x28	<1
	rjmp	instr_LD		;0x29	
	rjmp	instr_ST		;0x2A	
	rjmp	instr_NOP		;0x2B	X
	rjmp	instr_Exit		;0x2C	
	rjmp	instr_OSCall	;0x2D	X

//ARITHMETIC AND LOGIC

instr_ADD:
	;Params=	none
	rjmp	interpret
instr_ADC:
	;Params=	none
	rjmp	interpret
instr_SUB:
	;Params=	none
	rjmp	interpret
instr_AND:
	;Params=	none
	rjmp	interpret
instr_OR:
	;Params=	none
	rjmp	interpret
instr_EOR:
	;Params=	none
	rjmp	interpret
instr_SBR:
	;Params=	none
	rjmp	interpret
instr_CBR:
	;Params=	none
	rjmp	interpret
instr_INC:
	;Params=	none
	rjmp	interpret
instr_DEC:
	;Params=	none
	rjmp	interpret
instr_TST:
	;Params=	none
	rjmp	interpret
instr_CLR:
	;Params=	none
	rjmp	interpret
instr_SER:
	;Params=	none
	rjmp	interpret

//BRANCH

instr_RJMPF:
	;Params=	signed relative address
	ld		r1, Y+
	add		YL, r1		;add the relative address to the interpreter's PC
	brcc	PC+2
	inc		YH
	rjmp	interpret
instr_RJMPB:
	;Params=	signed relative address
	ld		r1, Y+
	sub		YL, r1		;add the relative address to the interpreter's PC
	brcc	PC+2
	dec		YH
	rjmp	interpret
instr_JMP:
	;Params=	none
	rjmp	interpret
instr_RCALL:
	;Params=	none
	rjmp	interpret
instr_CALL:
	;Params=	none
	rjmp	interpret
instr_RET:
	;Params=	none
	rjmp	interpret
instr_CPSE:
	;Params=	none
	rjmp	interpret
instr_CP:
	;Params=	none
	rjmp	interpret
instr_SBRC:
	;Params=	none
	rjmp	interpret
instr_SBRS:
	;Params=	none
	rjmp	interpret
instr_BREQ:
	;Params=	none
	rjmp	interpret
instr_BRNE:
	;Params=	none
	rjmp	interpret
instr_BRCS:
	;Params=	none
	rjmp	interpret
instr_BRCC:
	;Params=	none
	rjmp	interpret
instr_BRVS:
	;Params=	none
	rjmp	interpret
instr_BRVC:
	;Params=	none
	rjmp	interpret

//BIT OPERATIONS

instr_LSL:
	;Params=	none
	rjmp	interpret
instr_LSR:
	;Params=	none
	rjmp	interpret
instr_ROL:
	;Params=	none
	rjmp	interpret
instr_ROR:
	;Params=	none
	rjmp	interpret
instr_SEC:
	;Params=	none
	rjmp	interpret
instr_CLC:
	;Params=	none
	rjmp	interpret
instr_SEN:
	;Params=	none
	rjmp	interpret
instr_CLN:
	;Params=	none
	rjmp	interpret
instr_SEZ:
	;Params=	none
	rjmp	interpret
instr_CLZ:
	;Params=	none
	rjmp	interpret

//DATA TRANSFER

instr_MOV:
	;Params=	none
	rjmp	interpret
instr_LDI:
	;Params=	register to load into, byte to load into register
	ld		ZL, Y+
	ldi		ZH, 0
	ld		r1, Y+
	st		Z, r1
	rjmp	interpret
instr_LD:
	;Params=	none
	rjmp	interpret
instr_ST:
	;Params=	none
	rjmp	interpret

//MISCELLANEOUS

instr_NOP:
	;Params=	none
	rjmp	interpret

//SPECIAL

instr_Exit:
	;Params=	none
	rjmp	interpret
instr_OSCall:
	;Params=	func vec addr high, func vec addr low
	ld		r1, Y+		;store high byte of address
	ld		r2, Y+		;store low byte of address
	ldi		ZH, high(util_vectors)
	ldi		ZL, low(util_vectors)
	add		ZL, r2		;add low byte
	adc		ZH, r1		;add high byte
	ijmp
.include "utilities.asm"
