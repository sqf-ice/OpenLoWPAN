#ifndef CC2520_H
#define CC2520_H

#include "hal.h"
#include "cc2520_reg.h"

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

uint8_t cc2520GetState(CC2520Driver *ccp);
uint8_t cc2520UpdateState(CC2520Driver *ccp);

void cc2520Reset(CC2520Driver *ccp);

uint8_t cc2520ReadReg(CC2520Driver *ccp, uint8_t reg);
void cc2520WriteReg(CC2520Driver *ccp, uint8_t reg, uint8_t data);

void cc2520RxOn(CC2520Driver *ccp);
void cc2520TxOn(CC2520Driver *ccp);
void cc2520TRxOff(CC2520Driver *ccp);

#endif
