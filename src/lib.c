#include "serial.h"
#include "lib.h"

unsigned int index = 0;

void init(void) {
    //serial_setup();
    serial_init(index);
}

int putc(char c) {
    serial_send_byte(index, c);
    return 0;
}

int puts(char *str) {
    while (*str) {
        putc(*(str++));
    }
    return 0;
}

int puthex(char c) {
    char a16[] = "0123456789abcdef";
    char r = c & 0x0f;
    char l = (c >> 4) & 0x0f;
    putc(a16[l]);
    putc(a16[r]);
    putc(' ');
    return 0;
}

char getc() {
    return serial_recv_byte(index);
}

int send(char c[], unsigned int size) {
    unsigned int i = 0;
    for (i = 0; i < size; i++) {
        serial_send_byte(index, c[i]);
    }
    return 0;
}