#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <signal.h>

void start_daemon()
{

  pid_t pid;

  pid = fork();

  if ( pid < 0  ) exit(1);
  if ( pid > 0  ) exit(0);
  if ( setsid() < 0  ) exit(1);

  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);


  if ( pid < 0  ) exit(1);
  if ( pid > 0  ) exit(0);

  umask(0);

  chdir("/");

  for ( int x = sysconf(_SC_OPEN_MAX); x > 0; x-- )
    close(x);

  openlog("dae", LOG_PID, LOG_DAEMON);

}

int main()
{

  start_daemon();

  syslog(0, "Starting..");


  sleep(15);

  syslog(0, "Ending...");

  closelog();

  return 0;

}
