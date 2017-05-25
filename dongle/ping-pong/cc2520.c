#include "cc2520.h"


void cc2520Start(CC2520Driver *ccp, const CC2520Config *config)
{
    ccp->config = config;

    //Startup sequence from datasheet
    cc2520Reset(ccp);

    cc2520WriteReg(ccp, CC2520_REG_TXPOWER,     0x32);
    cc2520WriteReg(ccp, CC2520_REG_CCACTRL0,    0xF8);
    cc2520WriteReg(ccp, CC2520_REG_MDMCTRL0,    0x85);
    cc2520WriteReg(ccp, CC2520_REG_RXCTRL,      0x3F);
    cc2520WriteReg(ccp, CC2520_REG_FSCTRL,      0x5A);
    cc2520WriteReg(ccp, CC2520_REG_FSCAL1,      0x2B);
    cc2520WriteReg(ccp, CC2520_REG_AGCCTRL1,    0x11);
    cc2520WriteReg(ccp, CC2520_REG_ADCTEST0,    0x10);
    cc2520WriteReg(ccp, CC2520_REG_ADCTEST1,    0x0E);
    cc2520WriteReg(ccp, CC2520_REG_ADCTEST2,    0x03);
}

uint8_t cc2520SendOp(CC2520Driver *ccp, uint8_t op)
{
    spiExchange(ccp->config->spi, 1, &op, &ccp->state);
    return ccp->state;
}

uint8_t cc2520GetState(CC2520Driver *ccp)
{
    return ccp->state;
}

uint8_t cc2520UpdateState(CC2520Driver *ccp)
{
    uint8_t op = CC2520_OP_SNOP;
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, op);
    spiUnselect(ccp->config->spi);
    return ccp->state;
}

void cc2520Reset(CC2520Driver *ccp)
{
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_SRES);
    cc2520SendOp(ccp, CC2520_OP_SRES);
    spiUnselect(ccp->config->spi);
}

uint8_t cc2520ReadReg(CC2520Driver *ccp, uint8_t reg)
{
    bool freg = (reg < 0x40);
    uint8_t txdata[2] = {freg ? 0x00 : reg, 0x00};
    uint8_t rxdata[2];
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, freg ? (CC2520_OP_REGRD | reg) : (CC2520_OP_MEMRD));
    spiExchange(ccp->config->spi, freg ? 1 : 2, txdata, rxdata);
    spiUnselect(ccp->config->spi);

    return freg ? rxdata[0] : rxdata[1];
}

void cc2520WriteReg(CC2520Driver *ccp, uint8_t reg, uint8_t data)
{
    bool freg = (reg < 0x40);
    uint8_t txdata[2] = {freg ? 0x00 : reg, data};
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, freg ? (CC2520_OP_REGWR | reg) : (CC2520_OP_MEMWR));
    spiSend(ccp->config->spi, freg ? 1 : 2, txdata);
    spiUnselect(ccp->config->spi);
}

void cc2520RxOn(CC2520Driver *ccp)
{
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_SRXON);
    spiUnselect(ccp->config->spi);
}

void cc2520TxOn(CC2520Driver *ccp)
{
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_STXON);
    spiUnselect(ccp->config->spi);
}

void cc2520TRxOff(CC2520Driver *ccp)
{
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_SRFOFF);
    spiUnselect(ccp->config->spi);
}
