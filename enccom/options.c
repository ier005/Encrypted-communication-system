#include "options.h"

int mod_running;
struct option *opt_in_head, *opt_in_tail, *opt_out_head, *opt_out_tail;

void add_opt(u_int8_t io, u_int32_t id, u_int8_t type, __be32 ip, unsigned char *key, unsigned char *iv)
{
	struct option *opt, **option_head, **option_tail;
	if ((opt = kmalloc(sizeof(struct option), GFP_KERNEL)) == NULL)
		return;

	if (io == OPT_OUT) {
		option_head = &opt_out_head;
		option_tail = &opt_out_tail;
	} else {
		option_head = &opt_in_head;
		option_tail = &opt_in_tail;
	}

	opt->id = id;
	opt->type = type;
	opt->ip = ip;
	opt->key = key;
	opt->iv = iv;

	switch (type) {
		case 0:
			opt->cipher = crypto_alloc_skcipher("ecb(aes-generic)", 0, 0);
			crypto_skcipher_setkey(opt->cipher, key, 16);
			break;
		case 1:
			opt->cipher = crypto_alloc_skcipher("cbc(aes)", 0, 0);
			crypto_skcipher_setkey(opt->cipher, key, 16);
			break;
		case 2:
			opt->cipher = crypto_alloc_skcipher("xts(aes)", 0, 0);
			crypto_skcipher_setkey(opt->cipher, key, 16);
			break;
		case 3:
			opt->cipher = crypto_alloc_skcipher("ctr(aes)", 0, 0);
			crypto_skcipher_setkey(opt->cipher, key, 16);
			break;
		default:
			break;
	}

	if (*option_tail) {
		(*option_tail)->next = opt;
		opt->prev = *option_tail;
		opt->next = 0;
		*option_tail = opt;
	} else {
		*option_head = *option_tail = opt;
		opt->next = 0;
		opt->prev = 0;
	}

}

void mod_opt(u_int8_t io, u_int32_t id, u_int8_t type, __be32 ip, unsigned char *key, unsigned char *iv)
{
	struct option *opt;

	if (io == OPT_OUT)
		opt = opt_out_head;
	else
		opt = opt_in_head;

	while (opt != NULL && opt->id != id)
		opt = opt->next;
	if (!opt)
		return;

	opt->type = type;
	opt->ip = ip;
	kfree(opt->iv);
	opt->iv = iv;
	kfree(opt->key);
	opt->key = key;
	crypto_free_skcipher(opt->cipher);
	switch (type) {
		case 0:
			opt->cipher = crypto_alloc_skcipher("ecb(aes-generic)", 0, 0);
			crypto_skcipher_setkey(opt->cipher, key, 16);
			break;
		case 1:
			opt->cipher = crypto_alloc_skcipher("cbc(aes)", 0, 0);
			crypto_skcipher_setkey(opt->cipher, key, 16);
			break;
		case 2:
			opt->cipher = crypto_alloc_skcipher("xts(aes)", 0, 0);
			crypto_skcipher_setkey(opt->cipher, key, 16);
			break;
		case 3:
			opt->cipher = crypto_alloc_skcipher("ctr(aes)", 0, 0);
			crypto_skcipher_setkey(opt->cipher, key, 16);
			break;
		default:
			break;
	}	

}

void del_opt(u_int8_t io, u_int32_t id)
{
	struct option *opt;
	struct option **option_head, **option_tail;
	if (io == OPT_OUT) {
		opt = opt_out_head;
		option_head = &opt_out_head;
		option_tail = &opt_out_tail;
	} else {
		opt = opt_in_head;
		option_head = &opt_in_head;
		option_tail = &opt_in_tail;
	}
	
	while (opt != NULL && opt->id != id)
		opt = opt->next;
	if (!opt)
		return;

	if (opt->next)
		opt->next->prev = opt->prev;
	else
		*option_tail = opt->prev;

	if (opt->prev)
		opt->prev->next = opt->next;
	else
		*option_head = opt->next;

	kfree(opt->iv);
	kfree(opt->key);
	crypto_free_skcipher(opt->cipher);
	kfree(opt);
}

void free_opts(void)
{
	struct option *opt = opt_in_head;
	while (opt) {
		kfree(opt->iv);
		kfree(opt->key);
		crypto_free_skcipher(opt->cipher);
		kfree(opt);
		opt = opt->next;
	}
	opt = opt_out_head;
	while (opt) {
		kfree(opt->iv);
		kfree(opt->key);
		crypto_free_skcipher(opt->cipher);
		kfree(opt);
		opt = opt->next;
	}
}