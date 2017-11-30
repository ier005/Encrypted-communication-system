#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define OPT_IN 0
#define OPT_OUT 1

#define ECB_AES 0

int main(int argc, char **argv) {
	char opt[27];
	int fd;
	char key[17] = "0123456789abcdef";

	if (argc != 3) {
		printf("Usage: ./main ip io\n");
		return 1;
	}
	printf("%s\n", argv[1]);

	unsigned char io;
	if (argv[2][0] == '1')
		io = OPT_OUT;
	else
		io = OPT_IN;
	unsigned int id = 1;
	unsigned char type = 1;
	unsigned int ipn = inet_addr(argv[1]);
	unsigned char key_len = 16;
	memcpy(opt, &io, OPT_OUT);
	memcpy(opt + 1, &id, 4);
	memcpy(opt + 5, &type, 1);
	memcpy(opt + 6, &ipn, 4);
	memcpy(opt + 10, &key_len, 1);
	memcpy(opt + 11, key, key_len);

	if ((fd = open("/dev/enccom", O_WRONLY, S_IWUSR)) == -1) {
		printf("Failed to open file\n");
		return 1;
	}

	write(fd, opt, sizeof(opt));
	close(fd);

	return 0;
}
