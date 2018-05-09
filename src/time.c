#include "serial.h"

void delayOneMs(void) {
    TCNT0 = 0; // TCNTの値を0クリア(保険)
    OCR0A = 124; //[8分周,1ms] OCR0A = 1000000 / ( 8 * 1000 ) - 1 = 124;
    TCCR0B = (TCCR0B & 0b11111000) | 0b00000010; // 分周設定&タイマ開始
    while (!((TIFR0 >> 1) & 1)); // OCF0Aが1になるまでwhileで待機
    TIFR0 = 0b00000010; // OCF0Aに１を書き込む(クリア)
    TCCR0B = (TCCR0B & 0b11111000); // CS[2-0]を0にしてタイマ停止
    return;
}

void delayMs(unsigned long msTime) {
    unsigned long i;
    for (i = 0; i < msTime; ++i) {
        delayOneMs();
    }
    return;
}