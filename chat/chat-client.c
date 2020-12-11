#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("\n Usage: %s <ip of server> <user name>\n", argv[0]);
        return 1;
    }
    struct sockaddr_in serv_addr;
    char user[1024];
    int user_len = sprintf(user, "[%s] ", argv[2]);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in)) < 0) {
        perror("connect");
        return 1;
    }

    int pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid) {
        while (1) {
            char *msg = NULL;
            size_t len = 0;
            if (getline(&msg, &len, stdin) < 0) {
                perror("getline");
                break;
            }

            if (user_len != send(sockfd, user, user_len, 0)) {
                printf("send failed\n");
                break;
            }

            len = strlen(msg);
            if (len != send(sockfd, msg, len, 0)) {
                printf("send failed\n");
                break;
            }
            free(msg);
        }
        kill(pid, SIGINT);
    } else {
        while (1) {
            char c;
            if (read(sockfd, &c, 1) <= 0) {
                printf("read failed\n");
                break;
            }
            putchar(c);
        }
        kill(getppid(), SIGINT);
    }

    close(sockfd);

    return 0;
}
