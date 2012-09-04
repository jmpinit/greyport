.include "m1280def.inc"

.def	statReg			= r0
.def	instruction		= r1

.cseg
.org 0

rjmp reset

reset:
	//set up pointers for loading program
	ldi		ZH, high(program<<1)
	ldi		ZL, low(program<<1)
	ldi		YH, high(SRAM_START)
	ldi		YL, low(SRAM_START)
	ldi		r16, 36					;number of bytes of program to load
loadProgram:
	lpm		r1, Z+
	st		Y+, r1
	dec		r16
	brne	loadProgram
initialize:
	ldi		YH, high(SRAM_START)
	ldi		YL, low(SRAM_START)
interpret:
	ld		instruction, Y+
	ldi		ZH, high(instr_vectors)
	ldi		ZL, low(instr_vectors)
	add		ZL, instruction
	ijmp
	.include "instructions.asm"

//TEST PROGRAM
program:
	.dw	$002D
	.dw	$2801
	.dw	$7C10
	.dw	$002D
	.dw $2802
	.dw	$7201
	.dw	$002D
	.dw $2802
	.dw	$4810
	.dw	$002D
	.dw	$2802
	.dw	$4510
	.dw	$002D
	.dw	$2802
	.dw	$4C10
	.dw	$002D
	.dw	$2802
	.dw	$4C10
	.dw	$002D
	.dw	$2802
	.dw $4F10
	.dw	$002D
	.dw	$0E02
	.dw	$0030
