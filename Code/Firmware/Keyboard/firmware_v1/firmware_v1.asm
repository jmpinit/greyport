.include "m168def.inc"

;***** Pin definitions

.equ	pinRX	= 0			;RX pin is PD0
.equ	pinTX	= 1			;RX pin is PD1

.equ	portRow = PORTD
.equ	pinRow0	= 2			;Row 0 pin is PD2
.equ	pinRow1	= 3			;Row 1 pin is PD3
.equ	pinRow2	= 4			;Row 2 pin is PD4
.equ	pinRow3	= 5			;Row 3 pin is PD5

.equ	pinCol0 = 6			;Col 0 pin is PD6
.equ	pinCol1 = 7			;Col 1 pin is PD7
.equ	pinCol2 = 0			;Col 2 pin is PB0
.equ	pinCol3 = 1			;Col 3 pin is PB1
.equ	pinCol4 = 2			;Col 4 pin is PB2
.equ	pinCol5 = 6			;Col 5 pin is PB6
.equ	pinCol6 = 7			;Col 6 pin is PB7
.equ	pinCol7 = 0			;Col 7 pin is PC0
.equ	pinCol8 = 1			;Col 8 pin is PC1
.equ	pinCol9 = 2			;Col 9 pin is PC2
.equ	pinCol10 = 3		;Col 10 pin is PC3
.equ	pinCol11 = 4		;Col 11 pin is PC4
.equ	pinCol12 = 5		;Col 12 pin is PC5

.cseg
.org 0
	rjmp	reset
.org 0x0024		
	rjmp	hRXdone	;UART receive done

;***************************************************************************
;* RX Done Handler
;***************************************************************************

hRXDone:
	//entering interrupt
	cli

	push r0
	in	r0, SREG
	push r0

	//exiting interrupt
	pop r0
	out SREG, r0
	pop r0

	sei

	reti

;***************************************************************************
;* "UART Send"
;* INPUT: r17 is byte to send
;* DESTROYS: r16
;***************************************************************************

uartSend:
	//wait until tx buffer empty
	lds r16, UCSR0A
	sbrs r16, UDRE0
	rjmp uartSend
	
	//put data into tx buffer
	sts UDR0, r17

	ret

;***************************************************************************
;* "Read Rows"
;* DESTROYS: r16, r17, r19
;***************************************************************************

readColumns:
	//first column
	sbis	PIND, pinCol0
	ldi		r17, '1'
	sbic	PIND, pinCol0
	ldi		r17, '0'

	rcall	uartSend

	//second column
	sbis	PIND, pinCol1
	ldi		r17, '1'
	sbic	PIND, pinCol1
	ldi		r17, '0'

	rcall	uartSend

	//third column
	sbis	PINB, pinCol2
	ldi		r17, '1'
	sbic	PINB, pinCol2
	ldi		r17, '0'

	rcall	uartSend

	//fourth column
	sbis	PINB, pinCol3
	ldi		r17, '1'
	sbic	PINB, pinCol3
	ldi		r17, '0'
	
	rcall	uartSend

	//fifth column
	sbis	PINB, pinCol4
	ldi		r17, '1'
	sbic	PINB, pinCol4
	ldi		r17, '0'
	
	rcall	uartSend

	//sixth column
	sbis	PINB, pinCol5
	ldi		r17, '1'
	sbic	PINB, pinCol5
	ldi		r17, '0'
	
	rcall	uartSend

	//seventh column
	sbis	PINB, pinCol6
	ldi		r17, '1'
	sbic	PINB, pinCol6
	ldi		r17, '0'
	
	rcall	uartSend

	//eighth column
	sbis	PINC, pinCol7
	ldi		r17, '1'
	sbic	PINC, pinCol7
	ldi		r17, '0'
	
	rcall	uartSend

	//ninth column
	sbis	PINC, pinCol8
	ldi		r17, '1'
	sbic	PINC, pinCol8
	ldi		r17, '0'
	
	rcall	uartSend

	//tenth column
	sbis	PINC, pinCol9
	ldi		r17, '1'
	sbic	PINC, pinCol9
	ldi		r17, '0'
	
	rcall	uartSend

	//eleventh column
	sbis	PINC, pinCol10
	ldi		r17, '1'
	sbic	PINC, pinCol10
	ldi		r17, '0'
	
	rcall	uartSend

	//twelfth column
	sbis	PINC, pinCol11
	ldi		r17, '1'
	sbic	PINC, pinCol11
	ldi		r17, '0'
	
	rcall	uartSend

	//thirteenth column
	sbis	PINC, pinCol12
	ldi		r17, '1'
	sbic	PINC, pinCol12
	ldi		r17, '0'
	
	rcall	uartSend
	
	ret

;***************************************************************************
;* "Delay"
;* DESTROYS: r16, r17, r18
;***************************************************************************

delay:
	ldi		r16, 0
	ldi		r17, 0
	ldi		r18, 25
delay0:
	dec		r16
	brne	delay0
delay1:
	dec		r16
	dec		r17
	brne	delay0
delay2:
	dec		r16
	dec		r17
	dec		r18
	brne	delay0

	ret
;***** Program Execution Starts Here

reset:
	//entering initialization
	cli												;disable interrupts

	//UART initialization
	ldi r16, 51									;9600 baud
	sts UBRR0L, r16
	ldi r16, 0
	sts UBRR0H, r16

	ldi r16, (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0)	;enable receiver and transmitter
	sts UCSR0B, r16
	ldi r16, (1<<USBS0)|(3<<UCSZ00)				;8 data bits, 2 stop
	sts UCSR0C, r16

	//port direction initialization
	ldi	r16, (1<<pinRow0)|(1<<pinRow1)|(1<<pinRow2)|(1<<pinRow3)
	out	DDRD, r16

	//port state initialization
	ldi r16, (1<<pinRow0)|(1<<pinRow1)|(1<<pinRow2)|(1<<pinRow3)
	out	portRow, r16

	//exiting initialization
	sei					;enable interrupts

forever:
	/*
	ldi		r17, 'H'
	rcall	uartSend

	ldi		r17, 'E'
	rcall	uartSend

	ldi		r17, 'L'
	rcall	uartSend

	ldi		r17, 'L'
	rcall	uartSend

	ldi		r17, 'O'
	rcall	uartSend

	ldi		r17, 13
	rcall	uartSend

	rjmp	forever
	*/

	ldi		r16, $FF
	out		portRow, r16

	//row 1
	cbi		portRow, pinRow0
	rcall	readColumns
	sbi		portRow, pinRow0

	//row 2
	cbi		portRow, pinRow1
	rcall	readColumns
	sbi		portRow, pinRow1

	//row 3
	cbi		portRow, pinRow2
	rcall	readColumns
	sbi		portRow, pinRow2

	//row 4
	cbi		portRow, pinRow3
	rcall	readColumns
	sbi		portRow, pinRow3

	//print line ending
	ldi		r17, 13
	rcall	uartSend

	//rcall	delay
	
	rjmp forever
