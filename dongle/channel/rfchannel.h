#ifndef RF_CHANNEL_H
#define RF_CHANNEL_H 

#include "cc2520.h"
#include "hal.h"

typedef struct {
    CC2520Driver *ccp;
    bool coordinator;
    uint16_t panAddress;
    uint16_t shortAddress;
    uint64_t longAddress;
    int txNumRetries;
} RFChannelConfig;

typedef struct {
    const RFChannelConfig *cfg;

    MAC802154FrameHeader txHeader;
    MAC802154FrameHeader rxHeader;
    uint8_t rxBuffer[128];
    bool firstRx;
} RFChannelDriver;

void rfcStart(RFChannelDriver *rfp, const RFChannelConfig *cfg);
void rfcStop(RFChannelDriver *rfp);
uint8_t rfcSend(RFChannelDriver *rfp, const uint8_t *data, uint8_t size,
                bool toCoord, bool extendedAddr, const MAC802154Address *dst, sysinterval_t timeout);
uint8_t rfcReceive(RFChannelDriver *rfp, uint8_t *data, uint8_t size, MAC802154Address *src, sysinterval_t timeout);

bool rfcWaitACK(RFChannelDriver *rfp, sysinterval_t timeout);

#endif
