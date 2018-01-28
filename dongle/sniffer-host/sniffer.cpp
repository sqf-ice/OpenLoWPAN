#include <iostream>
#include <fstream>

extern "C" {
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
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
    int inf = open(device.c_str(), O_RDONLY);

    if (inf == -1)
    {
        std::cerr << "Cannot open " << device << ": " << strerror(errno) << std::endl;
        return -1;
    }

    struct termios options;
    tcgetattr(inf, &options);
    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tcsetattr(inf, TCSANOW, &options);

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

    outs->write((char*)&pcapHeader, sizeof(pcap_hdr_t));

    running = true;

    while(running)
    {
        SnifferHeader h = {0, 0, 0};

        while ((h.magic != SNIFFER_MAGIC) && running)
        {
            int status = read(inf, (uint8_t*)&h.magic, 1);

            if (status != 1)
                break;

            uint8_t byte = *((uint8_t*)&h.magic);
            if (byte != ((SNIFFER_MAGIC) & 0xFF))
            {
                std::cerr << "Wrong magic" << std::endl;
                continue;
            }
            read(inf, ((uint8_t*)&h.magic) + 1, 3);
        }

        if (!running)
            break;

        read(inf, &h.size, 4);
        read(inf, &h.ts, 8);
        
        std::cerr << "Packet detected: " << h.size << std::endl;

        if (h.size > 127)
            continue;

        uint8_t packet[127];
        read(inf, packet, h.size);

        pcaprec_hdr_s rec;

        rec.ts_sec = h.ts / 1000000;
        rec.ts_usec = h.ts % 1000000;
        rec.incl_len = h.size;
        rec.orig_len = h.size;

        outs->write((char*)&rec, sizeof(pcaprec_hdr_s));
        outs->write((char*)packet, h.size);
        outs->flush();
    }

    close(inf);

    return 0;
}

int main(int argc, char **argv)
{
    if ((argc != 3) && (argc != 2))
    {
        std::cout << "Usage: openlowpan-sniffer device [pcapfile]" << std::endl;
        return 0;
    }
    return sniff(argv[1], (argc == 3) ? argv[2] : std::string());
}
