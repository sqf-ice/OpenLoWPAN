#include "ch.h"
#include "hal.h"

#include <string.h>

#include "cc2520.h"
#include "mac802.14.5.h"

#define PING_PONG_PAN_ID        0xDEAD
#define PING_PONG_DEVICE_ID     0xBEEF

static void rxIRQ(EXTDriver *extp, expchannel_t channel);

static const EXTConfig extcfg = {
    {
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, rxIRQ},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL}
    }
};

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

static MAC802145Address address;
static MAC802145FrameHeader txHeader, rxHeader;

static const char *text = "PINGPONG";
uint8_t rxbuf[128];

CC2520Driver CC2520D;

static _THREADS_QUEUE_DECL(rxWaitQueue);
static THD_WORKING_AREA(trxWorkingArea, 256);

static void rxIRQ(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;
    chSysLockFromISR();
    chThdDequeueAllI(&rxWaitQueue, 0);
    chSysUnlockFromISR();
}

static THD_FUNCTION(trxThread, arg)
{
    while (true)
    {
        palSetLine(LINE_LED_TX);

        txHeader.sequenceNumber = 0;
        txHeader.frameType = MAC802145_FRAME_TYPE_DATA;
        txHeader.securityEnabled = false;
        txHeader.framePending = false;
        txHeader.ackRequest = false;
        txHeader.panIDCompressed = false;
        txHeader.srcAddressMode = MAC802145_ADDRESS_NOT_PRESENT;
        txHeader.frameVersion = MAC802145_FRAME_VERSION;
        txHeader.dstAddressMode = MAC802145_ADDRESS_SHORT;
        txHeader.dstAddress = address;

        cc2520FlushTx(&CC2520D);
        cc2520WriteTxPacket(&CC2520D, &txHeader, strlen(text) + 1, (const uint8_t*)text);
        cc2520TxOn(&CC2520D);

        chThdSleepMilliseconds(100);

        palClearLine(LINE_LED_TX);

        chSysLock();
        chThdEnqueueTimeoutS(&rxWaitQueue, S2ST(5));
        chSysUnlock();

        cc2520WriteReg(&CC2520D, CC2520_REG_EXCFLAG1, 0x0);
        uint8_t rxFIFOCount = cc2520ReadReg(&CC2520D, CC2520_REG_RXFIFOCNT);
        if (rxFIFOCount)
        {
            palSetLine(LINE_LED_RX);
            cc2520ReadRxPacket(&CC2520D, &rxHeader, 128, rxbuf);
        }
        cc2520FlushRx(&CC2520D);

        chThdSleepMilliseconds(100);
        palClearLine(LINE_LED_RX);
    }
}

int main(void) 
{
    halInit();
    chSysInit();

    extStart(&EXTD1, &extcfg);
    spiStart(&SPID1, &spicfg);

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
