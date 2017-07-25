#include "cc2520.h"


void cc2520Start(CC2520Driver *ccp, const CC2520Config *config)
{
    ccp->config = config;

    //Startup sequence from datasheet
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

void cc2520Op(CC2520Driver *ccp, uint8_t op)
{
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, op);
    spiUnselect(ccp->config->spi);
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
    uint8_t txdata[2] = {freg ? data : reg, data};
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, freg ? (CC2520_OP_REGWR | reg) : (CC2520_OP_MEMWR));
    spiSend(ccp->config->spi, freg ? 1 : 2, txdata);
    spiUnselect(ccp->config->spi);
}

void cc2520RxOn(CC2520Driver *ccp)
{
    cc2520Op(ccp, CC2520_OP_SRXON);
}

void cc2520TxOn(CC2520Driver *ccp)
{
    cc2520Op(ccp, CC2520_OP_STXON);
}

void cc2520TRxOff(CC2520Driver *ccp)
{
    cc2520Op(ccp, CC2520_OP_SRFOFF);
}

void cc2520FlushRx(CC2520Driver *ccp)
{
    cc2520Op(ccp, CC2520_OP_SFLUSHRX);
}

void cc2520FlushTx(CC2520Driver *ccp)
{
    cc2520Op(ccp, CC2520_OP_SFLUSHTX);
}

void cc2520WriteTxFIFO(CC2520Driver *ccp, uint8_t n, const uint8_t *data)
{
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_TXBUF);
    spiSend(ccp->config->spi, n, data);
    spiUnselect(ccp->config->spi);
}

void cc2520WriteTxPacket(CC2520Driver *ccp, const MAC802154FrameHeader* h, uint8_t n, const uint8_t *data)
{
    uint8_t header[MAC802154_MAX_HEADER_SIZE];
    uint8_t headerSize = mac802154HeaderSize(h);
    mac802154PackHeader(h, header);
    header[0] = headerSize - 1 + n + 2;

    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_TXBUF);
    spiSend(ccp->config->spi, headerSize, header);
    spiSend(ccp->config->spi, n, data);
    spiUnselect(ccp->config->spi);
}

void cc2520ReadRxFIFO(CC2520Driver *ccp, uint8_t n, uint8_t *data)
{
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_RXBUF);
    spiReceive(ccp->config->spi, n, data);
    spiUnselect(ccp->config->spi);
}

void cc2520ReadRxPacket(CC2520Driver *ccp, MAC802154FrameHeader* h, uint8_t maxn, uint8_t *data)
{
    uint8_t hdata[MAC802154_MAX_HEADER_SIZE];
    uint8_t headerSize, dataSize;
    uint16_t dataOk;

    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_RXBUF);
    spiReceive(ccp->config->spi, 3, hdata);

    h->frameSize = hdata[0];
    h->frameControl = hdata[1] | (hdata[2] << 8);
    headerSize = mac802154HeaderSize(h);
    dataSize = h->frameSize + 1 - headerSize - 2;

    if (dataSize > maxn)
        dataSize = maxn;

    spiReceive(ccp->config->spi, headerSize - 3, hdata + 3);
    spiReceive(ccp->config->spi, dataSize, data);
    spiReceive(ccp->config->spi, 2, &dataOk);

    spiUnselect(ccp->config->spi);

    mac802154UnpackHeader(hdata, h);
}

void cc2520FrameFilterSetup(CC2520Driver *ccp, bool enabled, bool coordinator)
{
    uint8_t frmfilt = 0xC;

    if (enabled)
        frmfilt |= (1 << 0);

    if (coordinator)
        frmfilt |= (1 << 1);

    cc2520WriteReg(ccp, CC2520_REG_FRMFILT0, frmfilt);
}

void cc2520FrameFilterTypes(CC2520Driver *ccp, uint8_t types, bool accept)
{
    uint8_t filt = cc2520ReadReg(ccp, CC2520_REG_FRMFILT1);
    filt = accept ? (filt | types) : (filt & (~types));
    filt = filt & (~0x07);
    cc2520WriteReg(ccp, CC2520_REG_FRMFILT1, filt);
}

void cc2520FrameFilterAddAddress(CC2520Driver *ccp, uint8_t id, bool extended, const MAC802154Address *address)
{
    uint8_t enableRegAddr = (extended ? CC2520_REG_SRCEXTEN0 : CC2520_REG_SRCSHORTEN0) + id / 8;

    // TODO BSET, BCLR
    uint8_t enableReg = cc2520ReadReg(ccp, enableRegAddr);
    enableReg |= 1 << (id % 8);
    cc2520WriteReg(ccp, enableRegAddr, enableReg);

    uint16_t filtAddress = CC2520_MEM_SRC_ADDR_TABLE + id * 4;
    uint8_t memData[8];

    if (extended)
    {
        memData[0] = (address->extendedAddress >>  0) & 0xFF;
        memData[1] = (address->extendedAddress >>  8) & 0xFF;
        memData[2] = (address->extendedAddress >> 16) & 0xFF;
        memData[3] = (address->extendedAddress >> 24) & 0xFF;
        memData[4] = (address->extendedAddress >> 32) & 0xFF;
        memData[5] = (address->extendedAddress >> 40) & 0xFF;
        memData[6] = (address->extendedAddress >> 48) & 0xFF;
        memData[7] = (address->extendedAddress >> 56) & 0xFF;
    }
    else
    {
        memData[0] = (address->panID >> 0) & 0xFF;
        memData[1] = (address->panID >> 8) & 0xFF;
        memData[2] = (address->shortAddress >> 0) & 0xFF;
        memData[3] = (address->shortAddress >> 8) & 0xFF;
    }
    cc2520WriteMemory(ccp, filtAddress, extended ? 8 : 4, memData);
}

void cc2520FrameFilterRemoveAddress(CC2520Driver *ccp, uint8_t id, bool extended)
{
    uint8_t enableRegAddr = (extended ? CC2520_REG_SRCEXTEN0 : CC2520_REG_SRCSHORTEN0) + id / 8;

    // TODO BSET, BCLR
    uint8_t enableReg = cc2520ReadReg(ccp, enableRegAddr);
    enableReg &= ~(1 << (id % 8));
    cc2520WriteReg(ccp, enableRegAddr, enableReg);
}

void cc2520FrameFilterSetLocalAddress(CC2520Driver *ccp, bool extended, const MAC802154Address *address)
{
    uint16_t filtAddress = extended ? (CC2520_MEM_LOCAL_ADDRESS_INFO) : (CC2520_MEM_LOCAL_ADDRESS_INFO + 8);
    uint8_t memData[8];

    if (extended)
    {
        memData[0] = (address->extendedAddress >>  0) & 0xFF;
        memData[1] = (address->extendedAddress >>  8) & 0xFF;
        memData[2] = (address->extendedAddress >> 16) & 0xFF;
        memData[3] = (address->extendedAddress >> 24) & 0xFF;
        memData[4] = (address->extendedAddress >> 32) & 0xFF;
        memData[5] = (address->extendedAddress >> 40) & 0xFF;
        memData[6] = (address->extendedAddress >> 48) & 0xFF;
        memData[7] = (address->extendedAddress >> 56) & 0xFF;
    }
    else
    {
        memData[0] = (address->panID >> 0) & 0xFF;
        memData[1] = (address->panID >> 8) & 0xFF;
        memData[2] = (address->shortAddress >> 0) & 0xFF;
        memData[3] = (address->shortAddress >> 8) & 0xFF;
    }
    cc2520WriteMemory(ccp, filtAddress, extended ? 8 : 4, memData);
}


void cc2520SetupGPIO(CC2520Driver *ccp, uint8_t gpio, uint8_t mode, bool polarity)
{
    if (gpio > 5)
        return;

    cc2520WriteReg(ccp, CC2520_REG_GPIOCTRL0 + gpio, mode);
    uint8_t rPol = cc2520ReadReg(ccp, CC2520_REG_GPIOPOLARITY);
    rPol = (rPol & (~(1 << gpio))) | ((polarity ? 1 : 0) << gpio);
    cc2520WriteReg(ccp, CC2520_REG_GPIOPOLARITY, rPol);
}

void cc2520SetupClockOutput(CC2520Driver *ccp, bool enabled, uint8_t div)
{
    if (div < 2)
        div = 2;

    if (div > 32)
        div = 32;

    uint8_t reg = cc2520ReadReg(ccp, CC2520_REG_EXTCLOCK);
    reg = ((enabled ? 1 : 0) << 5) | (32 - div);
    cc2520WriteReg(ccp, CC2520_REG_EXTCLOCK, reg);
}

void cc2520ReadMemory(CC2520Driver *ccp, uint16_t address, uint8_t n, uint8_t *data)
{
    uint8_t addr_lsb = address & 0xFF;
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_MEMRD | ((address >> 8) & 0xF));
    spiSend(ccp->config->spi, 1, &addr_lsb);
    spiReceive(ccp->config->spi, n, data);
    spiUnselect(ccp->config->spi);
}

void cc2520WriteMemory(CC2520Driver *ccp, uint16_t address, uint8_t n, const uint8_t *data)
{
    uint8_t addr_lsb = address & 0xFF;
    spiSelect(ccp->config->spi);
    cc2520SendOp(ccp, CC2520_OP_MEMWR | ((address >> 8) & 0xF));
    spiSend(ccp->config->spi, 1, &addr_lsb);
    spiSend(ccp->config->spi, n, data);
    spiUnselect(ccp->config->spi);
}

uint8_t cc2520RxFIFOCount(CC2520Driver *ccp)
{
    return cc2520ReadReg(ccp, CC2520_REG_RXFIFOCNT);
}

uint32_t cc2520GetExceptions(CC2520Driver *ccp)
{
    uint8_t exceptions[3];
    cc2520ReadMemory(ccp, CC2520_REG_EXCFLAG0, 3, exceptions);
    return (exceptions[2] << 16) | (exceptions[1] << 8) | (exceptions[0]);
}

void cc2520ClearExceptions(CC2520Driver *ccp, uint32_t exc)
{
    uint8_t exceptions[3] = {~(exc & 0xFF), ~((exc >> 8) & 0xFF), ~((exc >> 16) & 0xFF)};
    cc2520WriteMemory(ccp, CC2520_REG_EXCFLAG0, 3, exceptions);
}
