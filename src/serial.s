;	.section .text
;	.global	serial_init
;	.type	serial_init,@function
;	.set EIND, 0x30 + 0x0c ;
;serial_init:
	;in	    r16,	0x35	; MCUCRレジスタ
	;mov	    r17,	r16
	;ori	    r16,	(1 << 0) ; IVCEビット
	;andi	r17,	0b11111101	; IVSELビットを0に．若くは0を代入．
	;out	    0x35, 	r17

	;ldi	    r16,	0b00000000
	;sts	    0xc1,	r16	; USART_UCSR0Bレジスタ
	;ldi	    r16,	0b01100111
	;sts	    0xc4,	r16	; USART_UBRR0Lレジスタ 0b01100111
	;ldi	    r16,	0b00000000
	;sts	    0xc5,	r16	; USART_UBRR0Hレジスタ

	;lds	    r16,	0xc1	; USART_UCSR0Bレジスタ
	;ori	    r16,	(1 << 3) ; TXEN0ビット
	;ori	    r16,	(1 << 4) ; RXEN0ビット
	;sts	    0xc1,	r16	; USART_UCSR0Bレジスタ

	;lds	    r16,	0xc2	; USART_UCSR0Cレジスタ
	;ori	    r16,	(1 << 1) ; UCSZ00ビット
	;ori	    r16,	(1 << 2) ; UCSZ01ビット
	;sts	    0xc2,	r16	; USART_UCSR0Cレジスタ

	;in	    r16,	0x0a	; DDRDレジスタ
	;andi	r16,	0b11111110
	;out	    0x0a,	r16
	;in	    r16,	0x0b	; PORTDレジスタ
	;ori	    r16,	0b00000001
	;out	    0x0b,	r16
	;ret


	;.globl	serial_is_send_enable
	;.type	serial_is_send_enable, @function
;serial_is_send_enable:
;	lds	    r17,	0xc0	; UCSR0Aレジスタ
;	andi	r17,	0b00100000    ;	UDRE0ビット
;	breq	serial_is_send_enable	;z = 1
;	ret			;z = 0


;	.globl	serial_send_byte
;	.type	serial_send_byte,@function
;serial_send_byte:
;	rcall	serial_is_send_enable
; 	sts	0xc6,	r24	; UDR0レジスタ, r24に第一引数が入っている
;	ret

	;.section .text
	;.global	serial_setup
	;.type	serial_setup,@function
;serial_setup:
	;in	        r16,	0x35	; MCUCRレジスタ
	;mov	    r17,	r16
	;ori	    r16,	(1 << 0) ; IVCEビット
	;andi	    r17,	0b11111101	; IVSELビットを0に．若くは0を代入．
	;out	    0x35, 	r17

	;ldi	    r16,	0b00000000
	;sts	    0xc1,	r16	; USART_UCSR0Bレジスタ
	;ldi	    r16,	0b01100111
	;sts	    0xc4,	r16	; USART_UBRR0Lレジスタ 0b01100111
	;ldi	    r16,	0b00000000
	;sts	    0xc5,	r16	; USART_UBRR0Hレジスタ

	;lds	    r16,	0xc1	; USART_UCSR0Bレジスタ
	;ori	    r16,	(1 << 3) ; TXEN0ビット
	;ori	    r16,	(1 << 4) ; RXEN0ビット
	;sts	    0xc1,	r16	; USART_UCSR0Bレジスタ

	;lds	    r16,	0xc2	; USART_UCSR0Cレジスタ
	;ori	    r16,	(1 << 1) ; UCSZ00ビット
	;ori	    r16,	(1 << 2) ; UCSZ01ビット
	;sts	    0xc2,	r16	; USART_UCSR0Cレジスタ

	;in	    r16,	0x0a	; DDRDレジスタ
	;andi	r16,	0b11111110
	;out	    0x0a,	r16
	;in	    r16,	0x0b	; PORTDレジスタ
	;ori	    r16,	0b00000001
	;out	    0x0b,	r16
	;ret

	.globl	get_register
	.type	get_register,@function
get_register:
	;; 	ldi	r22,	0x61
	;; 	ldi	r23, 	0x62
	;; 	movw	r24,	r22
	ldi	r24,	0x0
	ldi	r25,	0x0
	ldi	r24,	0x61
	ret
