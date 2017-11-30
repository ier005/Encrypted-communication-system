#ifndef ENCCOM_OPTIONS_H
#define ENCCOM_OPTIONS_H

#include <linux/types.h>
#include <linux/slab.h>

#define OPT_LEN 128
#define OPT_IN 0
#define OPT_OUT 1

struct option
{
	u_int32_t id;
	u_int8_t type;
	__be32 ip;
	unsigned char *key;
	struct option *next;
	struct option *prev;
};


void add_opt(u_int8_t io, u_int32_t id, u_int8_t type, __be32 ip, unsigned char *key);
void mod_opt(u_int8_t io, u_int32_t id, u_int8_t type, __be32 ip, unsigned char *key);
void del_opt(u_int8_t io, u_int32_t id);

void free_opts(void);

#endif