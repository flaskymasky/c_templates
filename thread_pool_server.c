
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>

#define handle(str) \
  do { perror(str);exit(EXIT_FAILURE);}while(0);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t job_mutex = PTHREAD_MUTEX_INITIALIZER;

void xlock() { if (pthread_mutex_lock(&mutex) != 0) handle("mutex_lock"); }
void xunlock() { if (pthread_mutex_unlock(&mutex) != 0) handle("mutex_unlock"); }

static int queue[256];
static int queue_index = 0;

void work(int sockfd)
{
     write(sockfd, "hello\n", sizeof("hello\n"));

     close(sockfd);
}

void *run_job(void *arg)
{
     int job;
     for (;;) {

	  job = -10;

	  xlock();

	  if (queue_index > 0) {
	       
	       job = queue[0];

	       for (int x = 0; x < queue_index - 1; x++)
		    queue[x] = queue[x+1];
	       
	       queue_index--;
	  }
	  
	  xunlock();

	  if (job >= 0)
	       work(job);
     }

     pthread_exit(NULL);
}

void *add_job(void *arg)
{
  int *listenfd = (int*)arg;
  int clientfd;
  
  for (;;) {

  A:
    if ((clientfd = accept(*listenfd, NULL, NULL)) == -1) {
      if (errno == EINTR)
	goto A;
      else
	handle("accept");
    } else {
	 xlock();
	 queue[queue_index++] = clientfd;
	 xunlock();
    }

	
  }
 pthread_exit(NULL);
 
}
int main(int argc, char **argv)
{
  int listenfd;
  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    handle("socket");
  if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    handle("bind");
  if (listen(listenfd, 5) == -1)
    handle("listen");

  
  
  pthread_t th[8];

  if (pthread_create(&th[7], NULL, add_job, (void*)&listenfd) != 0)
    handle("pthread_create: add_job");

  for (int x = 0; x < 7; x++)
    if (pthread_create(&th[x], NULL, run_job, NULL) != 0)
      handle("pthread_create");

  for (int y = 0; y < 7; y++)
    if (pthread_join(th[y], NULL) != 0)
      handle("pthread_join");

  

  pthread_exit(NULL);
  
}
