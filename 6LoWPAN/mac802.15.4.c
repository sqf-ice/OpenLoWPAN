#include "mac802.15.4.h"

static inline void pack16(uint16_t v, uint8_t *p)
{
    p[0] = v & 0xFF;
    p[1] = (v >> 8) & 0xFF;
}

static inline void pack64(uint64_t v, uint8_t *p)
{
    p[0] = v & 0xFF;
    p[1] = (v >>  8) & 0xFF;
    p[2] = (v >> 16) & 0xFF;
    p[3] = (v >> 24) & 0xFF;
    p[4] = (v >> 32) & 0xFF;
    p[5] = (v >> 40) & 0xFF;
    p[6] = (v >> 48) & 0xFF;
    p[7] = (v >> 56) & 0xFF;
}

static inline uint16_t unpack16(uint8_t *p)
{
    return p[0] | (p[1] << 8);
}

static inline uint64_t unpack64(uint8_t *p)
{
    return (uint64_t)p[0] | ((uint64_t)p[1] << 8) |
           ((uint64_t)p[2] << 16) | ((uint64_t)p[3] << 24) |
           ((uint64_t)p[4] << 32) | ((uint64_t)p[5] << 40) |
           ((uint64_t)p[6] << 48) | ((uint64_t)p[7] << 56);
}

void mac802154PackHeader(const MAC802154FrameHeader *header, uint8_t *packed)
{
    uint8_t *p = packed;
    *p = header->frameSize; p++;
    pack16(header->frameControl, p); p += 2;
    *p = header->sequenceNumber; p++;

    if (header->dstAddressMode != MAC802154_ADDRESS_NOT_PRESENT)
    {
        pack16(header->dstAddress.panID, p); p += 2;
    }

    if (header->dstAddressMode == MAC802154_ADDRESS_SHORT)
    {
        pack16(header->dstAddress.shortAddress, p); p += 2;
    }
    else if (header->dstAddressMode == MAC802154_ADDRESS_EXTENDED)
    {
        pack64(header->dstAddress.extendedAddress, p); p += 8;
    }

    if ((header->srcAddressMode != MAC802154_ADDRESS_NOT_PRESENT) &&
            (!header->panIDCompressed || (header->dstAddressMode == MAC802154_ADDRESS_NOT_PRESENT)))
    {
        pack16(header->srcAddress.panID, p); p += 2;
    }
    if (header->srcAddressMode == MAC802154_ADDRESS_SHORT)
    {
        pack16(header->srcAddress.shortAddress, p); p += 2;
    }
    else if (header->srcAddressMode == MAC802154_ADDRESS_EXTENDED)
    {
        pack64(header->srcAddress.extendedAddress, p); p += 8;
    }
}

void mac802154UnpackHeader(const uint8_t *packed, MAC802154FrameHeader *header)
{
    uint8_t *p = (uint8_t*)packed;
    header->frameSize = *p; p++;
    header->frameControl = unpack16(p); p += 2;
    header->sequenceNumber = *p; p++;

    if (header->dstAddressMode != MAC802154_ADDRESS_NOT_PRESENT)
    {
        header->dstAddress.panID = unpack16(p); p += 2;
    }

    if (header->dstAddressMode == MAC802154_ADDRESS_SHORT)
    {
        header->dstAddress.shortAddress = unpack16(p); p += 2;
    }
    else if (header->dstAddressMode == MAC802154_ADDRESS_EXTENDED)
    {
        header->dstAddress.extendedAddress = unpack64(p); p += 8;
    }

    if ((header->srcAddressMode != MAC802154_ADDRESS_NOT_PRESENT) &&
            (!header->panIDCompressed || (header->dstAddressMode == MAC802154_ADDRESS_NOT_PRESENT)))
    {
        header->srcAddress.panID = unpack16(p); p += 2;
    }
    if (header->srcAddressMode == MAC802154_ADDRESS_SHORT)
    {
        header->srcAddress.shortAddress = unpack16(p); p += 2;
    }
    else if (header->srcAddressMode == MAC802154_ADDRESS_EXTENDED)
    {
        header->srcAddress.extendedAddress = unpack64(p); p += 8;
    }
}

uint8_t mac802154HeaderSize(const MAC802154FrameHeader *header)
{
    uint8_t size = 4;

    if (header->dstAddressMode != 0)
        size += 2;

    if (header->dstAddressMode == 2)
        size += 2;
    else if (header->dstAddressMode == 3)
        size += 8;

    if ((header->srcAddressMode != 0) &&
            (!header->panIDCompressed || (header->dstAddressMode == 0)))
        size += 2;

    if (header->srcAddressMode == 2)
        size += 2;
    else if (header->srcAddressMode == 3)
        size += 8;

    return size;
}
