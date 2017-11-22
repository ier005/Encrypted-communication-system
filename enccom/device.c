#include "device.h"


ssize_t device_wirte(struct file *fd, const char __user *buf, size_t len, loff_t *ppos)
{
	char cont[OPT_LEN];

	if (len > OPT_LEN)
		return 0;

	if (copy_from_user(cont, buf, len) != 0)
		return 0;


	u_int32_t id = *(u_int32_t *)cont;
	u_int8_t type = cont[4];
	__be32 ip = *(__be32 *)(cont + 5);
	add_opt(id, type, ip);

	return len;
}