#ifndef MAC_802_14_5_H
#define MAC_802_14_5_H 

#include <stdint.h>
#include <stdbool.h>

// FIXME: Security headers not included
#define MAC802145_MAX_HEADER_SIZE   24
#define MAC802145_MAX_FRAME_SIZE    127

#define MAC802145_FRAME_TYPE_BEACON     0
#define MAC802145_FRAME_TYPE_DATA       1
#define MAC802145_FRAME_TYPE_ACK        2
#define MAC802145_FRAME_TYPE_MAC_CMD    3

#define MAC802145_FRAME_VERSION         1

#define MAC802145_ADDRESS_NOT_PRESENT   0
#define MAC802145_ADDRESS_SHORT         2
#define MAC802145_ADDRESS_EXTENDED      3

typedef struct
{
    uint16_t panID;
    union 
    {
        uint16_t shortAddress;
        uint64_t extendedAddress;
    };
} MAC802145Address;

typedef struct
{
    uint8_t frameSize;
    union {
        uint16_t frameControl;
        struct
        {
            uint8_t frameType       : 3;
            bool    securityEnabled : 1;
            bool    framePending    : 1;
            bool    ackRequest      : 1;
            bool    panIDCompressed : 1;
            uint8_t _unused07       : 1;
            uint8_t _unused0709     : 2;
            uint8_t dstAddressMode  : 2;
            uint8_t frameVersion    : 2;
            uint8_t srcAddressMode  : 2;
        };
    };
    uint8_t sequenceNumber;
    MAC802145Address srcAddress;
    MAC802145Address dstAddress;
} MAC802145FrameHeader;

void mac802145PackHeader(const MAC802145FrameHeader *header, uint8_t *packed);
void mac802145UnpackHeader(const uint8_t *packed, MAC802145FrameHeader *header);

uint8_t mac802145HeaderSize(const MAC802145FrameHeader *header);

#endif
