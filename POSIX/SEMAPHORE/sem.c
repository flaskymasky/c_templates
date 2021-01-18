#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <semaphore.h>

int main()
{
	sem_t sem;


	sem_init(&sem, 0, 1);
	// or
	// sem_t *sem = sem_open("/name", O_RDWR | O_CREAT, 0644, 1);

	
	sem_wait(&sem);

	puts("protected");

	sem_post(&sem);


	sem_destroy(&sem);

	

	// or sem_close("/name");
	// sem_unlink(const *char);



	return 0;
}
	
