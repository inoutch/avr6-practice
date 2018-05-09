#include "xbee.h"
#include "lib.h"

XBeeRequest x_create_request(XBeeAddress64 *addr, uint8_t *data, uint8_t dataLength) {
    XBeeRequest request = {
            TX_REQUEST,// api id
            DEFAULT_FRAME_ID,// frame id
            data, dataLength,
            *addr, BROADCAST_ADDRESS,
            BROADCAST_RADIUS_MAX_HOPS,
            TX_UNICAST
    };
    return request;
}

XBeeAddress64 x_create_addr(uint32_t msb, uint32_t lsb) {
    XBeeAddress64 addr = {msb, lsb};
    return addr;
}

uint8_t x_get_frame_data_length(XBeeRequest *request) {
    return TX_API_LENGTH + request->payloadLength;
}

void x_send(XBeeRequest *request) {
    // send start_byte
    x_send_byte(START_BYTE, 0);

    uint8_t frameDataLength = x_get_frame_data_length(request);
    // send length
    uint8_t msbLen = ((frameDataLength + 2) >> 8) & 0xff;
    uint8_t lsbLen = ((frameDataLength + 2) >> 0) & 0xff;

    x_send_byte(msbLen, 1);
    x_send_byte(lsbLen, 1);

    // api id
    x_send_byte(request->apiId, 1);
    x_send_byte(request->frameId, 1);

    uint8_t checksum = 0;

    // compute checksum, start at api id
    checksum += request->apiId;
    checksum += request->frameId;

    uint8_t i;
    for (i = 0; i < frameDataLength; i++) {
        x_send_byte(x_get_frame_data(request, i), 1);
        checksum += x_get_frame_data(request, i);
    }

    // perform 2s complement
    checksum = 0xff - checksum;

    // send checksum
    x_send_byte(checksum, 1);
}

void x_send_byte(uint8_t b, bool escape) {
    if (escape == TRUE && (b == START_BYTE || b == ESCAPE || b == XON || b == XOFF)) {
        x_write(ESCAPE);
        x_write(b ^ 0x20);
    } else {
        x_write(b);
    }
}

XBeeResponse x_receive() {
    uint8_t b = 0;
    uint8_t pos = 0;
    uint8_t checksumTotal = 0;
    int available = 1;
    int escape = 0;

    XBeeResponse response = {};

    while (available) {
        b = getc();
        if (pos > 0 && b == START_BYTE && ATAP == 2) {
            // new packet start before previous packeted completed -- discard previous packet and start over
            //_response.setErrorCode(UNEXPECTED_START_BYTE);
            response.errorCode = UNEXPECTED_START_BYTE;
            return response;
        }
        if (pos > 0 && b == ESCAPE) {
            if (available) {
                b = getc();
                b = 0x20 ^ b;
            } else {
                // escape byte.  next byte will be
                escape = 1;
                continue;
            }
        }

        if (escape) {
            b = 0x20 ^ b;
            escape = 0;
        }

        // checksum includes all bytes starting with api id
        if (pos >= API_ID_INDEX) {
            checksumTotal += b;
        }

        switch (pos) {
            case 0:
                if (b == START_BYTE) {
                    pos++;
                }
                break;
            case 1:
                // length msb
                response.msbLength = b;
                pos++;
                break;
            case 2:
                // length lsb
                response.lsbLength = b;
                pos++;
                break;
            case 3:
                response.apiId = b;
                pos++;
                break;
            default:
                // starts at fifth byte
                if (pos > MAX_FRAME_DATA_SIZE) {
                    // exceed max size.  should never occur
                    //_response.setErrorCode(PACKET_EXCEEDS_BYTE_ARRAY_LENGTH);
                    response.errorCode = PACKET_EXCEEDS_BYTE_ARRAY_LENGTH;
                    return response;
                }
                // check if we're at the end of the packet
                // packet length does not include start, length, or checksum bytes, so add 3
                if (pos == (x_get_packet_size(&response) + 3)) {
                    // verify checksum

                    if ((checksumTotal & 0xff) == 0xff) {
                        response.checksum = b;
                        available = 1;
                        //_response.setAvailable(true);
                        //_response.setErrorCode(NO_ERROR);
                    } else {
                        // checksum failed
                        //_response.setErrorCode(CHECKSUM_FAILURE);
                        response.errorCode = CHECKSUM_FAILURE;
                    }

                    // minus 4 because we start after start,msb,lsb,api and up to but not including checksum
                    // e.g. if frame was one byte, _pos=4 would be the byte, pos=5 is the checksum, where end stop reading
                    response.frameLength = pos - 4;

                    // reset state vars
                    pos = 0;

                    return response;
                } else {
                    // add to packet array, starting with the fourth byte of the apiFrame
                    response.frameDataPtr[pos - 4] = b;
                    pos++;
                }
        }
    }
}

uint16_t x_get_packet_size(XBeeResponse *response) {
    return ((response->msbLength << 8) & 0xff) + (response->lsbLength & 0xff);
}

void x_write(uint8_t val) {
    putc(val);
}

uint8_t x_get_frame_data(XBeeRequest *request, uint8_t pos) {
    switch (pos) {
        case 0:
            return (request->addr64.msb >> 24) & 0xff;
        case 1:
            return (request->addr64.msb >> 16) & 0xff;
        case 2:
            return (request->addr64.msb >> 8) & 0xff;
        case 3:
            return (request->addr64.msb >> 0) & 0xff;
        case 4:
            return (request->addr64.lsb >> 24) & 0xff;
        case 5:
            return (request->addr64.lsb >> 16) & 0xff;
        case 6:
            return (request->addr64.lsb >> 8) & 0xff;
        case 7:
            return (request->addr64.lsb >> 0) & 0xff;
        case 8:
            return (request->addr16 >> 8) & 0xff;
        case 9:
            return request->addr16 & 0xff;
        case 10:
            return request->broadcastRadius;
        case 11:
            return request->option;
        default:
            return request->payloadPtr[pos - TX_API_LENGTH];
    }
}
