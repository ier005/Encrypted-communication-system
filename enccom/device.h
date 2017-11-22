#ifndef ENCCOM_DEVICE_H
#define ENCCOM_DEVICE_H

#include <linux/uaccess.h>
#include "options.h"

#define ENCCOM_MAJOR_NUMBER 124

ssize_t device_wirte(struct file *fd, const char __user *buf, size_t len, loff_t *ppos);

#endif