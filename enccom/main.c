#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include "packet_handle.h"
#include "device.h"

#include <linux/crypto.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("test");
MODULE_DESCRIPTION("An encrypted communication module with netfilter hooks");

//global variable
static struct nf_hook_ops li_hook;
static struct nf_hook_ops lo_hook;

struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = device_wirte
};

struct crypto_blkcipher *aes;
struct blkcipher_desc desc;
struct scatterlist sg;
char data[17] = "abcdefghijklmnop";

//hook function
static unsigned int li_hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	handle_packet_in(skb);
	
	return NF_ACCEPT;
}

static unsigned int lo_hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	handle_packet_out(skb);

	return NF_ACCEPT;
}

//init and exit
static int __init enccom_init(void)
{
	printk(KERN_INFO "Init enccom!\n");
	li_hook.hook = li_hook_func;
	li_hook.pf = PF_INET;
	li_hook.hooknum = NF_INET_LOCAL_IN;
	li_hook.priority = NF_IP_PRI_FIRST;

	lo_hook.hook = lo_hook_func;
	lo_hook.pf = PF_INET;
	lo_hook.hooknum = NF_INET_LOCAL_OUT;
	lo_hook.priority = NF_IP_PRI_FIRST;

	nf_register_hook(&li_hook);
	nf_register_hook(&lo_hook);
	register_chrdev(ENCCOM_MAJOR_NUMBER, "/dev/enccom", &fops);

	aes = crypto_alloc_blkcipher("ecb(aes-generic)", 0, 0);
	if (IS_ERR(aes)) {
		printk("could not allocate skcipher handle");
		return 0;
	}
	desc.tfm = aes;
	desc.flags = 0;
	crypto_blkcipher_setkey(aes, "0123456789abcdef", 16);
	li_hook.hook = li_hook_func;
	sg_init_one(&sg, data, 16);
	if (sg_copy_from_buffer(&sg, 1, data, 16) == 0) {
		printk("failed to copy from buffer");
		return 0;
	}
	printk("1 %s\n", data);
	crypto_blkcipher_encrypt(&desc, &sg, &sg, 16);
	if (sg_copy_to_buffer(&sg, 1, data, 16) == 0) {
		printk("failed to copy to buffer");
		return 0;
	}
	printk("2 %s\n", data);
	crypto_blkcipher_decrypt(&desc, &sg, &sg, 16);
	if (sg_copy_to_buffer(&sg, 1, data, 16) == 0) {
		printk("failed to copy to buffer");
		return 0;
	}
	printk("3 %s\n", data);


	return 0;
}

static void __exit enccom_cleanup(void)
{
	nf_unregister_hook(&li_hook);
	nf_unregister_hook(&lo_hook);
	unregister_chrdev(ENCCOM_MAJOR_NUMBER, "/dev/enccom");
	free_opts();

	crypto_free_blkcipher(aes);

	printk(KERN_INFO "Cleaning up enccom!\n");
}


module_init(enccom_init);
module_exit(enccom_cleanup);
