#ifndef ENCCOM_DEVICE_H
#define ENCCOM_DEVICE_H

#include <linux/uaccess.h>
#include "options.h"

#define ENCCOM_MAJOR_NUMBER 124

#define MODULE_CONT 0
#define OPT_ADD 1
#define OPT_MOD 2
#define OPT_DEL 3

ssize_t device_wirte(struct file *fd, const char __user *buf, size_t len, loff_t *ppos);

#endif