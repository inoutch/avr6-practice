#include "serial.h"

void timer_init() {
//    タイマ0,CTC,割り込み用、比較A一致で割り込み
    TCCR0A = 0b00000010;
    TCCR0B = 0b00000011; // N=64
    OCR0A = 78;  // 5msごとに割り込み
//    TIMSK0 = 0b0000010;    //比較A一致割り込み有効
}

void timer_interrupt_init() {
    TCCR1A = 0b00000010;
    TCCR1B = 0b00011001;
    ICR1L = 0b11111111;
    ICR1H = 0b11111001;
    OCR1AL = 0b11111111;
    OCR1AH = 0b01111100;
    TIMSK1 = 0b00100000;
}

void delay_one_ms(void) {
    TCNT0 = 0; // TCNTの値を0クリア(保険)
    OCR0A = 124; //[8分周,1ms] OCR0A = 1000000 / ( 8 * 1000 ) - 1 = 124;
    TCCR0B = (TCCR0B & 0b11111000) | 0b00000010; // 分周設定&タイマ開始
    while (!((TIFR0 >> 1) & 1)); // OCF0Aが1になるまでwhileで待機
    TIFR0 = 0b00000010; // OCF0Aに１を書き込む(クリア)
    TCCR0B = (TCCR0B & 0b11111000); // CS[2-0]を0にしてタイマ停止
    return;
}

void delay_ms(unsigned long msTime) {
    unsigned long i;
    for (i = 0; i < msTime; ++i) {
        delay_one_ms();
    }
    return;
}