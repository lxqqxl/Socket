#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

/*
readme:
    Muti_process TCP socket
    Server.cpp
    exit:
        -1:The input format is error
         0:The program is normal
         1:socket is error
         2:bind is error
         3:listen is error
         4:accept is error
*/

/*
socket -> bind -> listen -> accept(blocking, 3 handshake) -> send
*/

class Server
{
    public:
        int start_up(const char *ip, int port)
        {
            //1.socket
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0)
            {
                perror("socket");
                exit(1);
            }
            //2.bind
            struct sockaddr_in local;
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = inet_addr(ip);
            local.sin_port = htons(port);
            if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                perror("bind");
                exit(2);
            }
            //3.listen
            if (listen(sock, 10) < 0)
            {
                perror("listen");
                exit(3);
            }
            return sock;
        }
};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("The input format must be: %s [server-ip] [server-port]\n", argv[0]);
        exit(-1);
    }
    Server Socket;
    int sock = Socket.start_up(argv[1], atoi(argv[2]));
    //4.accept
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    while (1) //If muti-process model, there should add while(1), due to other IP will add and need to accept
    {
        int new_sock = accept(sock, (struct sockaddr *)&client, &len);
        if (new_sock < 0)
        {
            perror("accept");
            exit(4);
        }
        //add muti-process
        pid_t id = fork();
        if (id == 0) //child process
        {
            char buf[1024];
            while (1)
            {
                ssize_t s = read(new_sock, buf, sizeof(buf) - 1);
                if (s > 0)
                {
                    buf[s] = 0;
                    printf("Client %s say #: %s\n", inet_ntoa(client.sin_addr), buf);
                }
                else if (s == 0)
                {
                    printf("Client quit!\n");
                    break;
                }
            }
        }
        else if (id > 0) //father
        {
            pid_t _id = fork();
            if (_id == 0) //child
            {
                continue;
            }
            else if (_id > 0)//father
            {
                close(new_sock);
                exit(0);
            }
            else//error
            {
                break;
            }
            
        }
        else
        {
            perror("fork");
            break;
        }
        
/* 
        //output client information
        printf("Client_IP: %s, Client_PORT: %d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        //receive information
        char buf[1024];
        while (1)
        {
            ssize_t s = read(sock, buf, sizeof(buf) - 1);
            if (s > 0)
            {
                buf[s] = 0;
                printf("Client say #: %s", buf);
            }
            else if (s == 0)
            {
                printf("Client quit!\n");
                break;
            }
            write(new_sock, buf, strlen(buf));
        }
*/
    }
    close(sock);
    return 0;
}