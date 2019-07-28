#include <bits/stdc++.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

void* receive_inf(void *rec)
{
    //1.socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }
    //bind
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    char buf[1024];
    memset(buf, 0, sizeof(buf));

    struct sockaddr_in from_addr;
    memset(&from_addr, 0, sizeof(from_addr));
    socklen_t len = sizeof(from_addr);

    while (1)
    {
        int s = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&from_addr, &len);
        if (s > 0)
        {
            printf("Clinet IP %s, PORT %d\n", inet_ntoa(from_addr.sin_addr), ntohs(from_addr.sin_port));
            printf("Clinet Say # %s\n", buf);
        }
        else if (s == 0)
        {
            printf("Client Quit!\n");
            break;
        }
    }
    close(sock);
}

void* send_inf(void *send)
{
    //1.socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in send_addr;
    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.s_addr = inet_addr("192.168.11.17");
    send_addr.sin_port = htons(8000);
    socklen_t len = sizeof(send_addr);
    
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    while(1)
    {
        cin.getline(buf, sizeof(buf));
        printf("Server Say # %s\n", buf);
        sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *)&send_addr, len);
    }
    close(sock);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("The input format must be %s [SERVER-IP] [INPUT-PORT] [OUTPUT-PORT]\n", argv[0]);
        exit(-1);
    }
    pthread_t rec_id, send_id;
    pthread_create(&rec_id, 0, receive_inf, NULL);
    pthread_create(&send_id, 0, send_inf, NULL);

    pthread_join(rec_id, NULL);
    pthread_join(send_id, NULL);
    return 0;
}