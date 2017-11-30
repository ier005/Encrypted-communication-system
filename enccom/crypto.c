#include "crypto.h"

int crypt(int type, unsigned char *buf, unsigned int len, unsigned char *key)
{
	struct crypto_blkcipher *aes;
	struct blkcipher_desc desc;
	struct scatterlist sg;
	unsigned int remainder = len % 16;
	unsigned int rlen = len - remainder;
	int i;

	aes = crypto_alloc_blkcipher("ecb(aes-generic)", 0, 0);
	if (IS_ERR(aes)) {
		printk("could not allocate skcipher handle");
		return 0;
	}
	desc.tfm = aes;
	desc.flags = 0;
	crypto_blkcipher_setkey(aes, key, 16);
	sg_init_one(&sg, buf, rlen);
	if (sg_copy_from_buffer(&sg, 1, buf, rlen) == 0) {
		printk("failed to copy from buffer");
		return 0;
	}
	if (type == ENCCOM_ENCRYPT) {
		crypto_blkcipher_encrypt(&desc, &sg, &sg, rlen);
		if (sg_copy_to_buffer(&sg, 1, buf, rlen) == 0) {
			printk("failed to copy to buffer");
			return 0;
		}
		for (i = 0; i < remainder; i++) {
			buf[rlen + i] ^= key[i];					
		}
	} else {
		crypto_blkcipher_decrypt(&desc, &sg, &sg, rlen);
		if (sg_copy_to_buffer(&sg, 1, buf, rlen) == 0) {
			printk("failed to copy to buffer");
			return 0;
		}
		for (i = 0; i < remainder; i++) {
			buf[rlen + i] ^= key[i];
		}
	}
	
	crypto_free_blkcipher(aes);
	return 0;
}