#include "serial.h"
#include "xbee.h"
#include "os.h"
#include "lib.h"

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

void execution(int args, char *argv[]) {
}

int main(void) {
    os_init();

    char *str = malloc(sizeof(char) * 20);
    strcpy(str, "hogehoge\n");
    puts(str);
    free(str);

    thread_run(execution, 0, 64, 0, NULL);

    puts("done.\n");
    while (1);
    // exit(0);
}
