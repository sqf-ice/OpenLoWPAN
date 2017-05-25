#include "ch.h"
#include "hal.h"

#include "cc2520.h"

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

CC2520Driver CC2520D;

int main(void) 
{
    halInit();
    chSysInit();

    spiStart(&SPID1, &spicfg);

    cc2520Start(&CC2520D, &cccfg);

    volatile uint8_t status = 0;
    volatile int8_t rssi[16];


    uint8_t frmctl0 = cc2520ReadReg(&CC2520D, CC2520_REG_FRMCTRL0);
    frmctl0 |= (1 << 4);
    cc2520WriteReg(&CC2520D, CC2520_REG_FRMCTRL0, frmctl0);

    while (true)
    {
        for (int i = 0; i < 16; i++)
        {
            cc2520WriteReg(&CC2520D, CC2520_REG_FREQCTRL, 11 + 5 * i);
            cc2520RxOn(&CC2520D);

            osalThreadSleepMilliseconds(10);

            rssi[i] = (int8_t)cc2520ReadReg(&CC2520D, CC2520_REG_RSSI);
            cc2520TRxOff(&CC2520D);
        }

        palSetLine(LINE_LED_RX);
        chThdSleepMilliseconds(500);
        palClearLine(LINE_LED_RX);
        chThdSleepMilliseconds(500);
    }
}
