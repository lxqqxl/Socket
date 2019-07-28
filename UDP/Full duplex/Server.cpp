#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;

/*
readme:
    eixt:
        -1:The input format is error
         1:Socket is error
         2:Bind is error
*/

/*
define struct for socket
*/

struct Sock_inf
{
    int sock;
    struct sockaddr_in sour_addr;
    socklen_t len;
};

void* recieve_inf(void *p)
{
    Sock_inf cli_inf = *((Sock_inf *)p);
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int s = recvfrom(cli_inf.sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&cli_inf.sour_addr, &(cli_inf.len));
    if (s > 0)
    {
        printf("Client Say # %s", buf);
    }
    else if (s == 0)
    {
        printf("Client Quit!\n");
    }
    pthread_exit(0);
}

void* send_inf(void *p)
{
    Sock_inf cli_inf = *((Sock_inf *)p);
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    if (cin.getline(buf, sizeof(buf) - 1))
    {
        printf("Server Say # ");
        //fgets(buf, sizeof(buf) - 1, stdin);
        sendto(cli_inf.sock, buf, sizeof(buf), 0, (struct sockaddr *)&cli_inf.sour_addr, cli_inf.len);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("The input format must be: %s [SERVER-IP] [SERVER-PORT]\n", argv[0]);
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
    struct sockaddr_in ser_addr;
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ser_addr.sin_port = htons(atoi(argv[2]));
    
    if (bind(sock, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    //char buf[1024];
    //memset(buf, 0, sizeof(buf));
    //struct sockaddr_in cli_addr;
    //socklen_t len = sizeof(cli_addr);

    pthread_t recieve_id, read_id;

    struct sockaddr_in cli_addr;
    memset(&cli_addr, 0, sizeof(cli_addr));

    Sock_inf ser_addr_rev, ser_addr_send;
    ser_addr_rev.sour_addr = cli_addr;
    ser_addr_rev.len = sizeof(ser_addr_rev.sour_addr);
    ser_addr_rev.sock = sock;
    
    ser_addr_send.sour_addr = cli_addr;
    ser_addr_send.len = sizeof(ser_addr_send.sour_addr);
    ser_addr_send.sock = sock;
    
    while (1)
    {
        //int s = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&cli_addr, &len);
        //printf("Client IP : %s\n", inet_ntoa(cli_addr.sin_addr));
        //printf("Client Port : %d\n", ntohs(cli_addr.sin_port));
        /*
        
        if (s > 0)
        {
            printf("Client Say # %s", buf);
        }
        else if (s == 0)
        {
            printf("Client Quit!\n");
            break;
        }
        sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&cli_addr, len);
        memset(buf, 0, sizeof(buf)); 
        */  
        pthread_create(&recieve_id, 0, recieve_inf, (void *)&ser_addr_rev);
        pthread_create(&read_id, 0, send_inf, (void *)&ser_addr_send);
    }
    pthread_join(recieve_id, NULL);
    pthread_join(read_id, NULL);
    close(sock);
    return 0; 
}