#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

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

	// open or create msg queue container
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
	if ( msgctl(msq_id, MSG_STAT, (struct msqid_ds *)&info) == -1 )
	{
		perror("");
		exit(1);
	}

	// msg type
	msg_buf.mtype = 3;
	strcpy(msg_buf.mtext, "Hello Its me!");

	// send msg
	if ( msgsnd(msq_id, &msg_buf, strlen((msg_buf.mtext)), IPC_NOWAIT) == -1 )
	{
		perror("");
		exit(1);
	}

	puts("msg send.");


	return 0;
}


