#include "ch.h"
#include "hal.h"

#include <string.h>

#include "cc2520.h"
#include "mac802.15.4.h"

#define PING_PONG_PAN_ID        0xDEAD
#define PING_PONG_DEVICE_ID     0xBEEF

CC2520Driver CC2520D;
MAC802154Driver MACD;

static void macScanConfirm(MAC802154Driver *macp, MAC802154ScanStatus status,
                           MAC802154ScanType type, int size, const uint8_t *energies);

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
    0x0BC00000DEADBEEFULL,
    &macScanConfirm
};

static THD_WORKING_AREA(macWorkingArea, 256);
static THD_FUNCTION(macThread, arg)
{
    mac802154MLMEScanRequest(&MACD, MAC802154_SCAN_ED, 0, 2, 0);
    while (true)
    {
        chThdSleepMicroseconds(1000);
        mac802154Time(&MACD, 1000);
    }
}

static void macScanConfirm(MAC802154Driver *macp, MAC802154ScanStatus status,
                           MAC802154ScanType type, int size, const uint8_t *energies)
{
    volatile int testSize = sizeof (MACD);
    chThdSleepMicroseconds(1000);
    mac802154MLMEScanRequest(&MACD, MAC802154_SCAN_ED, 0, 2, 0);
}

int main(void) 
{
    halInit();
    chSysInit();

    spiStart(&SPID1, &spicfg);
    cc2520Start(&CC2520D, &cccfg);
    mac802154Start(&MACD, &maccfg);

    chThdCreateStatic(macWorkingArea, sizeof(macWorkingArea), NORMALPRIO, macThread, NULL);

    while (true)
        chThdSleepMilliseconds(1000);
}
