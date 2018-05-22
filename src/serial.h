#ifndef _SERIAL_H_INCLUDED_
#define _SERIAL_H_INCLUDED_

#include "type.h"

// ref: https://github.com/vancegroup-mirrors/avr-libc/blob/master/avr-libc/include/avr/iomxx0_1.h
#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))
#define _SFR_MEM8(mem_addr) _MMIO_BYTE(mem_addr)
#define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + __SFR_OFFSET)
#define __SFR_OFFSET 0x20

#define MCUCR   _SFR_IO8(0X35)
#define DDRD    _SFR_IO8(0x0A)
#define PORTD   _SFR_IO8(0x0B)

#define EIND    _SFR_IO8(0x3C)

#define TCCR0A  _SFR_IO8(0x24)
#define TCCR0B  _SFR_IO8(0x25)
#define TIMSK0  _SFR_MEM8(0x6E)
#define TCNT0   _SFR_IO8(0X26)
#define OCR0A   _SFR_IO8(0X27)
#define OCR0B   _SFR_IO8(0X28)
#define TIFR0   _SFR_IO8(0x15)

#define TCCR1A  _SFR_MEM8(0x80)
#define TCCR1B  _SFR_MEM8(0x81)
#define ICR1    _SFR_MEM16(0x86)
#define ICR1L   _SFR_MEM8(0x86)
#define ICR1H   _SFR_MEM8(0x87)
#define OCR1A   _SFR_MEM16(0x88)
#define OCR1AL  _SFR_MEM8(0x88)
#define OCR1AH  _SFR_MEM8(0x89)
#define TIMSK1  _SFR_MEM8(0x6F)

void serial_setup();

void serial_init(unsigned int index);

int serial_is_send_enable(unsigned int index);

void serial_send_byte(unsigned int index, unsigned char c);

int serial_is_recv_enable(unsigned int index);

unsigned char serial_recv_byte(unsigned index);

char get_register(void);

#endif
