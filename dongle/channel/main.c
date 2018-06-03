#include "ch.h"
#include "hal.h"

#include "cc2520.h"
#include "usb.h"
#include "rfchannel.h"

#define DEFAULT_PAN_ID        0xDEAD
#define DEFAULT_DEVICE_ID     0xBEEF

static const SPIConfig spiConfig = {
    FALSE,
    NULL,
    GPIO_RF_CS,
    SPI_CR1_BR_1,
    SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

static const CC2520Config cc2520Config = {
    &SPID1
};

static CC2520Driver CC2520D;

static const RFChannelConfig rfcConfig = {
    &CC2520D,
    true,
    DEFAULT_PAN_ID,
    DEFAULT_DEVICE_ID,
    0xC0C0C0C0C0C0C0C0
};

static RFChannelDriver RFCD;
static thread_t *rxThread = NULL;
static MAC802154Address dst;
static uint8_t txbuf[128];
static uint8_t rxbuf[128];

/*
static void rxIRQ(void *arg)
{
    chSysLockFromISR();
    chThdDequeueAllI(&rxQueue, 0);
    chSysUnlockFromISR();
}

static void txIRQ(void *arg)
{
    chSysLockFromISR();
    chThdDequeueAllI(&txQueue, 0);
    chSysUnlockFromISR();
}
*/

static THD_FUNCTION(rxFunc, arg)
{
    spiStart(&SPID1, &spiConfig);

    palSetLine(GPIO_LED_TX);
    chThdSleepMilliseconds(100);

    palClearLine(GPIO_RF_RST);
    palSetLine(GPIO_RF_PDN);
    chThdSleepMicroseconds(100);
    palSetLine(GPIO_RF_RST);
    chThdSleepMicroseconds(200);

    cc2520Start(&CC2520D, &cc2520Config);

    rfcStart(&RFCD, &rfcConfig);

    /*
    cc2520FrameFilterSetup(&CC2520D, true, true);

    uint8_t frmctl = cc2520ReadReg(&CC2520D, CC2520_REG_FRMCTRL0);
    frmctl |= (1 << 5);
    cc2520WriteReg(&CC2520D, CC2520_REG_FRMCTRL0, frmctl);

    ownAddress.extendedAddress = 0xDEADBEEFDEADBEEF;
    cc2520FrameFilterSetLocalAddress(&CC2520D, true, &ownAddress);

    ownAddress.panID = DEFAULT_PAN_ID;
    ownAddress.shortAddress = DEFAULT_DEVICE_ID;
    cc2520FrameFilterSetLocalAddress(&CC2520D, false, &ownAddress);

    cc2520SetupGPIO(&CC2520D, 0, CC2520_GPIO_EX_TX_FRM_DONE, false);
    cc2520SetupGPIO(&CC2520D, 1, CC2520_GPIO_EX_RX_FRM_DONE, false);

    palSetLineCallback(GPIO_RF_GPIO0, &txIRQ, (void*)GPIO_RF_GPIO0);
    palSetLineCallback(GPIO_RF_GPIO1, &rxIRQ, (void*)GPIO_RF_GPIO1);

    cc2520RxOn(&CC2520D);*/

    while (!chThdShouldTerminateX())
    {
        /*chSysLock();
        palEnableLineEventI(GPIO_RF_GPIO1, PAL_EVENT_MODE_FALLING_EDGE);
        chThdEnqueueTimeoutS(&rxQueue, TIME_MS2I(100));
        palDisableLineEventI(GPIO_RF_GPIO1);
        chSysUnlock();*/

        if (chThdShouldTerminateX())
            break;

        uint8_t txsize = chnReadTimeout(&SDU1, txbuf, 128, TIME_MS2I(1));

        if (txsize)
            rfcSend(&RFCD, txbuf, txsize, false, false, &dst, TIME_MS2I(10));

        uint8_t rxsize;
            rxsize = rfcReceive(&RFCD, rxbuf, 128, NULL, TIME_MS2I(100));

        if (rxsize && (SDU1.state == SDU_READY))
            chnWriteTimeout(&SDU1, rxbuf, rxsize, TIME_MS2I(10));

        palClearLine(GPIO_LED_RX);
    }

    cc2520TRxOff(&CC2520D);

    palClearLine(GPIO_RF_RST);
    palClearLine(GPIO_RF_PDN);

    palClearLine(GPIO_LED_TX);

    spiStop(&SPID1);

    rxThread = NULL;
}

static THD_WORKING_AREA(rxWA, 256);
static const thread_descriptor_t rxThreadDescriptor = {
    "rfthread",
    THD_WORKING_AREA_BASE(rxWA),
    THD_WORKING_AREA_END(rxWA),
    NORMALPRIO,
    rxFunc,
    NULL
};

void usbUserEvent(USBDriver *usbp, usbevent_t event)
{
    switch (event)
    {
    case USB_EVENT_CONFIGURED:
    case USB_EVENT_WAKEUP:
        chSysLockFromISR();
        if (!rxThread)
            rxThread = chThdCreateI(&rxThreadDescriptor);
        chSysUnlockFromISR();
        return;

    case USB_EVENT_UNCONFIGURED:
    case USB_EVENT_SUSPEND:
        chSysLockFromISR();
        if (rxThread)
            chThdTerminate(rxThread);
        chSysUnlockFromISR();

    case USB_EVENT_RESET:
    case USB_EVENT_ADDRESS:
    case USB_EVENT_STALLED:
        return;
    }
}

int main(void) 
{
    halInit();
    chSysInit();

    dst.panID = DEFAULT_PAN_ID;
    dst.shortAddress = DEFAULT_DEVICE_ID;

    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serialUSBConfig);

    usbDisconnectBus(serialUSBConfig.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serialUSBConfig.usbp, &usbConfig);
    usbConnectBus(serialUSBConfig.usbp);

    for(;;)
    {
        chThdSleepMilliseconds(100);
    }

}
