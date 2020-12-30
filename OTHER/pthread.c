#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>


// ###########################################
// compile with : gcc -o name name.c -lpthread
// ###########################################



// make it global ( visible to all threads )

static int count = 0;

pthread_mutex_t mutex_1, mutex_2;
pthread_cond_t cond;


void *first( void *a)
{
	pthread_mutex_lock(&mutex_1);

	for ( int x = 0; x < 10; x++ )
	{
		printf("count: %d\tid: %ld\n", count, pthread_self());
		count++;
		usleep(300000);
	}

	// tell sleep cond im finished
	pthread_cond_signal(&cond);

	pthread_mutex_unlock(&mutex_1);

	pthread_exit(NULL); // to return value, instead of NULL : pthread_exit( (void*) argument );
}

void *second( void *a)
{
	pthread_mutex_lock(&mutex_2);

	// wait for thread "first" to wake me up
	pthread_cond_wait(&cond, &mutex_2);

	for ( int x = 0; x < 10; x++ )
	{
		printf("count: %d\tid: %ld\n", count, pthread_self());
		count++;
		usleep(300000);
	}

	pthread_mutex_unlock(&mutex_2);

	pthread_exit(NULL);
}



int main()
{
	int status_first, status_second;
	pthread_t t1, t2;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_mutex_init(&mutex_1, NULL);
	pthread_mutex_init(&mutex_2, NULL);
	pthread_cond_init(&cond, NULL);

	// set joinable or detached ( wait or no wait for other threads )
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


	status_first = pthread_create(&t1, &attr, first, NULL); // if arg then : ( &t1, &attr, first, (void*) arg);
	status_second = pthread_create(&t2, &attr, second, NULL);

	if ( (status_first || status_second) != 0 )
	{
		perror("pthread_create(): ");
		exit(1);
	}

	pthread_join(t1, NULL);
	pthread_join(t2, NULL); // if not NULL then receive return value from thread , its (void*) cast it to your needs


	// clear up
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex_1);
	pthread_mutex_destroy(&mutex_2);
	pthread_cond_destroy(&cond);


	// wait for all threads to finish
	pthread_exit(NULL);

}
	




