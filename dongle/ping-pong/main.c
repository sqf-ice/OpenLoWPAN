#include "ch.h"
#include "hal.h"

static const SPIConfig spicfg = {
    NULL,
    GPIOA,
    GPIOA_RF_CS,
    SPI_CR1_BR_1,
    SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

int main(void) 
{
    halInit();
    chSysInit();

    spiStart(&SPID1, &spicfg);

    uint8_t cmd = 0;
    uint8_t status;

    while (true)
    {
        spiSelect(&SPID1);
        spiExchange(&SPID1, 1, &cmd, &status);
        spiUnselect(&SPID1);

        palSetLine(LINE_LED_RX);
        chThdSleepMilliseconds(500);
        palClearLine(LINE_LED_RX);
        chThdSleepMilliseconds(500);
    }
}
