#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>


void func( union sigval sv)
{
	mqd_t mqd = *( (mqd_t *) sv.sival_ptr);

	struct sigevent sev;

	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = func;
	sev.sigev_notify_attributes = NULL ;
	sev.sigev_value.sival_ptr = &mqd;

	struct mq_attr attr;

	if ( mq_getattr(mqd, &attr) == -1 ) {
		perror("");
		exit(1);
	}
	size_t len = attr.mq_msgsize + 1;
	char *msg = malloc( len * sizeof(char));
	printf("msg size: %d\n", len);

	if ( mq_receive(mqd, msg, len, NULL ) == -1 ) {
		perror("");
		exit(1);
	}

	printf("Received: %s\n", msg);

	memset(msg, 0, sizeof(msg));

	if ( mq_notify(mqd, &sev) == -1 ) {
		perror("");
		exit(1);
	}
	

	sleep(100);

}
int main(int argc, char **argv)
{
	struct sigevent sev;
	struct mq_attr attr;
	int flag;

	
	while ( (flag = getopt(argc, argv, "n:s:")) != -1 )
	{
		switch( flag )
		{
			case 'n':
				attr.mq_maxmsg = atoi(optarg);
				break;
			case 's':
				attr.mq_msgsize = atoi(optarg);
				break;
		}
	}


	mqd_t mqd;
	if ( (mqd = mq_open(argv[optind], O_RDWR | O_CREAT, 0644, &attr)) == -1 ) {
		perror("");
		exit(1);
	}

	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = func;
	sev.sigev_notify_attributes = NULL ;
	sev.sigev_value.sival_ptr = &mqd;

	if ( mq_notify(mqd, &sev) == -1 ) {
		perror("");
		exit(1);
	}

	sleep(100);
	

	return 0;
}

