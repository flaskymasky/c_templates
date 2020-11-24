#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main( int argc, char **argv)
{

  struct flock fl;
  memset(&fl, 0, sizeof(fl));

  fl.l_type = F_WRLCK;
  fl.l_whence = SEEK_SET;
  fl.l_start = 0;
  fl.l_len = 0;
  fl.l_pid = getpid();

  system("mount -o remount,mand /dev/sda3");

  int fd = open(argv[1], O_WRONLY | O_CREAT, 0644);
  if ( fd == -1  ) {
    perror("");
    exit(1);
  }

  if ( fcntl(fd, F_SETLK, &fl) == -1  ) {
    perror("");
    exit(1);
  }

  puts("locked");

  sleep(20);

  fl.l_type = F_UNLCK;
  if ( fcntl(fd, F_SETLK, &fl) == -1  ) {
    perror("");
    exit(1);
  }

  puts("unlocked");

  return 0;


}
