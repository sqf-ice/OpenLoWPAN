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

static void mac802154InitPIB(MAC802154PIB *pib, const MAC802154PHYPIB *ppib, uint64_t extAddress)
{
    pib->macExtendedAddress = extAddress;
    pib->macAckWaitDuration = 20 + 12 + ppib->phySHRDuration + (6 * ppib->phySymbolsPerOctet);
    pib->macAssociatedPANCoord = false;
    pib->macAutoRequest = true;
    pib->macBattLifeExt = false;
    pib->macBattLifeExtPeriods = 6;
    pib->macCoordExtendedAddress = 0xFFFFFFFFFFFFFFFFULL;
    pib->macCoordShortAddress = 0xFFFF;
    pib->macDSN = 0;
    pib->macMaxBE = 5;
    pib->macMaxCSMABackoffs = 4;
    pib->macMaxFrameRetries = 3;
    pib->macMinBE = 3;
    //pib->macLIFSPeriod =
    //pib->macSIFSPeriod =
    pib->macPANId = 0xFFFF;
    pib->macRangingSupported = false;
    pib->macResponseWaitTime = 32;
    pib->macRxOnWhenIdle = false;
    pib->macSecurityEnabled = false;
    pib->macShortAddress = 0xFFFF;
    pib->macSyncSymbolOffset = 0;
    pib->macTimestampSupported = false;
    pib->macTxControlActiveDuration = 2000;
    pib->macTxControlPauseDuration = 2000;
    pib->macTxTotalDuration = 0;

    int m = pib->macMaxBE - pib->macMinBE;
    m = pib->macMaxCSMABackoffs < m ? pib->macMaxCSMABackoffs : m;
    int r = 0;
    for (int i = 0; i < m; i++)
        r += 1 << (pib->macMinBE + i);
    r += ((1 << pib->macMinBE) - 1) * pib->macMaxCSMABackoffs;

    pib->macMaxFrameTotalWaitTime = r * 20 + ppib->phyMaxFrameDuration;
}

void mac802154Start(MAC802154Driver *macp, const MAC802154Config *config)
{
    macp->cfg = config;
    macp->state = MAC802154_S_READY;
    mac802154InitPIB(&macp->pib, config->phy->pib(config->phy), config->extendedAddress);
}

static void mac802154MLMEScanProcess(MAC802154Driver *macp, bool timeout)
{
    if (macp->state != MAC802154_S_SCAN)
        return;

    if (macp->ctx.scanType == MAC802154_SCAN_ED)
    {
        if (timeout && (macp->ctx.scanCurrent >= 0))
        {
            macp->ctx.scanEnergies[macp->ctx.scanCurrent] = macp->cfg->phy->getEnergy(macp);
            macp->cfg->phy->toggleEnergyScan(macp, false);
        }
    }

    macp->ctx.scanCurrent++;

    if (macp->ctx.scanCurrent >= macp->ctx.scanCount)
    {
        macp->state = MAC802154_S_READY;
        if (macp->ctx.scanType == MAC802154_SCAN_ED)
        {
            if (macp->cfg->macMLMEScanConfirm)
                macp->cfg->macMLMEScanConfirm(macp, MAC802154_SCAN_SUCCESS, macp->ctx.scanType,
                                              macp->ctx.scanCount, macp->ctx.scanEnergies);
        }
        return;
    }

    macp->cfg->phy->set(macp, MAC802154_PHY_CURRENT_CHANNEL, &macp->ctx.scanChannels[macp->ctx.scanCurrent]);
    macp->cfg->phy->toggleEnergyScan(macp, true);
    macp->stateTimeUS = 0;
}

void mac802154Time(MAC802154Driver *macp, int deltaUS)
{
    macp->stateTimeUS += deltaUS;

    switch (macp->state)
    {
    case MAC802154_S_SCAN:
        if (macp->stateTimeUS >= ((1 << macp->ctx.scanDuration) * MAC802154_A_BASE_SUPERFRAME_DURATION * 16))
            mac802154MLMEScanProcess(macp, true);
    }
}

void mac802154MLMEScanRequest(MAC802154Driver *macp, MAC802154ScanType type, int *channels,
                              int duration, int channelPage)
{
    if (!channels)
    {
        for (int i = 0; i < MAC802154_MAX_CHANNELS; i++)
        {
            macp->ctx.scanChannels[i] = i + 11;
            macp->ctx.scanEnergies[i] = 0xFF;
        }
        macp->ctx.scanCount = MAC802154_MAX_CHANNELS;
    }
    else
    {
        int i = 0;
        while (channels[i] >= 0)
        {
            macp->ctx.scanChannels[i] = channels[i];
            macp->ctx.scanEnergies[i] = 0xFF;
            i++;
        }
        macp->ctx.scanCount = i;
    }
    macp->ctx.scanCurrent = -1;
    macp->ctx.scanDuration = duration;
    macp->ctx.scanPage = channelPage;
    macp->ctx.scanType = type;

    macp->state = MAC802154_S_SCAN;
    macp->stateTimeUS = 0;

    macp->cfg->phy->set(macp, MAC802154_PHY_CURRENT_PAGE, &macp->ctx.scanPage);

    mac802154MLMEScanProcess(macp, false);
}
