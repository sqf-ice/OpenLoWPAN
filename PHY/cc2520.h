#ifndef CC2520_H
#define CC2520_H

#include "hal.h"
#include "cc2520_reg.h"
#include "mac802.14.5.h"

typedef struct 
{
    SPIDriver   *spi;
} CC2520Config;

typedef struct 
{
    const CC2520Config *config;
    uint8_t state;
} CC2520Driver;

void cc2520Start(CC2520Driver *ccp, const CC2520Config *config);

uint8_t cc2520SendOp(CC2520Driver *ccp, uint8_t op);
void cc2520Op(CC2520Driver *ccp, uint8_t op);

uint8_t cc2520GetState(CC2520Driver *ccp);
uint8_t cc2520UpdateState(CC2520Driver *ccp);

void cc2520Reset(CC2520Driver *ccp);

uint8_t cc2520ReadReg(CC2520Driver *ccp, uint8_t reg);
void cc2520WriteReg(CC2520Driver *ccp, uint8_t reg, uint8_t data);

void cc2520ReadMemory(CC2520Driver *ccp, uint16_t address, uint8_t n, uint8_t *data);
void cc2520WriteMemory(CC2520Driver *ccp, uint16_t address, uint8_t n, const uint8_t *data);

void cc2520FrameFilterSetup(CC2520Driver *ccp, bool enabled, bool coordinator);
void cc2520FrameFilterTypes(CC2520Driver *ccp, uint8_t types, bool accept);
void cc2520FrameFilterAddAddress(CC2520Driver *ccp, uint8_t id, bool extended, const MAC802145Address *address);
void cc2520FrameFilterRemoveAddress(CC2520Driver *ccp, uint8_t id, bool extended);
void cc2520FrameFilterSetLocalAddress(CC2520Driver *ccp, bool extended, const MAC802145Address *address);
void cc2520SetupGPIO(CC2520Driver *ccp, uint8_t gpio, uint8_t mode, bool polarity);
void cc2520SetupClockOutput(CC2520Driver *ccp, bool enabled, uint8_t div);

void cc2520RxOn(CC2520Driver *ccp);
void cc2520TxOn(CC2520Driver *ccp);
void cc2520TRxOff(CC2520Driver *ccp);

void cc2520FlushRx(CC2520Driver *ccp);
void cc2520FlushTx(CC2520Driver *ccp);

void cc2520WriteTxFIFO(CC2520Driver *ccp, uint8_t n, const uint8_t *data);
void cc2520ReadRxFIFO(CC2520Driver *ccp, uint8_t n, uint8_t *data);
void cc2520WriteTxPacket(CC2520Driver *ccp, const MAC802145FrameHeader* h, uint8_t n, const uint8_t *data);
void cc2520ReadRxPacket(CC2520Driver *ccp, MAC802145FrameHeader* h, uint8_t n, uint8_t *data);
uint8_t cc2520ReadRawPacket(CC2520Driver *ccp, uint8_t *data);


#endif
