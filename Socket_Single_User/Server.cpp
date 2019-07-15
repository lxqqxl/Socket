#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

/*
readme:
    This is cpp can only deal single user
    TCP socket program
    exit():
        1: socket is error;
        2: bind is error;
        3: listen is error
        4: accept is error
*/

/*
server:
    socket -> bind -> listen -> accept(Blocking, Three-way handshake) -> send
client:
    socket -> connect(Blocking, Three-way handshake) -> rcv
*/

 class Server
 {
 public:
    static int start_up(const char* local_ip, int local_port)
    {
        //1.create sock
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("socket");
            close(sock);
            exit(1);
        }
        //2.create bind
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(local_port);
        local.sin_addr.s_addr = inet_addr(local_ip);//Can there use string? if not  change to const char * type
        if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            perror("bind");
            close(sock);
            exit(2);
        }
        //3.listen
        if (listen(sock, 10) < 0)
        {
            perror("listen");
            close(sock);
            exit(3);
        }
        return sock;
    }
 };
int main(int argc, char *argv[])//const char * type
{
    if (argc < 3)
    {
        printf("The input format must be: %s [server-ip] [server-port]\n", argv[0]);
    }
    //build sock
    Server Socket;
    int sock = Socket.start_up(argv[1], atoi(argv[2]));
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    while (1)
    {
        int new_sock = accept(sock, (struct sockaddr *) &client, &len);
        if (new_sock < 0)
        {
            perror("accept");
            close(sock);
            return 4;
        }
        printf("client_ip : %s, client_port : %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        char buf[1024];
        while (1)
        {
            ssize_t s = read(new_sock, buf, sizeof(buf) - 1);//waiting for trigger, so use select only is inefficient
            printf("t = %ld\n", s);
            if (s > 0)
            {
                buf[s] = 0;//assignment 0 here to ensure end of s.
                printf("client say # %s", buf);
            }
            else if (s == 0)
            {
                printf("client quit!\n");
                break;
            }
            write(new_sock, buf, strlen(buf));
        }
    }
    close(sock);
    return 0;
}
