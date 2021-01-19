#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#define handle(msg) \
	do { perror(msg);exit(EXIT_FAILURE); }while(0); 

// READ to shared virtual memory posix

#define PAGE sysconf(_SC_PAGESIZE)

int main(int argc, char **argv)
{
	char file_name[256];
	sprintf(file_name, "/%s", argv[1]);

	int shm_id = shm_open(file_name, O_RDWR | O_CREAT, 0644);
	if ( shm_id == -1 ) handle("shm_open");


	char *data = (char*) mmap(0, PAGE, PROT_READ, MAP_SHARED, shm_id, 0);
	if ( !data ) handle("mmap");
	

	printf("data: %s\n", data);


	if ( munmap(data, PAGE) == -1 )
		handle("munmap");

	close(shm_id);
		
	sem_unlink(file_name)

	return 0;
}
