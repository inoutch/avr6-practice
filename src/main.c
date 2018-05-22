#include "serial.h"
#include "lib.h"
#include "time.h"
#include "thread.h"
#include "xbee.h"
#include "interrupt.h"

int global_time = 0;

void send_hello() {
    uint8_t payload[] = "begin";
    XBeeAddress64 addr = x_create_addr(0, 0);
    XBeeRequest request = x_create_request(&addr, payload, sizeof(payload));
    x_send(&request);
    while (1);
}

void echo() {
    while (1) {
        XBeeResponse response = x_receive();
        if (response.apiId != 0x90) {
            continue;
        }
        XBeeAddress64 addr = x_create_addr(0x0013A200, 0x41531E82);// digi
        //XBeeAddress64 addr = x_create_addr(0x0013A200, 0x40D8D2AF);
        XBeeRequest request = x_create_request(&addr, response.frameDataPtr, response.frameLength);
        x_send(&request);
    }
}

void xbee_test() {
    echo();
    while (1) {
        XBeeResponse response = x_receive();
        if (response.apiId != 0x90) {
            continue;
        }

        uint8_t payload[] = "begin";
        //XBeeAddress64 addr = x_create_addr(0x0013A200, 0x40D8D2AF);
        XBeeAddress64 addr = x_create_addr(0x0013A200, 0x41531E82);
        XBeeRequest request = x_create_request(&addr, payload, sizeof(payload));
        //XBeeRequest request = x_create_request(&addr, response.frameDataPtr, response.frameLength);
        x_send(&request);
    }
}

void execution(int argc, char *argv[]) {

}

void intr(softvec_type_t type, uint16_t sp) {
//    if (global_time == 20000)
//        global_time = 0;
//    else
//        global_time++;
}

int main(void) {
    INTR_DISABLE;
    // initialize
    init();
    softvec_init();

    timer_init();
    timer_interrupt_init();

    softvec_setintr(SOFTVEC_TYPE_TIMEINT, intr);
    INTR_ENABLE;

//    thread_ptr thread;
//    thread_create(&thread, execution, 0, NULL);
//    puti(123);
    while (1) {
        puthex((global_time & 0xff00) >> 8);
        puthex((global_time & 0x00ff) >> 0);
        putc('\n');
        delay_ms(10000);
    }
}
