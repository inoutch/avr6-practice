#include "serial.h"

#define TXEN (1 << 3)
#define RXEN (1 << 4)
#define UCSZ0 (1 << 1)
#define UCSZ1 (1 << 2)
// UDR
#define TX (1 << 5)
#define RX (1 << 7)

static struct avr_sci {
    volatile char ucsr_a;
    volatile char ucsr_b;
    volatile char ucsr_c;
    volatile char ucsr_unknown0;
    volatile char ubrr_l;
    volatile char ubrr_h;
    volatile char udr;
};

struct {
    volatile struct avr_sci *sci;
} regs[3] = {{(volatile struct avr_sci *) 0xc0},
             {(volatile struct avr_sci *) 0xc8},
             {(volatile struct avr_sci *) 0xd0}};

void serial_init(unsigned int index) {
    volatile struct avr_sci *sci = regs[index].sci;
    sci->ubrr_l = 0b01100111;
    sci->ubrr_h = 0b00000000;

    //sci->ucsr_a = 0b00000000;
    sci->ucsr_b |= TXEN | RXEN;
    //sci->ucsr_c |= UCSZ0 | UCSZ1;
    sci->ucsr_c = 0b00000110;

    TCCR0A = 0b00000010;
    TCCR0B = 0b00000010;
}

int serial_is_send_enable(unsigned int index) {
    volatile struct avr_sci *sci = regs[index].sci;
    return (sci->ucsr_a & TX);
}

void serial_send_byte(unsigned int index, unsigned char c) {
    volatile struct avr_sci *sci = regs[index].sci;
    while (!serial_is_send_enable(index));
    sci->udr = c;
}

int serial_is_recv_enable(unsigned int index) {
    volatile struct avr_sci *sci = regs[index].sci;
    return (sci->ucsr_a & RX);
}

unsigned char serial_recv_byte(unsigned index) {
    volatile struct avr_sci *sci = regs[index].sci;
    while (!serial_is_recv_enable(index));
    return sci->udr;
}