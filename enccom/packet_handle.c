#include "packet_handle.h"

// calculate the checksum if protocol is tcp or udp
// change the protocol in ip header
static unsigned char tcpudp_csum(struct sk_buff *skb, unsigned char eproto, int lay4_len)
{
	unsigned char proto;
	struct iphdr *ipheader = ip_hdr(skb);

	if (ipheader->protocol == IPPROTO_TCP) {
		struct tcphdr *tcph = tcp_hdr(skb);
		tcph->check = 0;
		skb->csum = csum_partial((unsigned char *)tcph, lay4_len, 0);
		tcph->check = csum_tcpudp_magic(ipheader->saddr, ipheader->daddr, lay4_len, ipheader->protocol, skb->csum);
	}
	if (ipheader->protocol == IPPROTO_UDP) {
		struct udphdr *udph = udp_hdr(skb);
		udph->check = 0;
		skb->csum = csum_partial((unsigned char *)udph, lay4_len, 0);
		udph->check = csum_tcpudp_magic(ipheader->saddr, ipheader->daddr, lay4_len, ipheader->protocol, skb->csum);
	}

	skb->ip_summed = CHECKSUM_NONE;

	proto = ipheader->protocol;
	ipheader->protocol = eproto;

	return proto;
}


// add padding data, the real leagth of the ip packet and
// the real protocol to the tail of the skb
static int packet_out_pre_handle(struct sk_buff *skb, int padlen)
{
	struct iphdr *ipheader = ip_hdr(skb);
	int tot_len = ntohs(ipheader->tot_len);
	int lay4_len = tot_len - ip_hdrlen(skb);
	int nfrags;
	struct sk_buff *trailer;
	u8 *tail;
	unsigned char proto;

	proto = tcpudp_csum(skb, ENC_PROTO, lay4_len);

	if ((nfrags = skb_cow_data(skb, padlen, &trailer)) < 0)
		return nfrags;
	else
		ipheader = ip_hdr(skb);


	tail = skb_tail_pointer(trailer);
	memset(tail, 0, padlen - 3);
	memcpy(tail + padlen - 3, &(tot_len), 2);
	memcpy(tail + padlen - 1, &proto, 1);
	pskb_put(skb, trailer, padlen);
	ipheader->tot_len = htons(tot_len + padlen);
	ipheader->check = 0;
	ip_send_check(ipheader);
	return nfrags;

}


// check whether the packet should be encrypted, 
// add the padding data and encrypted
int handle_packet_out(struct sk_buff *skb)
{
	if (!mod_running)
		return 0;
	struct iphdr *ipheader = ip_hdr(skb);
	struct option *opt = opt_out_head;
	

	while (opt) {
		if (opt->ip == ipheader->daddr) {
			struct crypto_skcipher *skcipher = opt->cipher;
			struct skcipher_request *req;
			struct scatterlist *sg;
			int nfrags;

			unsigned char *buf = skb_network_header(skb) + ipheader->ihl * 4;
			int offset = buf - skb->data;
			//int alen = 0;
			int blksize = crypto_skcipher_blocksize(skcipher);
			int clen = ALIGN(skb->len - offset + 3, blksize);
			int padlen = clen - (skb->len - offset);

			if ((nfrags = packet_out_pre_handle(skb, padlen)) < 0)
				return 0;
			
			req = skcipher_request_alloc(skcipher, GFP_KERNEL);
			sg = kmalloc(sizeof(struct scatterlist) * nfrags, GFP_KERNEL);
			sg_init_table(sg, nfrags);
			skb_to_sgvec(skb, sg, offset, clen);

			skcipher_request_set_crypt(req, sg, sg, clen, opt->iv);
			crypto_skcipher_encrypt(req);


			kfree(sg);
			skcipher_request_free(req);

			break;
		}
		opt = opt->next;
	}

	return 0;
}


// check whether the packet should be decrypted, 
// decrypted, trim the padding data and recover the ip protocol and tot_len
int handle_packet_in(struct sk_buff *skb)
{
	if (!mod_running)
		return 0;
	struct iphdr *ipheader = ip_hdr(skb);
	struct option *opt = opt_in_head;


	while (opt) {
		if (opt->ip == ipheader->saddr) {
			struct crypto_skcipher *skcipher = opt->cipher;
			struct skcipher_request *req;
			struct scatterlist *sg;
			int nfrags;
			struct sk_buff *trailer;
			unsigned char proto;

			unsigned char *buf = skb_network_header(skb) + ipheader->ihl * 4;
			int offset = buf - skb->data;
			int tot_len = ntohs(ipheader->tot_len);
			int lay4_len = tot_len - ip_hdrlen(skb);
			u16 rlen;

			if ((nfrags = skb_cow_data(skb, 0, &trailer)) < 0)
				return 0;
			else
				ipheader = ip_hdr(skb);

			skb->ip_summed = CHECKSUM_NONE;
			req = skcipher_request_alloc(skcipher, GFP_KERNEL);
			sg = kmalloc(sizeof(struct scatterlist) * nfrags, GFP_KERNEL);
			sg_init_table(sg, nfrags);
			skb_to_sgvec(skb, sg, offset, lay4_len);

			skcipher_request_set_crypt(req, sg, sg, lay4_len, opt->iv);
			crypto_skcipher_decrypt(req);

			skb_copy_bits(skb, skb->len - 3, &rlen, 2);
			skb_copy_bits(skb, skb->len - 1, &proto, 1);
			ipheader->tot_len = htons(rlen);
			ipheader->protocol = proto;
			ipheader->check = 0;
			ip_send_check(ipheader);

			pskb_trim(skb, skb->len - (tot_len - rlen));


			kfree(sg);
			skcipher_request_free(req);

			break;
		}
		opt = opt->next;
	}

	return 0;
}