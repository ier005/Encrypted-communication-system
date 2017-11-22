#ifndef ENCCOM_PACKET_HANDLE_H
#define ENCCOM_PACKET_HANDLE_H

#include <linux/ip.h>
#include "options.h"

int handle_packet(struct sk_buff *skb);

#endif