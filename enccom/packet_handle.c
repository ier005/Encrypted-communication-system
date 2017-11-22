#include "packet_handle.h"

extern struct option *option_head, *option_tail;

int handle_packet(struct sk_buff *skb)
{
	struct iphdr *ipheader = ip_hdr(skb);
	struct option *opt = option_head;

	while (opt) {
		if (opt->ip == ipheader->saddr) {
			printk("ip header length: %d, protocol: %d, saddr: %#x, daddr: %#x\n", ipheader->ihl * 4, ipheader->protocol, ipheader->saddr, ipheader->daddr);
		}
		opt = opt->next;
	}

	return 0;
}