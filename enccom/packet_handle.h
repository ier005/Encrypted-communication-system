#ifndef ENCCOM_PACKET_HANDLE_H
#define ENCCOM_PACKET_HANDLE_H

#include <linux/ip.h>
#include <net/ip.h>
#include <net/tcp.h>
#include "options.h"

#define ENC_PROTO 50

int handle_packet_in(struct sk_buff *skb);
int handle_packet_out(struct sk_buff *skb);

#endif