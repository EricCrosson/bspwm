#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "common.h"

int main(int argc, char *argv[])
{
    int sock_fd, nbr, i;
    struct sockaddr_un sock_address;
    char *socket_path;
    char msg[BUFSIZ];
    char rsp[BUFSIZ];

    if (argc < 2)
        return -1;

    socket_path = getenv(SOCKET_ENV_VAR);

    if (socket_path == NULL)
        return -1;

    msg[0] = '\0';

    for (i = 1; i < argc; i++) {
        strcat(msg, argv[i]);
        if (i < (argc - 1))
            strcat(msg, TOKEN_SEP);
    }

    sock_address.sun_family = AF_UNIX;
    strcpy(sock_address.sun_path, socket_path);

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    connect(sock_fd, (struct sockaddr *) &sock_address, sizeof(sock_address));

    send(sock_fd, msg, strlen(msg), 0);

    if ((nbr = recv(sock_fd, rsp, sizeof(rsp), 0)) > 0) {
        rsp[nbr] = '\0';
        if (strcmp(rsp, EMPTY_RESPONSE) != 0)
            printf("%s", rsp);
    }

    close(sock_fd);
    return 0;
}