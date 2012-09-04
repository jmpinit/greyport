.include "m1280def.inc"

util_vectors:
	rjmp	util_test			;0x00
	rjmp	util_serialInit		;0x01
	rjmp	util_serialSend		;0x02

util_test:
	rjmp	interpret
util_serialInit:
	ldi		r16, 0			;set baud rate at 9600
	sts		UBRR0H, r16
	ldi		r16, 103
	sts		UBRR0L, r16
	ldi		r16, (3<<UCSZ00);8 data bits
	sts		UCSR0C, r16
	ldi		r16, (1<<TXEN0)	;enable transmitter
	sts		UCSR0B, r16
	rjmp	interpret
util_serialSend:
	lds		r17, UCSR0A
	sbrs	r17, UDRE0
	rjmp	util_serialSend
	sts		UDR0, r16
	rjmp	interpret
