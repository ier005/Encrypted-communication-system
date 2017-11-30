#include "device.h"


ssize_t device_wirte(struct file *fd, const char __user *buf, size_t len, loff_t *ppos)
{
	char cont[OPT_LEN];

	if (len > OPT_LEN)
		return 0;

	if (copy_from_user(cont, buf, len) != 0)
		return 0;

	//io1, id4, type1, ip4, key_len1, key
	u_int8_t io = cont[0];
	u_int32_t id = *(u_int32_t *)(cont + 1);
	u_int8_t type = cont[5];
	__be32 ip = *(__be32 *)(cont + 6);
	u_int8_t key_len = cont[10];
	unsigned char *key = kmalloc(key_len, GFP_KERNEL);
	if (!key)
		return 0;
	memcpy(key, cont + 11, key_len);
	add_opt(io, id, type, ip, key);

	return len;
}