#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

/*
readme:
    exit:
        -1:The input format is error
         1:Socket is error
         2:Bind is error
*/

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("The input format must be : %s [SERVER-IP] [SERVER-PORT].\n", argv[0]);
        exit(-1);
    }
    //1.socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }
    //2.bind
    struct sockaddr_in Server;
    memset(&Server, 0, sizeof(Server));
    Server.sin_family = AF_INET;
    Server.sin_addr.s_addr = inet_addr(argv[1]);
    Server.sin_port = htons(atoi(argv[2]));
    if (bind(sock, (struct sockaddr *)&Server, sizeof(Server)) < 0)
    {
        perror("bind");
        exit(2);
    }
    //deal
    struct sockaddr_in Client;
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    while (1)
    {
        socklen_t len = sizeof(Client);
        
        int s = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&Client, &len);//blocking function
        if (s > 0)
        {
            buf[s] = 0;
            printf("Client Say # %s", buf);
        }
        if (s == 0)
        {
            printf("Client Quit!\n");
            //close(sock);
            //return 0;
            break;
        }
        sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&Client, sizeof(Client));
    }
    close(sock);
    return 0;
}