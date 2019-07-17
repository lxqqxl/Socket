#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

/*
readme:
    Muti-process for TCP socket
    Client.cpp
    exit:
        -1:The input format is error
         1:Socket is error
         2:Connect is error
*/
/*
socket -> connect -> receive
*/

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("The input format must be: %s [Server-ip] [Server-port]", argv[0]);
        exit(-1);
    }
    //1.socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }
    //2.connect
    struct sockaddr_in Server;
    Server.sin_family = AF_INET;
    Server.sin_addr.s_addr = inet_addr(argv[1]);
    Server.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (struct sockaddr *)&Server, sizeof(Server)) < 0)
    {
        perror("connect");
        exit(2);
    }
    while (1)//due to muti-process, here add while(1), muti-pthread should add before
    {
        printf("Please Entry # ");
        fflush(stdout);
        char buf[1024];
        ssize_t s = read(0, buf, sizeof(buf) - 1);
        if (s > 0)
        {
            buf[s] = 0;//end flag
        }
        write(sock, buf, strlen(buf));
        ssize_t _s = read(sock, buf, sizeof(buf) - 1);
        if (_s > 0)
        {
            buf[_s - 1] = 0;
            printf("Server Echo # %s\n", buf);
        }
    }
    close(sock);
    return 0;
}