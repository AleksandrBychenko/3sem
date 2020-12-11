#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define MAX_CLIENTS 100

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = /*htonl(INADDR_LOOPBACK); */htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;

    memset(fds, 0, sizeof(fds));
    fds[0].fd = listenfd;
    fds[0].events = POLLIN;

    while (1) {
        printf("wait for events\n");
        if (poll(fds, nfds, -1) < 0) {
            perror("poll");
            return 1;
        }

        char all_mesgs[10000];
        int all_mesgs_len = 0;
        //all_mesgs[0] = 0;

        int current_size = nfds;
        printf("test events [0..%d)\n", current_size);
        for (int i = 0; i < current_size; i++) {
            if (fds[i].revents == 0) {
                continue;
            }
            if (fds[i].revents != POLLIN) {
                printf("error on fds %d, exiting\n", i);
                return 1;
            }
            printf("got event at %d\n", i);
            if (i == 0) {
                fds[nfds].fd = accept(listenfd, (struct sockaddr *) NULL, NULL);
                printf(" added accepted as %d\n", nfds);
                fds[nfds].events = POLLIN;
                nfds++;
            } else {
                int sockfd = fds[i].fd;
                int n;

                // check all_mesgs_len < sizeof(all_mesgs)
                while ((n = read(sockfd, &all_mesgs[all_mesgs_len++], 1)) == 1) {
                    if(all_mesgs[all_mesgs_len-1] == '\n') {
                        break;
                    }
                }

                if(n<=0) {
                    printf("fail to recv, closing %d\n", i);
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
            }
        }
        if(all_mesgs_len) {
            for (int i = 1; i < nfds; i++) {
                if (fds[i].fd < 0)
                    continue;

                if (send(fds[i].fd, all_mesgs, all_mesgs_len, 0) < 0) {
                    printf("fail to send, closing %d\n", i);
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
            }
        }

    }
}
