#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
using namespace std;

struct Sock_inf
{
    int sock;
    struct sockaddr_in sour_addr;
    socklen_t len;
}; 

void* recieve_inf(void *p)
{
    Sock_inf ser_inf = *((Sock_inf *)p);
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int s = recvfrom(ser_inf.sock, buf, sizeof(buf), 0, (struct sockaddr *)&ser_inf.sour_addr, &ser_inf.len);
    if (s > 0)
    {
        printf("Server Say # %s", buf);
    }
    else if (s == 0)
    {
        printf("Server Say no!\n");
    }
    pthread_exit(0);
}
void* send_inf(void *p)
{
    Sock_inf ser_inf = *((Sock_inf *)p);
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    if (cin.getline(buf, sizeof(buf) - 1))
    {
        printf("Client Say # %s", buf);
        sendto(ser_inf.sock, buf, sizeof(buf), 0, (struct sockaddr *)&ser_inf.sour_addr, ser_inf.len);
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

    struct sockaddr_in ser_addr;
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ser_addr.sin_port = htons(atoi(argv[2]));
    socklen_t len = sizeof(ser_addr);

    if (bind(sock, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    //char buf[1024];
    //memset(buf, 0, sizeof(buf));
    pthread_t recieve_id, send_id;
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    Sock_inf cli_addr_rev, cli_addr_send;
    cli_addr_rev.sour_addr = addr;
    cli_addr_rev.len = sizeof(cli_addr_rev.sour_addr);
    cli_addr_rev.sock = sock;

    cli_addr_send.sour_addr = ser_addr;
    cli_addr_send.len = sizeof(cli_addr_send.sour_addr);
    cli_addr_send.sock = sock;

    while (1)
    {
        /*
        memset(buf, 0, sizeof(buf));
        printf("Client Say # ");
        fgets(buf, sizeof(buf) - 1, stdin);
        sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&ser_addr, len);
        int s = recvfrom(sock, buf, strlen(buf), 0, (struct sockaddr *)&ser_addr, &len);
        if (s > 0)
        {
            printf("Server Say # %s", buf);
        }
        */
        pthread_create(&recieve_id, 0, recieve_inf, (void *)&cli_addr_rev);
        pthread_create(&send_id, 0, send_inf, (void *)&cli_addr_send);
    }
    return 0;
}