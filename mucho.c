#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define handle(str)                                                  \
    do {                                                             \
        perror(str);                                                 \
        exit(EXIT_FAILURE);                                          \
    } while (0);

int main(int argc, char **argv)
{
    int sockfd, connfd;
    pid_t pid;

    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[1]));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        handle("socket");

    if (bind(sockfd, (struct sockaddr *)&servaddr,
             sizeof(servaddr)) == -1)
        handle("bind");

    if (listen(sockfd, 1024) == -1)
        handle("listen");

    if ((connfd = accept(sockfd, NULL, NULL)) == -1)
        handle("accept");

    if ((pid = fork()) == 0) {
        close(sockfd);
        dup2(connfd, 0);
        dup2(connfd, 1);
        dup2(connfd, 2);
        execl("/bin/emacs", "emacs", NULL);
        
        perror("FAIL");
        exit(1);
    }

    close(connfd);

    return 0;
}
