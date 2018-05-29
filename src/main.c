#include "serial.h"
#include "xbee.h"
#include "os.h"
#include "lib.h"
#include "time.h"


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

void execution1(int args, char *argv[]) {
    puts("1111111a");
}

void execution2(int args, char *argv[]) {
    puts("2222222b");
}

void execution3(int args, char *argv[]) {
    puts("33333333c");
}

int main(void) {
    os_init();

    thread_run(execution1, 128, 0, NULL);
    thread_run(execution2, 128, 0, NULL);
    thread_run(execution3, 128, 0, NULL);
    puts("4444444d");

    while (1) {
        delay_ms(1000);
    }
    // exit(0);
}