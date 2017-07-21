#include <iostream>
#include <fstream>

extern "C" {
#include "../sniffer/sniffer.h"
}

typedef struct pcap_hdr_s {
    uint32_t magic_number;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t  thiszone;
    uint32_t sigfigs;
    uint32_t snaplen;
    uint32_t network;
} pcap_hdr_t;

typedef struct pcaprec_hdr_s {
    uint32_t ts_sec;
    uint32_t ts_usec;
    uint32_t incl_len;
    uint32_t orig_len;
} pcaprec_hdr_t;

static bool running = false;

int sniff(const std::string& device, const std::string& pcapFile)
{
    std::ifstream ins(device, std::ios_base::binary);

    if (!ins.is_open())
        return -1;

    std::ofstream outf;
    std::ostream *outs = &std::cout;

    if (!pcapFile.empty())
    {
        outf.open(pcapFile, std::ios_base::binary);
        if (!outf.is_open())
            return -1;
        outs = &outf;
    }

    pcap_hdr_t pcapHeader;
    pcapHeader.magic_number = 0xA1B2C3D4;
    pcapHeader.version_major = 2;
    pcapHeader.version_minor = 4;
    pcapHeader.thiszone = 0;
    pcapHeader.sigfigs = 0;
    pcapHeader.snaplen = 0xFFFF;
    pcapHeader.network = 0xC3;

    outf.write((char*)&pcapHeader, sizeof(pcap_hdr_t));

    running = true;

    while(running)
    {
        SnifferHeader h = {0, 0, 0};

        while ((h.magic != SNIFFER_MAGIC) && running)
        {
            uint8_t byte = ins.peek();
            if (byte != ((SNIFFER_MAGIC) & 0xFF))
            {
                std::cerr << "Wrong magic" << std::endl;
                ins.read((char*)&byte, 1);
                continue;
            }

            ins.read((char*)&h.magic, 4);
        }

        if (!running)
            break;

        ins.read((char*)&h.size, 4);
        ins.read((char*)&h.ts, 8);

        if (h.size > 127)
            continue;

        uint8_t packet[127];
        ins.read((char*)packet, h.size);

        pcaprec_hdr_s rec;

        rec.ts_sec = h.ts / 1000000;
        rec.ts_usec = h.ts % 1000000;
        rec.incl_len = h.size;
        rec.orig_len = h.size;

        outs->write((char*)&rec, sizeof(pcaprec_hdr_s));
        outs->write((char*)packet, h.size);
        outs->flush();

        //std::cout << "Packet: " << h.size << " " << h.ts << std::endl;
    }

    return 0;
}

int main(int argc, char **argv)
{
    return sniff(argv[1], argv[2]);
}
