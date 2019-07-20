#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

/*
readme:
    Muti-Thread TCP socket
    exit:
        -1:The input format is error
         0:The program is normal
         1:

*/

struct parameter
{
    char *IP;
    int PORT;
    int sock;
}HandleParameter;

class Server
{
    public:
        int Set_up(const char *IP, int PORT)
        {
            //1.socket
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0)
            {
                perror("socket");
                exit(1);
            }
            //reconnectable for accidental disconnection
            int opt = 1;
            if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            {
                perror("setsockopt");
                exit(5);
            }
            //2.bind
            struct sockaddr_in local;
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = inet_addr(IP);
            local.sin_port = htons(PORT);
            //ssize_t len = sizeof(local);
            if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                perror("bind");
                exit(2);
            }
            if (listen(sock, 10) < 0)
            {
                perror("listen");
                exit(3);
            }
            return sock;
        }
};

void* HandlerRequest(void* arg)
{
    parameter handleParameter = *((parameter *)arg);
    char buf[10024];
    //const char *msg =  "HTTP/1.0 200 OK <\r\n\r\n<html><h1>yingying beautiful</h1></html>\r\n";
    while (1)
    {
        ssize_t s = read(handleParameter.sock, buf, sizeof(buf) - 1);
        if (s > 0)
        {
            buf[s] = 0;
            printf("Client %s %d Say # %s", handleParameter.IP, handleParameter.PORT, buf);
            write(handleParameter.sock, buf, strlen(buf));
            //break;
        }
        else if (s == 0)
        {
            printf("Client Quit!\n");
            break;
        }
    }

}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("The input format must be: %s [Server-IP] [Server-PORT\n", argv[0]);
        exit(-1);
    }
    Server Socket;
    int sock = Socket.Set_up(argv[1], atoi(argv[2]));
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    while (1)
    {
        /*
        int newsock = accept(sock, (struct sockaddr *)&client, &len);
        if (newsock < 0)
        {
            perror("accept");
            exit(4);
        }
        printf("Client %s %d is connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        char buf[1024];
        while (1)
        {
            ssize_t s = read(newsock, buf, sizeof(buf) - 1);
            if (s > 0)
            {
                buf[s] = 0;
                printf("Client Say # %s", buf);
            }
            if (s == 0)
            {
                printf("Client Quit!\n");
                break;
            }
            write(newsock, buf, strlen(buf));
        }
        */
       //Muti-Thread program
        int new_sock = accept(sock, (struct sockaddr *)&client, &len);
        if (new_sock < 0)
        {
            perror("accept");
            exit(4);
        }
        printf("Client %s %d is connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        pthread_t id;

        //assign
        HandleParameter.IP = inet_ntoa(client.sin_addr);
        HandleParameter.PORT = ntohs(client.sin_port);
        HandleParameter.sock = new_sock;

        pthread_create(&id, NULL, HandlerRequest, (void *)&HandleParameter);//if success return 0, else return -1
        pthread_detach(id);
    }
    close(sock);
    return 0;
}
