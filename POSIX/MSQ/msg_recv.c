#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>

static void tfunc( union sigval sv )
{
	mqd_t mqd = *( (mqd_t *) (sv.sival_ptr) );

	struct mq_attr attr;
	memset(&attr, 0, sizeof(attr));

	if ( mq_getattr(mqd, &attr) == -1 ) {
		perror("");
		exit(1);
	}

	struct sigevent sev;
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = tfunc;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = sv.sival_ptr;

	if ( mq_notify(mqd, &sev) == -1 ) {
		perror("");
		exit(1);
	}

	size_t n = attr.mq_msgsize + 1;
	
	char *msg = malloc ( n * sizeof(char) );

	if ( mq_receive(mqd, msg, n, NULL) == -1 ) {
		perror("");
		exit(1);
	}

	printf("Received: %s\n", msg);

	memset(msg, 0, sizeof(msg));

	free(msg);

	sleep(100);
}
	
	
int main(int argc, char **argv)
{
	struct mq_attr attr;
	memset(&attr, 0, sizeof(attr));
	attr.mq_msgsize = 1024;
	attr.mq_maxmsg = 10;
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;


	mqd_t mqd = mq_open(argv[1], O_RDONLY | O_CREAT, 0644, &attr);	
	if ( mqd == -1 ) {
		perror("");
		exit(1);
	}

	struct sigevent sev;
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = tfunc;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = &mqd;

	if ( mq_notify(mqd, &sev) == -1 ) {
		perror("");
		exit(1);
	}


	sleep(100);

	mq_close(mqd);

	return 0;
}	


