#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>

int main( int argc, char **argv)
{
	mqd_t mqd = mq_open(argv[1], O_WRONLY | O_CREAT, 0644);
	if ( mqd == -1 ) {
		perror("");
		exit(1);
	}

	if ( mq_send(mqd, argv[2], strlen(argv[2]), 10) == -1 ) {
		perror("");
		exit(1);
	}

	puts("done");

	mq_close(mqd);

	return 0;

}
