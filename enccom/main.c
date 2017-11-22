#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include "packet_handle.h"
#include "device.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("test");
MODULE_DESCRIPTION("An encrypted communication module with netfilter hooks");

//global variable
static struct nf_hook_ops li_hook;

struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = device_wirte
};

//hook function
static unsigned int li_hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	handle_packet(skb);
	
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

	nf_register_hook(&li_hook);
	register_chrdev(ENCCOM_MAJOR_NUMBER, "/dev/enccom", &fops);

	return 0;
}

static void __exit enccom_cleanup(void)
{
	nf_unregister_hook(&li_hook);
	unregister_chrdev(ENCCOM_MAJOR_NUMBER, "/dev/enccom");
	free_opts();

	printk(KERN_INFO "Cleaning up enccom!\n");
}


module_init(enccom_init);
module_exit(enccom_cleanup);
