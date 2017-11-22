#ifndef ENCCOM_OPTIONS_H
#define ENCCOM_OPTIONS_H

#include <linux/types.h>
#include <linux/slab.h>

#define OPT_LEN 128

struct option
{
	u_int32_t id;
	u_int8_t type;
	__be32 ip;
	struct option *next;
	struct option *prev;
};


void add_opt(u_int32_t id, u_int8_t type, __be32 ip);
void del_opt(u_int32_t id);

void free_opts(void);

#endif