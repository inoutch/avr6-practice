#ifndef _XBEE_H_INCLUDED_
#define _XBEE_H_INCLUDED_

#include "type.h"

typedef struct {
    uint8_t frameDataPtr[256];
    uint8_t apiId;
    uint8_t msbLength;
    uint8_t lsbLength;
    uint8_t checksum;
    uint8_t frameLength;
    bool complete;
    uint8_t errorCode;
} XBeeResponse;

typedef struct {
    uint32_t msb;
    uint32_t lsb;
} XBeeAddress64;

typedef struct {
    uint8_t apiId;
    uint8_t frameId;
    uint8_t *payloadPtr;
    uint8_t payloadLength;
    XBeeAddress64 addr64;
    uint16_t addr16;
    uint8_t broadcastRadius;
    uint8_t option;
} XBeeRequest;

#define MAX_FRAME_DATA_SIZE 110
#define TX_REQUEST 0x10
#define DEFAULT_FRAME_ID 1
#define BROADCAST_ADDRESS 0xfffe
#define BROADCAST_RADIUS_MAX_HOPS 0
#define TX_UNICAST 0
#define TX_API_LENGTH 12

#define START_BYTE 0x7e
#define ESCAPE 0x7d
#define XON 0x11
#define XOFF 0x13

#define API_ID_INDEX 3
#define ATAP 2

enum {
    NO_ERROR = 0,
    UNEXPECTED_START_BYTE,
    PACKET_EXCEEDS_BYTE_ARRAY_LENGTH,
    CHECKSUM_FAILURE,
};

XBeeRequest x_create_request(XBeeAddress64 *addr, uint8_t *data, uint8_t dataLength);

XBeeAddress64 x_create_addr(uint32_t msb, uint32_t lsb);

uint8_t x_get_frame_data_length(XBeeRequest *request);

void x_send(XBeeRequest *request);

void x_send_byte(uint8_t b, bool escape);

XBeeResponse x_receive();

uint16_t x_get_packet_size(XBeeResponse *response);

void x_write(uint8_t val);

uint8_t x_get_frame_data(XBeeRequest *request, uint8_t pos);

#endif