#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>


pthread_mutex_t mutex;

int count = 0;

void *first( void *a)
{
	for ( int x = 0; x < 250000; x++ )
	{
		int s = pthread_mutex_lock(&mutex);
		if ( s == EOWNERDEAD )
		{
			if ( pthread_mutex_consistent(&mutex) != 0 )
			{
				perror("");
				exit(1);
			}
			if ( pthread_mutex_unlock(&mutex) != 0 )
			{
				perror("");
				exit(1);
			}
			else {
				perror("");
				exit(1);
			}

		}
		if ( s ==  0 )
		{
			count++;
			if ( pthread_mutex_unlock(&mutex) != 0 ) {
				perror("");
				exit(1);
			}
		}

	}
}
void *second( void *a)
{
	for ( int x = 0; x < 250000; x++ )
	{
		int s = pthread_mutex_lock(&mutex);
		if ( s == EOWNERDEAD )
		{
			if ( pthread_mutex_consistent(&mutex) != 0 )
			{
				perror("");
				exit(1);
			}
			if ( pthread_mutex_unlock(&mutex) != 0 )
			{
				perror("");
				exit(1);
			}
			else {
				perror("");
				exit(1);
			}

		}
		if ( s ==  0 )
		{
			count++;
			if ( pthread_mutex_unlock(&mutex) != 0 ) {
				perror("");
				exit(1);
			}
		}

	}
}


int main()
{
	pthread_t t1, t2;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);

	pthread_mutex_init(&mutex, &attr);

	pthread_create(&t1, NULL, first, NULL);
	pthread_create(&t2, NULL, second, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("count: %d\n", count);

	pthread_mutex_destroy(&mutex);
	pthread_mutexattr_destroy(&attr);
	pthread_exit(NULL);

}
