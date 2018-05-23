    .section .text

    .global intr_syscall
    .type   intr_syscall,@function
intr_syscall:
    cli
    push	r31
	push	r30
	push	r29
	push	r28
	push	r27
	push	r26
	push	r25
	push	r24
	push	r23
	push	r22
	push	r21
	push	r20
	push	r19
	push	r18
	push	r17
	push	r16
	push	r15
	push	r14
	push	r13
	push	r12
	push	r11
	push	r10
	push	r9
	push	r8
	push	r7
	push	r6
	push	r5
	push	r4
	push	r3
	push	r2
	push	r1
	push	r0; stack1: r31 ~ r0

	in      r20, 0x3b; RAMPZ
	in      r21, 0x3c; EIND
	push    r20
	push    r21; stack1: r31 ~ r0 | 0x3b | 0x3c

	in	    r31, 0x3f; SREG
	push	r31; stack1: r31 ~ r0 | 0x3b | 0x3c | 0x3f

	in	    r22, 0x3d; SP1
	in	    r23, 0x3e; SP2

	ldi	    r28, lo8(_intrstack)
	ldi	    r29, hi8(_intrstack)
	out	    0x3d, r28;
	out	    0x3e, r29; stack1 -> stack2

	push	r22; stack: 0x3d
	push	r23; stack: 0x3d | 0x3e

	eor	    r1, r1

	in	    r28, 0x3d	; 61
	in	    r29, 0x3e	; 62
	ldi	    r24, 1; SOFTVEC_TYPE_SOFTERR
    ldi	    r25, 0
	rcall	interrupt

	pop	    r23; stack: 0x3d > 0x3e
	pop	    r22; stack: > 0x3d
	out	    0x3d, r22; SP1
	out	    0x3e, r23; SP2 stack2 -> stack1

	pop	    r31; stack1: r31 ~ r0 | 0x3b | 0x3c > 0x3f
	out	    0x3f, r31; SREG

	pop     r21;stack1: r31 ~ r0 | 0x3b > 0x3c
	pop     r20;stack1: r31 ~ r0 > 0x3b
	out     0x3b, r20; RAMPZ
	out     0x3c, r21; ENID

	pop	r0
	pop	r1
	pop	r2
	pop	r3
	pop	r4
	pop	r5
	pop	r6
	pop	r7
	pop	r8
	pop	r9
	pop	r10
	pop	r11
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	r16
	pop	r17
	pop	r18
	pop	r19
	pop	r20
	pop	r21
	pop	r22
	pop	r23
	pop	r24
	pop	r25
	pop	r26
	pop	r27
	pop	r28
	pop	r29
	pop	r30
	pop	r31
	reti

	.global	intr_timeint
	.type	intr_timeint,@function
intr_timeint:
    cli
    push	r31
	push	r30
	push	r29
	push	r28
	push	r27
	push	r26
	push	r25
	push	r24
	push	r23
	push	r22
	push	r21
	push	r20
	push	r19
	push	r18
	push	r17
	push	r16
	push	r15
	push	r14
	push	r13
	push	r12
	push	r11
	push	r10
	push	r9
	push	r8
	push	r7
	push	r6
	push	r5
	push	r4
	push	r3
	push	r2
	push	r1
	push	r0; stack1: r31 ~ r0

	in      r20, 0x3b; RAMPZ
	in      r21, 0x3c; EIND
	push    r20
	push    r21; stack1: r31 ~ r0 | 0x3b | 0x3c

	in	    r31, 0x3f; SREG
	push	r31; stack1: r31 ~ r0 | 0x3b | 0x3c | 0x3f

	in	    r22, 0x3d; SP1
	in	    r23, 0x3e; SP2

	ldi	    r28, lo8(_intrstack)
	ldi	    r29, hi8(_intrstack)
	out	    0x3d, r28;
	out	    0x3e, r29; stack1 -> stack2

	push	r22; stack: 0x3d
	push	r23; stack: 0x3d | 0x3e

	eor	    r1, r1

	in	    r28, 0x3d	; 61
	in	    r29, 0x3e	; 62
	ldi	    r24, 3; SOFTVEC_TYPE_TIMEINT
    ldi	    r25, 0
	rcall	interrupt

	pop	    r23; stack: 0x3d > 0x3e
	pop	    r22; stack: > 0x3d
	out	    0x3d, r22; SP1
	out	    0x3e, r23; SP2 stack2 -> stack1

	pop	    r31; stack1: r31 ~ r0 | 0x3b | 0x3c > 0x3f
	out	    0x3f, r31; SREG

	pop     r21;stack1: r31 ~ r0 | 0x3b > 0x3c
	pop     r20;stack1: r31 ~ r0 > 0x3b
	out     0x3b, r20; RAMPZ
	out     0x3c, r21; ENID

	pop	r0
	pop	r1
	pop	r2
	pop	r3
	pop	r4
	pop	r5
	pop	r6
	pop	r7
	pop	r8
	pop	r9
	pop	r10
	pop	r11
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	r16
	pop	r17
	pop	r18
	pop	r19
	pop	r20
	pop	r21
	pop	r22
	pop	r23
	pop	r24
	pop	r25
	pop	r26
	pop	r27
	pop	r28
	pop	r29
	pop	r30
	pop	r31
	reti
