#ifndef SNIFFER_SNIFFER_H
#define SNIFFER_SNIFFER_H

#define SNIFFER_MAGIC 0x5B1FEED4

typedef struct
{
    uint32_t magic;
    uint32_t size;
    uint64_t ts;
} SnifferHeader;

#endif // SNIFFER_SNIFFER_H

