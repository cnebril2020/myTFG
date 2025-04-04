#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 256

int
main(int argc, char const *argv[])
{
	FILE *fp;
	char line[MAXLINE];

	fp = fopen("/proc/net/tcp", "r");
	if (fp == NULL) {
		perror("fopen");
		return 0;
	}

	while (fgets(line, sizeof(line), fp)) {
		/* code */
		printf("%s", line);
	}

	fclose(fp);

	return 0;
}
