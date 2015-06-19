#include <util/dump.h>
#include <transport.h>
#include <stdio.h>

void hci_dump(dump_t *dump,transport_t channel,
        const void *buffer,size_t len)
{
    switch(channel) {
    case CHANNEL_HCI:
        channel = 0x04;
        break;
    case CHANNEL_ACL:
        channel = 0x02;
        break;
    case CHANNEL_SCO:
        channel = 0x03;
        break;
    default:
        break;
    }
    fprintf(dump,"\n%06x %02x ",0x00,channel); 
    for(size_t i = 0;i < len;i++) {
        if(!((i + 1) % 16))
            fprintf(dump,"\n%04x ",i + 1);
        fprintf(dump,"%02x ",((u8*)buffer)[i]);
    }
    fflush(dump);
}

