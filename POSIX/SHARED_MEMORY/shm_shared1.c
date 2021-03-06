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

// WRITES to shared virtual memory posix

#define PAGE sysconf(_SC_PAGESIZE)

int main(int argc, char **argv)
{
	char file_name[256];
	sprintf(file_name, "/%s", argv[1]);

	int shm_id = shm_open(file_name, O_RDWR | O_CREAT, 0644);
	if ( shm_id == -1 ) handle("shm_open");

	if ( ftruncate(shm_id, PAGE) == -1 ) handle("ftruncate");

	char *data = (char*) mmap(0, PAGE, PROT_WRITE, MAP_SHARED, shm_id, 0);
	if ( !data ) handle("mmap");
	

	memcpy(data, "hello maximus", strlen("hello maximus"));


	if ( munmap(data, PAGE) == -1 )
		handle("munmap");

	close(shm_id);
		


	return 0;
}
