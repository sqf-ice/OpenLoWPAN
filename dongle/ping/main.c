#include "ch.h"
#include "hal.h"

#include <string.h>

#include "cc2520.h"
#include "mac802.15.4.h"

#define PING_PONG_PAN_ID        0xDEAD
#define PING_PONG_DEVICE_ID     0xBEEF

CC2520Driver CC2520D;
MAC802154Driver MACD;

static const SPIConfig spicfg = {
    NULL,
    GPIOA,
    GPIOA_RF_CS,
    SPI_CR1_BR_1,
    SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

static const CC2520Config cccfg = {
    &SPID1
};

static const MAC802154Config maccfg = {
    &CC2520D.vmt,
    0x0BC00000DEADBEEFULL
};

static MAC802154Address address;
static MAC802154FrameHeader txHeader;

static const char *text = "Test 802.15.4 packet! Hello there!";


static THD_WORKING_AREA(txWorkingArea, 256);
static THD_FUNCTION(txThread, arg)
{
    txHeader.sequenceNumber = 0;
    txHeader.frameType = MAC802154_FRAME_TYPE_DATA;
    txHeader.securityEnabled = false;
    txHeader.framePending = false;
    txHeader.ackRequest = false;
    txHeader.panIDCompressed = false;
    txHeader.srcAddressMode = MAC802154_ADDRESS_NOT_PRESENT;
    txHeader.frameVersion = MAC802154_FRAME_VERSION;
    txHeader.dstAddressMode = MAC802154_ADDRESS_SHORT;
    txHeader.dstAddress = address;

    while (true)
    {
        palSetLine(GPIO_LED_TX);

        cc2520FlushTx(&CC2520D);
        cc2520WriteTxPacket(&CC2520D, &txHeader, strlen(text) + 1, (const uint8_t*)text);
        cc2520TxOn(&CC2520D);

        chThdSleepMilliseconds(100);

        palClearLine(GPIO_LED_TX);
        
        txHeader.sequenceNumber++;
        
        chThdSleepMilliseconds(1000);
    }
}

int main(void) 
{
    halInit();
    chSysInit();

    spiStart(&SPID1, &spicfg);

    address.panID = PING_PONG_PAN_ID;
    address.shortAddress = PING_PONG_DEVICE_ID;

    cc2520Start(&CC2520D, &cccfg);
    cc2520FrameFilterSetup(&CC2520D, true, true);
    cc2520FrameFilterSetLocalAddress(&CC2520D, false, &address);

    mac802154Start(&MACD, &maccfg);

    volatile int testSize = sizeof (MACD);

    chThdCreateStatic(txWorkingArea, sizeof(txWorkingArea), NORMALPRIO, txThread, NULL);

    while (true)
        chThdSleepMilliseconds(1000);
}
