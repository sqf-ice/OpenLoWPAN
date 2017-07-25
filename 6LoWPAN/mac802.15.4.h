#ifndef MAC_802_15_4_H
#define MAC_802_15_4_H 

#include <stdint.h>
#include <stdbool.h>

// FIXME: Security headers not included
#define MAC802154_MAX_HEADER_SIZE   24
#define MAC802154_MAX_FRAME_SIZE    127

#define MAC802154_FRAME_TYPE_BEACON     0
#define MAC802154_FRAME_TYPE_DATA       1
#define MAC802154_FRAME_TYPE_ACK        2
#define MAC802154_FRAME_TYPE_MAC_CMD    3

#define MAC802154_FRAME_VERSION         1

#define MAC802154_ADDRESS_NOT_PRESENT   0
#define MAC802154_ADDRESS_SHORT         2
#define MAC802154_ADDRESS_EXTENDED      3

typedef struct
{
    uint16_t panID;
    union 
    {
        uint16_t shortAddress;
        uint64_t extendedAddress;
    };
} MAC802154Address;

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
    MAC802154Address srcAddress;
    MAC802154Address dstAddress;
} MAC802154FrameHeader;

void mac802154PackHeader(const MAC802154FrameHeader *header, uint8_t *packed);
void mac802154UnpackHeader(const uint8_t *packed, MAC802154FrameHeader *header);

uint8_t mac802154HeaderSize(const MAC802154FrameHeader *header);

#endif
