#include "packet_handle.h"
#include <linux/byteorder/generic.h>

extern struct option *opt_in_head, *opt_in_tail, *opt_out_head, *opt_out_tail;


int handle_packet_in(struct sk_buff *skb)
{
	struct iphdr *ipheader = ip_hdr(skb);
	//struct skb_shared_info *psh = skb_shinfo(skb);
	struct option *opt = opt_in_head;


	while (opt) {
		if (opt->ip == ipheader->saddr) {
			//printk("ip header length: %d, protocol: %d, saddr: %#x, daddr: %#x\n", ipheader->ihl * 4, ipheader->protocol, ipheader->saddr, ipheader->daddr);
			unsigned char *buf = skb_network_header(skb) + ipheader->ihl * 4;
			/*if (ipheader->protocol == 51) {
				ipheader->protocol = 6;
			}*/
			crypt(ENCCOM_DECRYPT, buf, skb_tail_pointer(skb) - buf, opt->key);
			printk("dport: %hu\n", ntohs(*(short *)(buf+2)));
			break;
		}
		opt = opt->next;
	}


/*
	printk("in: len: %d, data_len: %d, room: %d, real length: %d, ip length: %hu\n", skb->len, skb->data_len, skb->end - skb->tail, skb_headlen(skb), ntohs(ipheader->tot_len));
	if (skb_is_nonlinear(skb))
		printk("This packet is not linear. The nr_frags is %d. saddr=%#x, daddr=%#x\n", psh->nr_frags, ipheader->saddr, ipheader->daddr);
	if (skb->next)
		printk("next is not NULL");
	if (psh->frag_list) {
		struct sk_buff *s = psh->frag_list;
		while (s) {
			printk("frag_list real length: %d\n", skb_headlen(s));
			s = s->next;
		}
	}
	if (ipheader->saddr == 0xee62a8c0) {
	}
*/
	return 0;
}

int handle_packet_out(struct sk_buff *skb)
{
	struct iphdr *ipheader = ip_hdr(skb);
	/*struct skb_shared_info *psh = skb_shinfo(skb);
	int key_len = 16;
	printk("out: len: %d, data_len: %d, room: %d, real length: %d, ip length: %hu\n", skb->len, skb->data_len, skb->end - skb->tail, skb->len - skb->data_len, ntohs(ipheader->tot_len));
	if (skb_is_nonlinear(skb))
		printk("This packet is not linear. The nr_frags is %d. saddr=%#x, daddr=%#x\n", psh->nr_frags, ipheader->saddr, ipheader->daddr);
	if (skb->next)
		printk("next is not NULL");
	if (psh->frag_list) {
		struct sk_buff *s = psh->frag_list;
		while (s) {
			printk("frag_list real length: %d\n", skb_headlen(s));
			s = s->next;
		}
	}

	if (skb->data_len != 0)
		return 1;

	int room = skb->end - skb->tail;
	int remainder = skb->len % key_len;
	if (room < key_len + remainder)
		return 1;
	unsigned int datalen = 
	if (room < )
	*/
	struct option *opt = opt_out_head;


	while (opt) {
		if (opt->ip == ipheader->daddr) {
			//printk("ip header length: %d, protocol: %d, saddr: %#x, daddr: %#x\n", ipheader->ihl * 4, ipheader->protocol, ipheader->saddr, ipheader->daddr);
			unsigned char *buf = skb_network_header(skb) + ipheader->ihl * 4;
			/*if (ipheader->protocol == 51) {
				ipheader->protocol = 6;
			}*/
			crypt(ENCCOM_DECRYPT, buf, skb_tail_pointer(skb) - buf, opt->key);
			printk("dport: %hu\n", ntohs(*(short *)(buf+2)));
			break;
		}
		opt = opt->next;
	}

	return 0;
}