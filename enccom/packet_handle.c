#include "packet_handle.h"

int handle_packet_in(struct sk_buff *skb)
{
	if (!mod_running)
		return 0;
	struct iphdr *ipheader = ip_hdr(skb);
	struct option *opt = opt_in_head;


	while (opt) {
		if (opt->ip == ipheader->saddr) {
			unsigned char *buf = skb_network_header(skb) + ipheader->ihl * 4;
			crypt(ENCCOM_DECRYPT, buf, skb_tail_pointer(skb) - buf, opt->key);
			printk("dport: %hu\n", ntohs(*(short *)(buf+2)));
			break;
		}
		opt = opt->next;
	}

	return 0;
}

int handle_packet_out(struct sk_buff *skb)
{
	if (!mod_running)
		return 0;
	struct iphdr *ipheader = ip_hdr(skb);
	struct option *opt = opt_out_head;

	while (opt) {
		if (opt->ip == ipheader->daddr) {
			unsigned char *buf = skb_network_header(skb) + ipheader->ihl * 4;
			printk("dport: %hu\n", ntohs(*(short *)(buf+2)));
			
			struct tcphdr *tcph = tcp_hdr(skb);
			tcph->check = 0;
			skb->csum = csum_partial((unsigned char *)tcph, ntohs(ipheader->tot_len) - ip_hdrlen(skb), 0);
			tcph->check = csum_tcpudp_magic(ipheader->saddr, ipheader->daddr, ntohs(ipheader->tot_len) - ip_hdrlen(skb), ipheader->protocol, skb->csum);
			ipheader->check = 0;
			ipheader->check = ip_fast_csum(ipheader, ipheader->ihl);
			skb->ip_summed = CHECKSUM_NONE;
			crypt(ENCCOM_ENCRYPT, buf, skb_tail_pointer(skb) - buf, opt->key);
			break;
		}
		opt = opt->next;
	}

	return 0;
}