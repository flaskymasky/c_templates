#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

// create buffer for msg
struct msg_buffer
{
	long mtype;
	char mtext[1000];
};

int main()
{
	int msq_id;

	// get basic info
	struct msqid_ds info;

	// get more kernel info
	struct msginfo info_msg;

	//create msg_buffer;
	struct msg_buffer msg_buf;

	//create key
	key_t key = ftok("/tmp", 'a');
	if ( key == (key_t) -1 )
	{
		perror("");
		exit(1);
	}

	// open or create msg queue 
	if ( (msq_id = msgget(key, 0644 | IPC_CREAT)) == -1 )
	{
		perror("");
		exit(1);
	}

	// get basic info from msg queue
	if ( msgctl(msq_id, IPC_STAT, &info) == -1 )
	{
		perror("");
		exit(1);
	}

	// get more specific info from msg queue # note cast to ( struct msqid_ds *)
	if ( msgctl(msq_id, MSG_STAT, (struct msqid_ds *)&info_msg) == -1 )
	{
		perror("");
		exit(1);
	}

	// receive msg					      all , truncate is to long
	if ( msgrcv(msq_id, &msg_buf, sizeof((msg_buf.mtext)), 0, MSG_NOERROR) == -1 )
	{
		perror("");
		exit(1);
	}

	printf("Received: %s\n", msg_buf.mtext);


	return 0;
}


