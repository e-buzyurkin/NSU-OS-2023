#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

char *socket_path = "server_socket";

void sigpipe_handler(int signum) {
    write(1, "server closed\n", 14);
    exit(0);
}

int main() {
    sigset(SIGPIPE, sigpipe_handler);

    int sockfd, bytes_read;

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }
    write(1, "client socket created\n", 22);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socket_path);

    if (connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        perror("connect error");
        exit(1);
    }
    write(1, "connect successful\n", 19);

    char buf[BUFSIZ];
    while ((bytes_read = read(0, buf, BUFSIZ)) > 0) {
        write(sockfd, buf, bytes_read);
    }

    return 0;
}