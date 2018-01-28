#include "ch.h"
#include "hal.h"

#include "cc2520.h"
#include "usb.h"
#include "sniffer.h"

static const SPIConfig spiConfig = {
    NULL,
    GPIOA,
    GPIOA_RF_CS,
    SPI_CR1_BR_1,
    SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

static const CC2520Config cc2520Config = {
    &SPID1
};

CC2520Driver CC2520D;

static void rxIRQ(EXTDriver *extp, expchannel_t channel);
static void updateTimer(GPTDriver *gptp);

static const EXTConfig extConfig = {
    {
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_DISABLED, NULL},
        {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, rxIRQ},
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

static const GPTConfig timerConfig =
{
    1000000,
    updateTimer,
    0,
    0
};

static _THREADS_QUEUE_DECL(rxQueue);

static uint64_t usTimer;
static uint64_t lastPacketTimestamp;

static void updateTimer(GPTDriver *gptp)
{
    usTimer += gptGetIntervalX(gptp);
}

static void rxIRQ(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;

    if (channel == PAL_PAD(GPIO_RF_GPIO0))
    {
        uint16_t timOffset = gptGetCounterX(&GPTD3);
        lastPacketTimestamp = usTimer + timOffset;
    }
    chSysLockFromISR();
    chThdDequeueAllI(&rxQueue, 0);
    chSysUnlockFromISR();
}

static uint8_t buf[128];
static SnifferHeader snifferHeader;

static THD_FUNCTION(sniffer, arg)
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

    cc2520FrameFilterSetup(&CC2520D, false, false);
    cc2520SetupGPIO(&CC2520D, 0, CC2520_GPIO_EX_SFD, false);
    cc2520SetupGPIO(&CC2520D, 1, CC2520_GPIO_EX_RX_FRM_DONE, false);

    uint8_t frmCtl = cc2520ReadReg(&CC2520D, CC2520_REG_FRMCTRL0);
    frmCtl &= ~(1 << 6);
    cc2520WriteReg(&CC2520D, CC2520_REG_FRMCTRL0, frmCtl);

    usTimer = 0;
    gptStart(&GPTD3, &timerConfig);
    gptStartContinuous(&GPTD3, 1000 - 1);

    extStart(&EXTD1, &extConfig);

    cc2520RxOn(&CC2520D);

    uint64_t packetTime = 0;

    while (!chThdShouldTerminateX())
    {
        chSysLock();
        chThdEnqueueTimeoutS(&rxQueue, MS2ST(100));
        chSysUnlock();

        if (chThdShouldTerminateX())
            break;

        uint32_t exceptions = cc2520GetExceptions(&CC2520D);
        cc2520ClearExceptions(&CC2520D, exceptions);

        while (exceptions & ((1 << 6) | (1 << 8) | (1 << 13)))
        {
            if (exceptions & (1 << 6))
            {
                cc2520TRxOff(&CC2520D);
                cc2520FlushRx(&CC2520D);
                cc2520RxOn(&CC2520D);
                break;
            }

            if (exceptions & (1 << 13))
                packetTime = lastPacketTimestamp;

            if (exceptions & (1 << 8))
            {
                uint8_t size = cc2520RxFIFOCount(&CC2520D);

                if (size > 128)
                    break;

                cc2520ReadRxFIFO(&CC2520D, size, buf);
                size = buf[0] > (size - 1) ? (size - 1) : buf[0];

                snifferHeader.magic = SNIFFER_MAGIC;
                snifferHeader.size = size;
                snifferHeader.ts = packetTime;

                if (SDU1.state == SDU_READY)
                {
                    chnWriteTimeout(&SDU1, (uint8_t*)&snifferHeader, sizeof(SnifferHeader), MS2ST(10));
                    chnWriteTimeout(&SDU1, buf + 1, size, MS2ST(10));
                }
            }
            exceptions = cc2520GetExceptions(&CC2520D);
            cc2520ClearExceptions(&CC2520D, exceptions);
        }
    }

    cc2520TRxOff(&CC2520D);

    extChannelDisable(&EXTD1, 8);
    extChannelDisable(&EXTD1, 9);
    extStop(&EXTD1);

    palClearLine(GPIO_RF_RST);
    palClearLine(GPIO_RF_PDN);

    palClearLine(GPIO_LED_TX);

    spiStop(&SPID1);
}

static THD_WORKING_AREA(snifferWA, 256);
static const thread_descriptor_t snifferThreadDescriptor = {
    "sniffer",
    THD_WORKING_AREA_BASE(snifferWA),
    THD_WORKING_AREA_END(snifferWA),
    NORMALPRIO,
    sniffer,
    NULL
};
static thread_t *snifferThread = NULL;

void usbUserEvent(USBDriver *usbp, usbevent_t event)
{
    switch (event)
    {
    case USB_EVENT_CONFIGURED:
    case USB_EVENT_WAKEUP:
        chSysLockFromISR();
        if (!snifferThread)
            snifferThread = chThdCreateI(&snifferThreadDescriptor);
        chSysUnlockFromISR();
        return;

    case USB_EVENT_RESET:
    case USB_EVENT_UNCONFIGURED:
    case USB_EVENT_SUSPEND:
    case USB_EVENT_ADDRESS:
    case USB_EVENT_STALLED:
        return;
    }
}

int main(void) 
{
    halInit();
    chSysInit();

    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serialUSBConfig);

    usbDisconnectBus(serialUSBConfig.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serialUSBConfig.usbp, &usbConfig);
    usbConnectBus(serialUSBConfig.usbp);

    for(;;)
    {
        chSysLock();
        chThdEnqueueTimeoutS(&rxQueue, TIME_INFINITE);
        chSysUnlock();

        palSetLine(GPIO_LED_RX);
        chThdSleepMilliseconds(100);
        palClearLine(GPIO_LED_RX);
    }
}
