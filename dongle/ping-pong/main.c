#include "ch.h"
#include "hal.h"

#include <string.h>

#include "cc2520.h"
#include "mac802.15.4.h"

#define PING_PONG_PAN_ID        0xDEAD
#define PING_PONG_DEVICE_ID     0xBEEF

static const SPIConfig spicfg = {
    FALSE,
    NULL,
    GPIO_RF_CS,
    SPI_CR1_BR_1,
    SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

static const CC2520Config cccfg = {
    &SPID1
};

static MAC802154Address address;
static MAC802154FrameHeader txHeader, rxHeader;

static const char *text = "PINGPONG";
uint8_t rxbuf[128];

CC2520Driver CC2520D;

static _THREADS_QUEUE_DECL(rxWaitQueue);
static THD_WORKING_AREA(trxWorkingArea, 256);

static void rxIRQ(void *arg)
{
    (void)arg;
    chSysLockFromISR();
    chThdDequeueAllI(&rxWaitQueue, 0);
    chSysUnlockFromISR();
}

static THD_FUNCTION(trxThread, arg)
{
    while (true)
    {
        palSetLine(GPIO_LED_TX);

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

        cc2520FlushTx(&CC2520D);
        cc2520WriteTxPacket(&CC2520D, &txHeader, strlen(text) + 1, (const uint8_t*)text);
        cc2520TxOn(&CC2520D);

        chThdSleepMilliseconds(100);

        palClearLine(GPIO_LED_TX);

        chSysLock();
        chThdEnqueueTimeoutS(&rxWaitQueue, TIME_S2I(5));
        chSysUnlock();

        cc2520WriteReg(&CC2520D, CC2520_REG_EXCFLAG1, 0x0);
        uint8_t rxFIFOCount = cc2520ReadReg(&CC2520D, CC2520_REG_RXFIFOCNT);
        if (rxFIFOCount)
        {
            palSetLine(GPIO_LED_RX);
            cc2520ReadRxPacket(&CC2520D, &rxHeader, 128, rxbuf);
        }
        cc2520FlushRx(&CC2520D);

        chThdSleepMilliseconds(100);
        palClearLine(GPIO_LED_RX);
    }
}

int main(void) 
{
    halInit();
    chSysInit();

    spiStart(&SPID1, &spicfg);

    palSetLineCallback(GPIO_RF_GPIO1, &rxIRQ, NULL);
    palEnableLineEvent(GPIO_RF_GPIO1, PAL_EVENT_MODE_FALLING_EDGE);

    address.panID = PING_PONG_PAN_ID;
    address.shortAddress = PING_PONG_DEVICE_ID;

    cc2520Start(&CC2520D, &cccfg);
    cc2520FrameFilterSetup(&CC2520D, true, true);
    cc2520FrameFilterSetLocalAddress(&CC2520D, false, &address);
    cc2520SetupGPIO(&CC2520D, 1, CC2520_GPIO_EX_RX_FRM_DONE, false);
    cc2520RxOn(&CC2520D);

    chThdCreateStatic(trxWorkingArea, sizeof(trxWorkingArea), NORMALPRIO, trxThread, NULL);

    while (true)
        chThdSleepMilliseconds(1000);
}
