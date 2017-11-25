#ifndef ENCCOM_PACKET_HANDLE_H
#define ENCCOM_PACKET_HANDLE_H

#include <linux/ip.h>
#include "options.h"
#include "crypto.h"

int handle_packet_in(struct sk_buff *skb);
int handle_packet_out(struct sk_buff *skb);

#endif