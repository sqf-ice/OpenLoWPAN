#include "rfchannel.h"

#ifndef chThdYield
#define chThdYield() chThdSleepMicroseconds(100)
#endif

void rfcStart(RFChannelDriver *rfp, const RFChannelConfig *cfg)
{
    rfp->cfg = cfg;

    rfp->txHeader.sequenceNumber = 1;
    rfp->txHeader.frameType = MAC802154_FRAME_TYPE_DATA;
    rfp->txHeader.securityEnabled = false;
    rfp->txHeader.framePending = false;
    rfp->txHeader.ackRequest = true;
    rfp->txHeader.panIDCompressed = false;
    if (rfp->cfg->coordinator)
        rfp->txHeader.srcAddressMode = MAC802154_ADDRESS_NOT_PRESENT;
    else
        rfp->txHeader.srcAddressMode = MAC802154_ADDRESS_SHORT;
    rfp->txHeader.frameVersion = MAC802154_FRAME_VERSION;
    rfp->txHeader.dstAddressMode = MAC802154_ADDRESS_NOT_PRESENT;
    rfp->txHeader.srcAddress.panID = rfp->cfg->panAddress;
    rfp->txHeader.srcAddress.shortAddress = rfp->cfg->shortAddress;

    rfp->firstRx = true;
    
    cc2520FrameFilterSetup(rfp->cfg->ccp, true, rfp->cfg->coordinator);
    cc2520FrameFilterSetLocalAddress(rfp->cfg->ccp, false, &rfp->txHeader.srcAddress);

    uint8_t frmctl = cc2520ReadReg(rfp->cfg->ccp, CC2520_REG_FRMCTRL0);
    frmctl |= (1 << 5);
    cc2520WriteReg(rfp->cfg->ccp, CC2520_REG_FRMCTRL0, frmctl);

    //cc2520SetupGPIO(&RFD1, 1, CC2520_GPIO_EX_RX_FRM_DONE, false);

    cc2520RxOn(rfp->cfg->ccp);

    //palSetLineCallback(GPIO_RF_GPIO0, &rxIRQ, (void*)GPIO_RF_GPIO0);
}

void rfcStop(RFChannelDriver *rfp)
{
    cc2520TRxOff(rfp->cfg->ccp);
}

bool rfcWaitACK(RFChannelDriver *rfp, sysinterval_t timeout)
{
    systime_t start = chVTGetSystemTimeX();
    bool fail = true;

    while (chVTTimeElapsedSinceX(start) < timeout)
    {
        uint32_t exceptions = cc2520GetExceptions(rfp->cfg->ccp);
        cc2520ClearExceptions(rfp->cfg->ccp, exceptions);

        if (!(exceptions & ((1 << 6) | (1 << 8))))
            continue;

        if (exceptions & (1 << 6))
        {
            cc2520TRxOff(rfp->cfg->ccp);
            cc2520FlushRx(rfp->cfg->ccp);
            cc2520RxOn(rfp->cfg->ccp);
            fail = true;
            break;
        }

        if (exceptions & (1 << 8))
        {
            uint8_t size = cc2520RxFIFOCount(rfp->cfg->ccp);

            if (size > 128)
                size = 128;

            cc2520ReadRxFIFO(rfp->cfg->ccp, size, rfp->rxBuffer);
            size = rfp->rxBuffer[0] > (size - 1) ? (size - 1) : rfp->rxBuffer[0];
            bool crcOk = (rfp->rxBuffer[size] & (1 << 7)) ? true : false;

            if (!crcOk)
                continue;

            mac802154UnpackHeader(rfp->rxBuffer, &rfp->rxHeader);

            if ((rfp->rxHeader.frameType == MAC802154_FRAME_TYPE_ACK) &&
                (rfp->rxHeader.sequenceNumber == rfp->txHeader.sequenceNumber))
            {
                fail = false;
                break;
            }
        }
        chThdYield();
    }

    return !fail;
}

uint8_t rfcSend(RFChannelDriver *rfp, const uint8_t *data, uint8_t size,
                bool toCoord, bool extendedAddr, const MAC802154Address *dst, sysinterval_t timeout)
{
    uint8_t sended = 0;
    rfp->txHeader.dstAddressMode = toCoord ? MAC802154_ADDRESS_NOT_PRESENT :
                                             (extendedAddr ? MAC802154_ADDRESS_EXTENDED : MAC802154_ADDRESS_SHORT);
    if (!toCoord)
        rfp->txHeader.dstAddress = *dst;

    uint8_t blockSize = 128 - mac802154HeaderSize(&rfp->txHeader) - 2;

    systime_t start = chVTGetSystemTimeX();

    while ((sended < size) && (chVTTimeElapsedSinceX(start) < timeout))
    {
        uint8_t curSize = (size - sended) > blockSize ? blockSize : (size - sended);

        cc2520FlushTx(rfp->cfg->ccp);
        cc2520WriteTxPacket(rfp->cfg->ccp, &rfp->txHeader, curSize, data + sended);
        cc2520TxOn(rfp->cfg->ccp);

        bool hasAck = false;
        while (!hasAck && (chVTTimeElapsedSinceX(start) < timeout))
        {
            if (rfcWaitACK(rfp, TIME_MS2I(5)))
            {
                hasAck = true;
                break;
            }
            cc2520TxOn(rfp->cfg->ccp);
        }

        sended += curSize;
        rfp->txHeader.sequenceNumber++;
    }

    return sended;
}

uint8_t rfcReceive(RFChannelDriver *rfp, uint8_t *data, uint8_t size, MAC802154Address *src, sysinterval_t timeout)
{
    systime_t start = chVTGetSystemTimeX();
    uint8_t received = 0;

    while ((chVTTimeElapsedSinceX(start) < timeout) && (received < size))
    {
        uint32_t exceptions = cc2520GetExceptions(rfp->cfg->ccp);
        cc2520ClearExceptions(rfp->cfg->ccp, exceptions);

        if (!(exceptions & ((1 << 6) | (1 << 8))))
            continue;

        if (exceptions & (1 << 6))
        {
            cc2520TRxOff(rfp->cfg->ccp);
            cc2520FlushRx(rfp->cfg->ccp);
            cc2520RxOn(rfp->cfg->ccp);
            break;
        }

        if (exceptions & (1 << 8))
        {
            uint8_t psize = cc2520RxFIFOCount(rfp->cfg->ccp);

            if (psize > 128)
                psize = 128;

            cc2520ReadRxFIFO(rfp->cfg->ccp, psize, rfp->rxBuffer);
            psize = rfp->rxBuffer[0] > (psize - 1) ? (psize - 1) : rfp->rxBuffer[0];
            bool crcOk = (rfp->rxBuffer[psize] & (1 << 7)) ? true : false;

            if (!crcOk)
                continue;

            uint8_t oldSeq = rfp->rxHeader.sequenceNumber;

            mac802154UnpackHeader(rfp->rxBuffer, &rfp->rxHeader);
            uint8_t hSize = mac802154HeaderSize(&rfp->rxHeader);

            if ((rfp->rxHeader.sequenceNumber == oldSeq) && !rfp->firstRx)
                continue;

            rfp->firstRx = false;

            for (int i = 0; (i < (size - received)) && (i < psize - hSize - 1); i++)
            {
                data[received] = rfp->rxBuffer[hSize + i];
                received++;
            }
        }
        chThdSleepMilliseconds(1);
    }

    if (src)
        *src = rfp->rxHeader.srcAddress;

    return received;
}
