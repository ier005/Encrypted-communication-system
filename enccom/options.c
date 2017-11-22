#include "options.h"

struct option *option_head, *option_tail;

void add_opt(u_int32_t id, u_int8_t type, __be32 ip)
{
	struct option *opt;
	if ((opt = kmalloc(sizeof(struct option), GFP_KERNEL)) == NULL)
		return;

	opt->id = id;
	opt->type = type;
	opt->ip = ip;

	if (option_tail) {
		option_tail->next = opt;
		opt->prev = option_tail;
		opt->next = 0;
		option_tail = opt;
	} else {
		option_head = option_tail = opt;
		opt->next = 0;
		opt->prev = 0;
	}

}

void del_opt(u_int32_t id)
{
	struct option *opt = option_head;
	
	while (opt != NULL && opt->id != id)
		opt = opt->next;
	if (!opt)
		return;

	if (opt->next)
		opt->next->prev = opt->prev;
	else
		option_tail = opt->prev;

	if (opt->prev)
		opt->prev->next = opt->next;
	else
		option_head = opt->next;

	kfree(opt);
}

void free_opts(void)
{
	struct option *opt = option_head;
	while (opt) {
		kfree(opt);
		opt = opt->next;
	}
}