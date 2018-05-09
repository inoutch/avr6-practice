#include "serial.h"
#include "lib.h"
#include "time.h"
#include "xbee.h"

int main(void) {
    init();

    while (1) {
        XBeeResponse response = x_receive();

        uint8_t payload[] = "begin";
//        XBeeAddress64 addr = x_create_addr(0, 0);
        XBeeAddress64 addr = x_create_addr(0x0013A200, 0x40D8D2AF);
//        XBeeRequest request = x_create_request(&addr, payload, sizeof(payload));
        XBeeRequest request = x_create_request(&addr, response.frameDataPtr, response.frameLength);
        x_send(&request);
        delayMs(20000);
    }
}
