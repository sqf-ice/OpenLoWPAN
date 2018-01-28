#ifndef MAC_802_15_4_H
#define MAC_802_15_4_H 

#include <stdint.h>
#include <stdbool.h>

// FIXME: Security headers not included
#define MAC802154_MAX_HEADER_SIZE       24
#define MAC802154_MAX_FRAME_SIZE        127

#define MAC802154_FRAME_VERSION         1

typedef enum {MAC802154_FRAME_TYPE_BEACON = 0, MAC802154_FRAME_TYPE_DATA = 1,
              MAC802154_FRAME_TYPE_ACK = 2, MAC802154_FRAME_TYPE_MAC_CMD = 3} MAC802154FrameType;

typedef enum  {MAC802154_ADDRESS_NOT_PRESENT = 0, MAC802154_ADDRESS_SHORT = 1,
               MAC802154_ADDRESS_EXTENDED = 2} MAC802154AddressMode;

//#pragma pack(push, 1)

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
            MAC802154FrameType
                    frameType       : 3;
            bool    securityEnabled : 1;
            bool    framePending    : 1;
            bool    ackRequest      : 1;
            bool    panIDCompressed : 1;
            uint8_t _unused07       : 1;
            uint8_t _unused0709     : 2;
            MAC802154AddressMode
                    dstAddressMode  : 2;
            uint8_t frameVersion    : 2;
            MAC802154AddressMode
                    srcAddressMode  : 2;
        };
    };
    uint8_t sequenceNumber;
    MAC802154Address srcAddress;
    MAC802154Address dstAddress;
} MAC802154FrameHeader;

typedef struct
{
    uint64_t    macExtendedAddress;
    uint32_t    macAckWaitDuration;
    bool        macAssociatedPANCoord;
    bool        macAutoRequest;
    bool        macBattLifeExt;
    uint8_t     macBattLifeExtPeriods;
    uint64_t    macCoordExtendedAddress;
    uint16_t    macCoordShortAddress;
    uint8_t     macDSN;
    uint8_t     macMaxBE;
    uint8_t     macMaxCSMABackoffs;
    uint32_t    macMaxFrameTotalWaitTime;
    uint8_t     macMaxFrameRetries;
    uint8_t     macMinBE;
    uint32_t    macLIFSPeriod;
    uint32_t    macSIFSPeriod;
    uint16_t    macPANId;
    bool        macRangingSupported;
    uint8_t     macResponseWaitTime;
    bool        macRxOnWhenIdle;
    bool        macSecurityEnabled;
    uint16_t    macShortAddress;
    uint16_t    macSyncSymbolOffset;
    bool        macTimestampSupported;
    uint32_t    macTxControlActiveDuration;
    uint16_t    macTxControlPauseDuration;
    uint32_t    macTxTotalDuration;

    // FFD devices
    bool        macAssociationPermit;
    uint8_t     *macBeaconPayload;
    uint8_t     macBeaconPayloadLength;
    uint8_t     macBeaconOrder;
    uint32_t    macBeaconTxTime;
    uint8_t     macBSN;
    bool        macPromiscuousMode;
    uint8_t     macSuperframeOrder;
    uint16_t    macTransactionPersistenceTime;
} MAC802154PIB;

// Reduced PIB...
typedef struct
{
    uint8_t     phyCurrentChannel;
    uint8_t     phyTXPowerTolerance;
    int8_t      phyTXPower;
    uint8_t     phyCCAMode;
    uint8_t     phyCurrentPage;
    uint16_t    phyMaxFrameDuration;
    uint16_t    phySHRDuration;
    uint16_t    phySymbolsPerOctet;
} MAC802154PHYPIB;

struct _MAC802154PHYDriverVMT;
typedef struct _MAC802154PHYDriverVMT MAC802154PHYDriverVMT;

typedef struct
{
    MAC802154PHYDriverVMT *phy;
    uint64_t extendedAddress;
} MAC802154Config;

typedef struct
{
    const MAC802154Config *cfg;
    MAC802154PIB pib;
} MAC802154Driver;

typedef struct _MAC802154PHYDriverVMT
{
    MAC802154PHYPIB* (*pib)(MAC802154PHYDriverVMT *phy);
} MAC802154PHYDriverVMT;

//#pragma pack(pop)


void mac802154PackHeader(const MAC802154FrameHeader *header, uint8_t *packed);
void mac802154UnpackHeader(const uint8_t *packed, MAC802154FrameHeader *header);

uint8_t mac802154HeaderSize(const MAC802154FrameHeader *header);

void mac802154Start(MAC802154Driver *macp, const MAC802154Config *config);

#endif
