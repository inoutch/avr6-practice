	.section .text
	.global	start
	.type	start,@function
start:
	ldi	r28, lo8(_bootstack)
	ldi	r29, hi8(_bootstack)
	out	0x3d, r28
	out	0x3e, r29

data_copy_set:
	eor	r1, r1

	/* length */
	ldi	r18, lo8(_rodata_start)
	ldi	r19, hi8(_rodata_start)
	ldi	r20, lo8(_edata)
	ldi	r21, hi8(_edata)
	sub	r20, r18
	sbc	r21, r19

	/* src */
	ldi	r30, lo8(_etext)
	ldi	r31, hi8(_etext)

	/* dst */
	ldi	r26, lo8(_rodata_start)
	ldi	r27, hi8(_rodata_start)

data_copy:
	cp	r1, r20
	cpc	r1, r21
	brge	bss_clear_set
	lpm	r24, Z+
	;;  #ifdef SIMULATOR
	;;  	ld	r24, X
	;; 	#endif
	st	X+, r24
	subi	r20, 0x01
	sbci	r21, 0x00
	rjmp	data_copy

bss_clear_set:
	eor	r24, r24

	/* length */
	ldi	r18, lo8(_bss_start)
	ldi	r19, hi8(_bss_start)
	ldi	r20, lo8(_ebss)
	ldi	r21, hi8(_ebss)
	sub	r20, r18
	sbc	r21, r19

	/* dst */
	ldi	r26, lo8(_bss_start)
	ldi	r27, hi8(_bss_start)

bss_clear:
	cp	    r1, r20
	cpc	    r1, r21
	brge	call_main
	st	    X+, r24
	subi	r20, 0x01
	sbci	r21, 0x00
	rjmp	bss_clear

call_main:
    out     0x3C, 0x00  ; EIND を初期化
	call	main
