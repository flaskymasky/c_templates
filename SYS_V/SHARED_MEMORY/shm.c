#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main()
{
  int shm_id;
  int *data; // char or int

  key_t key = ftok("/tmp", 'c');
  if ( key == (key_t) -1 )
    {
      perror("");
      exit(1);

    }

  // init shm
  if ( (shm_id = shmget(key, 1024, 0644 | IPC_CREAT)) == -1 )
    {
      perror("");
      exit(1);
    }

  // attach
  if ( (data = (int*) shmat(shm_id, NULL, 0)) == (void*) -1 )
    {
      perror("");
      exit(1);
    }

  // set shm var
  *data = 4;


  // detach
  if ( shmdt(data) == -1 )
    {
      perror("");
      exit(1);
    }

  return 0;
}
