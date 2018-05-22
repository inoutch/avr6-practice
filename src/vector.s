	.section .text
	.global	start
	.type	start,@function
reset:		jmp	start
int0:		jmp	nointr
int1:		jmp	nointr
int2:       jmp nointr
int3:       jmp nointr
int4:       jmp nointr
int5:       jmp nointr
int6:       jmp nointr
int7:       jmp nointr
pcint0:		jmp	nointr
pcint1:		jmp	nointr
pcint2:		jmp	nointr
wdt:		jmp	nointr
timer2_compa:	jmp	nointr
timer2_compb:	jmp	nointr
timer2_ovf:	jmp	nointr
timer1_capt:	jmp	intr_timeint
timer1_compa:	jmp nointr
timer1_compb:	jmp	nointr
timer1_compc:	jmp	nointr
timer1_ovf:	jmp	nointr
timer0_compa:	jmp	nointr
timer0_compb:	jmp	nointr
timer0_ovf:	jmp	nointr
spi_stc:	jmp	nointr
usart_rx:	jmp	nointr
usart_udre:	jmp	nointr
usart_tx:	jmp	nointr
adc:		jmp	nointr
ee_rdy:		jmp	nointr
analog_comp:	jmp	nointr
twi:		jmp	nointr
spm_ready:	jmp	nointr

nointr:		jmp	nointr
