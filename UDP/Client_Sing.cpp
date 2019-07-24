#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>

using namespace std;

/*
readme:
    socket -> sendto -> recvfrom -> close
*/

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("The input format must be : %s [SERVER-IP] [SERVER-PORT].\n", argv[0]);
        exit(-1);
    }
    //1.
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in ser_addr;
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ser_addr.sin_port = htons(atoi(argv[2]));

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    struct sockaddr_in loc_addr;
    while (1)
    {
        socklen_t len = sizeof(ser_addr);
        printf("Please Entry # ");
        scanf("%s", buf);
        int s = sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *)&ser_addr, len);
        if (s > 0)
        {
            buf[s] = 0;
            printf("Client Say # %s\n", buf);
        }
        recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&loc_addr, &len);
        printf("Server Say # %s.\n", buf);
    }
    close(sock);
    return 0;
}