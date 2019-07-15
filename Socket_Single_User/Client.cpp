#include <bits/stdc++.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

/*
readme:
    This is cpp can only deal single user
    TCP socket program
    exit:
        -1: the input format is error
         1: socket is error
         2: connect is error
*/

/*
    Server:
        socket -> bind -> listen -> accept(blocking, 3 handshake) -> send
    Client:
        socket -> connect(blocking, 3 handshake) -> rev
*/


class Celient
{
public:

};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("The input format must be: %s [server-ip] [server-port]\n", argv[0]);
        exit(-1);
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);//return 1 
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect");
        exit(2);//return 2
    }
    while (1)//write and send
    {
        printf("Please input information #");
        fflush(stdout);
        char buf[1024];
        ssize_t s = read(0, buf, sizeof(buf) - 1);
        if (s > 0)//read success
        {
            buf[s] = 0;
        }
        write(sock, buf, strlen(buf));
        ssize_t _s = read(sock, buf, sizeof(buf) - 1);
        if (_s > 0)
        {
            buf[_s - 1] = 0;
            printf("Server echo # %s\n", buf);
        }
    }
    close(sock);
    return 0;
}