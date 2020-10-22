#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
//		      n-th sem
//		       |
//		       | 
// lock		       |
struct sembuf p[1] = { 0, -1, SEM_UNDO };

// unlock
struct sembuf v[1] = { 0, 1, SEM_UNDO };

#ifdef _SEM_SEMUN_UNDEFINED
union semun {
	int val;

	struct semid_ds *info; // need malloc

	unsigned short *array;

	struct seminfo *__buf; // need malloc
};

#endif

int main()
{
	int sem_id;

	union semun sem_un;

	// create key

	key_t key = ftok("/tmp", 'a');
	if ( key == (key_t) -1 )
	{
		perror("");
		exit(1);
	}

	// open or create semaphore

	if ( ( sem_id = semget(key, 1, 0644 | IPC_CREAT)) == -1 ) // semget( key, n-sems, mode);
	{
		perror("");
		exit(1);
	}

	// set value of semaphore to 1 ( default )

	sem_un.val = 1;
	if ( semctl(sem_id, 0, SETVAL, sem_un) == -1 ) // semctl( sem_id, n-th sem, OPERATION, semun struct);
	{
		perror("");
		exit(1);
	}

	// lock
	if ( semop(sem_id, &p[0], 1) == -1 ) { perror(""); exit(1); }

	// do something

	// unlock
	if ( semop(sem_id, &v[0], 1) == -1 ) { perror(""); exit(1); }

	// remove semaphore

	if ( semctl(sem_id, IPC_RMID, 0) == -1 )
	{
		perror("");
		exit(1);
	}

	return 0;
}



