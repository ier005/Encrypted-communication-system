#ifndef ENCCOM_CRYPTO_H
#define ENCCOM_CRYPTO_H

#include <linux/scatterlist.h>
#include <linux/crypto.h>

#define ENCCOM_ENCRYPT 0
#define ENCCOM_DECRYPT 1

int crypt(int type, unsigned char *buf, unsigned int len);

#endif