#ifndef ENCCOM_OPTIONS_H
#define ENCCOM_OPTIONS_H

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/scatterlist.h>
#include <crypto/skcipher.h>

#define OPT_LEN 128

#define OPT_IN 0
#define OPT_OUT 1

#define MODULE_CONT 0
#define OPT_ADD 1
#define OPT_MOD 2
#define OPT_DEL 3

extern struct option *opt_in_head, *opt_in_tail, *opt_out_head, *opt_out_tail;
extern int mod_running;

struct option
{
	u_int32_t id;
	u_int8_t type;
	__be32 ip;
	struct crypto_skcipher *cipher;	
	unsigned char *key;
	unsigned char *iv;
	struct option *next;
	struct option *prev;
};


void add_opt(u_int8_t io, u_int32_t id, u_int8_t type, __be32 ip, unsigned char *key, unsigned char *iv);
void mod_opt(u_int8_t io, u_int32_t id, u_int8_t type, __be32 ip, unsigned char *key, unsigned char *iv);
void del_opt(u_int8_t io, u_int32_t id);

void free_opts(void);

#endif