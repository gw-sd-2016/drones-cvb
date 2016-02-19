#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
	This program reads in coordinates from a pipe and displays them
	on a 2-D coordinate system
**/
int main(int argc, char** argv) {

	printf("Display program started.\n");

	int num, fifo;
	char temp[38];

	if ((fifo = open("/tmp/fifo2", O_RDONLY)) < 0) {
		printf("%s\n", strerror(errno));
		return 0;
	}

	while (true) {

		if ((num = read(fifo, temp, sizeof(temp))) < 0) {
			printf("%s\n", strerror(errno));
		}

		printf("In FIFO2 is %s \n", temp);

	}

}