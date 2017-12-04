#include "device.h"


ssize_t device_wirte(struct file *fd, const char __user *buf, size_t len, loff_t *ppos)
{
	char cont[OPT_LEN];

	if (len > OPT_LEN)
		return 0;

	if (copy_from_user(cont, buf, len) != 0)
		return 0;

	//operation1, io1, id4, type1, ip4, key_len1, key
	u_int8_t oper = cont[0];
	if (oper == MODULE_CONT) {
		if (mod_running)
			mod_running = 0;
		else
			mod_running = 1;
		return len;
	}
	u_int8_t io = cont[1];
	u_int32_t id = *(u_int32_t *)(cont + 2);

	if (oper != OPT_DEL) {
		u_int8_t type = cont[6];
		__be32 ip = *(__be32 *)(cont + 7);
		u_int8_t key_len = cont[11];
		unsigned char *iv = kmalloc(16, GFP_KERNEL);
		unsigned char *key = kmalloc(key_len, GFP_KERNEL);
		if (!key || !iv)
			return 0;
		memcpy(key, cont + 12, key_len);
		memset(iv, 0, 16);

		if (oper == OPT_ADD)
			add_opt(io, id, type, ip, key, iv);
		else
			mod_opt(io, id, type, ip, key, iv);
	} else {
		del_opt(io, id);
	}

	return len;
}