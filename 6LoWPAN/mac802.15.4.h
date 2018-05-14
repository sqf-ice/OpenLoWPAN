#ifndef MAC_802_15_4_H
#define MAC_802_15_4_H 

#include <stdint.h>
#include <stdbool.h>

#define MAC802154_A_MAX_PHY_PACKET_SIZE         127
#define MAC802154_A_TURNAROUND_TIME             12

#define MAC802154_A_BASE_SLOT_DURATION          60
#define MAC802154_A_GTS_DESC_PERSISTENCE_TIME   4     
#define MAC802154_A_MAX_BEACON_OVERHEAD         75 
#define MAC802154_A_MAX_LOST_BEACONS            4
#define MAC802154_A_MAX_MPDU_UNSECURED_OVERHEAD 25
#define MAC802154_A_MAX_SIFS_FRAME_SIZE         18
#define MAC802154_A_MAX_SIFS_FRAME_SIZE         18
#define MAC802154_A_MIN_CAP_LENGTH              440
#define MAC802154_A_MIN_MPDU_OVERHEAD           9
#define MAC802154_A_NUM_SUPERFRAME_SLOTS        16
#define MAC802154_A_UNIT_BACKOFF_PERIOD         20
#define MAC802154_A_BASE_SUPERFRAME_DURATION    (MAC802154_A_BASE_SLOT_DURATION * MAC802154_A_NUM_SUPERFRAME_SLOTS)
#define MAC802154_A_MAX_BEACON_PAYLOAD_LENGTH   (MAC802154_A_MAX_PHY_PACKET_SIZE – MAC802154_A_MAX_BEACON_OVERHEAD)
#define MAC802154_A_MAX_MAC_SAFE_PAYLOAD_SIZE   (MAC802154_A_MAX_PHY_PACKET_SIZE – MAC802154_A_MAX_MPDU_UNSECURED_OVERHEAD)

// FIXME: Security headers not included
#define MAC802154_MAX_HEADER_SIZE               24

#define MAC802154_MAX_CHANNELS                  16
#define MAC802154_MAX_SCAN_PANS                 4

#define MAC802154_FRAME_VERSION                 1

#define MAC802154_CMD_ASSOCIATE_REQUEST         1
#define MAC802154_CMD_ASSOCIATE_RESPONSE        2
#define MAC802154_CMD_DISASSOCIATOIN_NOTIFY     3
#define MAC802154_CMD_DATA_REQUEST              4
#define MAC802154_CMD_PAN_ID_CONFLICT_NOTIFY    5
#define MAC802154_CMD_ORPHAN_NOTIFY             6
#define MAC802154_CMD_BEACON_REQUEST            7
#define MAC802154_CMD_COORDINATOR_REALIGNMENT   8
#define MAC802154_CMD_GTS_REQUSET               9

typedef enum {MAC802154_FRAME_TYPE_BEACON = 0, MAC802154_FRAME_TYPE_DATA = 1,
              MAC802154_FRAME_TYPE_ACK = 2, MAC802154_FRAME_TYPE_MAC_CMD = 3} 
              MAC802154FrameType;

typedef enum  {MAC802154_ADDRESS_NOT_PRESENT = 0, MAC802154_ADDRESS_SHORT = 2,
               MAC802154_ADDRESS_EXTENDED = 3}
               MAC802154AddressMode;

//#pragma pack(push, 1)

typedef struct
{
    uint16_t panID;
    union 
    {
        uint16_t shortAddress;
        uint64_t extendedAddress;
    };
} MAC802154Address;

typedef struct
{
    uint8_t frameSize;
    union {
        uint16_t frameControl;
        struct
        {
            MAC802154FrameType
                    frameType       : 3;
            bool    securityEnabled : 1;
            bool    framePending    : 1;
            bool    ackRequest      : 1;
            bool    panIDCompressed : 1;
            uint8_t _unused07       : 1;
            uint8_t _unused0709     : 2;
            MAC802154AddressMode
                    dstAddressMode  : 2;
            uint8_t frameVersion    : 2;
            MAC802154AddressMode
                    srcAddressMode  : 2;
        };
    };
    uint8_t sequenceNumber;
    MAC802154Address srcAddress;
    MAC802154Address dstAddress;
} MAC802154FrameHeader;

typedef struct
{
    MAC802154Address coord;
    uint8_t channel;
    uint8_t channelPage;
    uint8_t linkQuality;
} MAC802154PANDescriptor;

typedef enum {
    MAC802154_MAC_EXTENDED_ADDRESS, MAC802154_MAC_ACK_WAIT_DURATION,
    MAC802154_MAC_ASSOCIATED_PAN_COORD, MAC802154_MAC_AUTO_REQUEST,
    MAC802154_MAC_BATT_LIFE_EXT, MAC802154_MAC_BATT_LIFE_EXT_PERIODS,
    MAC802154_MAC_COORD_EXTENDED_ADDRESS, MAC802154_MAC_COORD_SHORT_ADDRESS,
    MAC802154_MAC_DSN, MAC802154_MAC_MAX_BE, MAC802154_MAC_MAX_CSMA_BACKOFFS,
    MAC802154_MAC_MAX_FRAME_TOTAL_WAIT_TIME, MAC802154_MAC_MAX_FRAME_RETRIES,
    MAC802154_MAC_MIN_BE, MAC802154_MAC_LIFS_PERIOD, MAC802154_MAC_SIFS_PERIOD,
    MAC802154_MAC_PAN_ID, MAC802154_MAC_RANGING_SUPPORTED,
    MAC802154_MAC_RESPONSE_WAIT_TIME, MAC802154_MAC_RX_ON_WHEN_IDLE,
    MAC802154_MAC_SECURITY_ENABLED, MAC802154_MAC_SHORT_ADDRESS,
    MAC802154_MAC_SYNC_SYMBOL_OFFSET, MAC802154_MAC_TIMESTAMP_SUPPORTED,
    MAC802154_MAC_TX_CONTROL_ACTIVE_DURATION, 
    MAC802154_MAC_TX_CONTROL_PAUSE_DURATION, MAC802154_MAC_TX_TOTAL_DURATION,
    MAC802154_MAC_ASSOCIATION_PERMIT, MAC802154_MAC_BEACON_PAYLOAD,
    MAC802154_MAC_BEACON_PAYLOAD_LENGTH, MAC802154_MAC_BEACON_ORDER,
    MAC802154_MAC_BEACON_TX_TIME, MAC802154_MAC_BSN,
    MAC802154_MAC_PROMISCUOUS_MODE, MAC802154_MAC_SUPERFRAME_ORDER,
    MAC802154_MAC_TRANSACTION_PERSISTENCE_TIME
} MAC802154Parameter;

typedef struct
{
    uint64_t    macExtendedAddress;
    uint32_t    macAckWaitDuration;
    bool        macAssociatedPANCoord;
    bool        macAutoRequest;
    bool        macBattLifeExt;
    uint8_t     macBattLifeExtPeriods;
    uint64_t    macCoordExtendedAddress;
    uint16_t    macCoordShortAddress;
    uint8_t     macDSN;
    uint8_t     macMaxBE;
    uint8_t     macMaxCSMABackoffs;
    uint32_t    macMaxFrameTotalWaitTime;
    uint8_t     macMaxFrameRetries;
    uint8_t     macMinBE;
    uint32_t    macLIFSPeriod;
    uint32_t    macSIFSPeriod;
    uint16_t    macPANId;
    bool        macRangingSupported;
    uint8_t     macResponseWaitTime;
    bool        macRxOnWhenIdle;
    bool        macSecurityEnabled;
    uint16_t    macShortAddress;
    uint16_t    macSyncSymbolOffset;
    bool        macTimestampSupported;
    uint32_t    macTxControlActiveDuration;
    uint16_t    macTxControlPauseDuration;
    uint32_t    macTxTotalDuration;

    // FFD devices
    bool        macAssociationPermit;
    uint8_t     *macBeaconPayload;
    uint8_t     macBeaconPayloadLength;
    uint8_t     macBeaconOrder;
    uint32_t    macBeaconTxTime;
    uint8_t     macBSN;
    bool        macPromiscuousMode;
    uint8_t     macSuperframeOrder;
    uint16_t    macTransactionPersistenceTime;
} MAC802154PIB;

typedef enum {
    MAC802154_PHY_CURRENT_CHANNEL, MAC802154_PHY_TX_POWER_TOLERANCE, 
    MAC802154_PHY_TX_POWER, MAC802154_PHY_CCA_MODE, MAC802154_PHY_CURRENT_PAGE, 
    MAC802154_PHY_MAX_FRAME_DURATION, MAC802154_PHY_SHR_DURATION, 
    MAC802154_PHY_SYMBOLS_PER_OCTET
} MAC802154PHYParameter;

typedef enum {
    MAC802154_SCAN_ED, MAC802154_SCAN_ACTIVE, MAC802154_SCAN_PASSIVE,
    MAC802154_SCAN_ORPHAN
} MAC802154ScanType;

typedef enum {
    MAC802154_SCAN_SUCCESS, MAC802154_SCAN_LIMIT_REACHED, MAC802154_SCAN_NO_BEACON,
    MAC802154_SCAN_IN_PROGRESS, MAC802154_SCAN_COUNTER_ERROR, MAC802154_SCAN_FRAME_TOO_LONG,
    MAC802154_SCAN_UNAVAILABLE_KEY, MAC802154_SCAN_UNSUPPORTED_SECURITY,
    MAC802154_SCAN_INVALID_PARAMETER
} MAC802154ScanStatus;

typedef enum {
    MAC802154_S_READY, MAC802154_S_SCAN
} MAC802154State;

// Reduced PIB...
typedef struct
{
    uint8_t     phyCurrentChannel;
    uint8_t     phyTXPowerTolerance;
    int8_t      phyTXPower;
    uint8_t     phyCCAMode;
    uint8_t     phyCurrentPage;
    uint16_t    phyMaxFrameDuration;
    uint16_t    phySHRDuration;
    uint8_t     phySymbolsPerOctet;
    uint8_t     phySymbolTime;
} MAC802154PHYPIB;

struct _MAC802154PHYDriverVMT;
typedef struct _MAC802154PHYDriverVMT MAC802154PHYDriverVMT;

struct _MAC802154Driver;
typedef struct _MAC802154Driver MAC802154Driver;

typedef struct
{
    MAC802154PHYDriverVMT *phy;
    uint64_t extendedAddress;
    void (*macMLMEScanConfirm)(MAC802154Driver *macp, MAC802154ScanStatus status,
                               MAC802154ScanType type, int size, const uint8_t *energies,
                               MAC802154PANDescriptor *pans);
} MAC802154Config;

typedef struct _MAC802154Driver
{
    const MAC802154Config *cfg;
    MAC802154PIB pib;
    MAC802154State state;
    int stateTimeUS;
    MAC802154FrameHeader txHeader;
    MAC802154FrameHeader rxHeader;
    union {
        struct {
            uint8_t scanChannels[MAC802154_MAX_CHANNELS];
            union {
                uint8_t scanEnergies[MAC802154_MAX_CHANNELS];
                MAC802154PANDescriptor scanPANs[MAC802154_MAX_SCAN_PANS];
            };
            int8_t  scanCurrent;
            uint8_t scanCount;
            uint8_t scanPage;
            uint8_t scanDuration;
            MAC802154ScanType scanType;
        };
    } ctx;
} MAC802154Driver;

typedef struct _MAC802154PHYDriverVMT
{
    MAC802154PHYPIB* (*pib)(MAC802154PHYDriverVMT *phy);
    int (*set)(MAC802154Driver *macp, MAC802154PHYParameter param, void *data);
    int (*get)(MAC802154Driver *macp, MAC802154PHYParameter param, void *data);

    void (*toggleRX)(MAC802154Driver *macp, bool on);

    void (*toggleEnergyScan)(MAC802154Driver *macp, bool on);
    uint8_t (*getEnergy)(MAC802154Driver *macp);

    void (*txPacket)(MAC802154Driver *macp, const MAC802154FrameHeader* h,
                     uint8_t n, const uint8_t *data);
} MAC802154PHYDriverVMT;


//#pragma pack(pop)


void mac802154PackHeader(const MAC802154FrameHeader *header, uint8_t *packed);
void mac802154UnpackHeader(const uint8_t *packed, MAC802154FrameHeader *header);

uint8_t mac802154HeaderSize(const MAC802154FrameHeader *header);

void mac802154Start(MAC802154Driver *macp, const MAC802154Config *config);

void mac802154Time(MAC802154Driver *macp, int deltaUS);
void mac802154MLMEScanRequest(MAC802154Driver *macp, MAC802154ScanType type, int *channels,
                              int duration, int channelPage);

void mac802154MCPSDataIndication(MAC802154Driver *macp, uint8_t n, const uint8_t *data);

#endif
