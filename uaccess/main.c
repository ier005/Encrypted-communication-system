#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	char *ip = "192.168.98.1";
	char opt[9];
	int fd;

	unsigned int id = 1;
	unsigned char type = 1;
	unsigned int ipn = inet_addr(ip);
	memcpy(opt, &id, 4);
	memcpy(opt + 1, &type, 1);
	memcpy(opt + 5, &ipn, 4);

	if ((fd = open("/dev/enccom", O_WRONLY, S_IWUSR)) == -1) {
		printf("Failed to open file");
		return 1;
	}

	write(fd, opt, sizeof(opt));
	close(fd);

	return 0;
}