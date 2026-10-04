#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <ifaddrs.h>

#define PORT 5050
#define MULTICAST_ADDR "239.0.0.1"
#define BUF_SIZE 4096

char ip[INET_ADDRSTRLEN];

void get_ip_address(void)
{
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;

    if (getifaddrs(&ifaddr) == -1)
        exit(1);

    for (ifa = ifaddr; ifa; ifa = ifa->ifa_next) {

        if (!ifa->ifa_addr)
            continue;

        if (ifa->ifa_addr->sa_family != AF_INET)
            continue;

        struct sockaddr_in *a =
            (struct sockaddr_in *)ifa->ifa_addr;

        if (a->sin_addr.s_addr == htonl(INADDR_LOOPBACK))
            continue;

        if (inet_ntop(AF_INET, &a->sin_addr,
                      ip, sizeof(ip))) {
            break;
        }
    }

    freeifaddrs(ifaddr);
}

int main(void)
{
    char buffer[BUF_SIZE];
    int socketfd;
    struct sockaddr_in addr, client;
    socklen_t client_len = sizeof(client);

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketfd < 0) {
        perror("socket");
        exit(1);
    }

    get_ip_address();

    printf("[ 🛜  ] %s:%d - UDP \n", ip, PORT);

    /*
     * Bind na porta multicast
     */
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketfd,
             (struct sockaddr *)&addr,
             sizeof(addr)) < 0) {

        perror("bind");
        exit(1);
    }

    /*
     * Entrar no grupo multicast
     */
    struct ip_mreq mreq;

    mreq.imr_multiaddr.s_addr =
        inet_addr(MULTICAST_ADDR);

    mreq.imr_interface.s_addr =
        htonl(INADDR_ANY);

    if (setsockopt(socketfd,
                   IPPROTO_IP,
                   IP_ADD_MEMBERSHIP,
                   &mreq,
                   sizeof(mreq)) < 0) {

        perror("IP_ADD_MEMBERSHIP");
        exit(1);
    }

    /*
     * Endereço para resposta multicast
     */
    struct sockaddr_in multicast;

    memset(&multicast, 0, sizeof(multicast));

    multicast.sin_family = AF_INET;
    multicast.sin_port = htons(PORT);
    multicast.sin_addr.s_addr =
        inet_addr(MULTICAST_ADDR);

    /*
     * TTL = 1
     */
    unsigned char ttl = 1;

    setsockopt(socketfd,
               IPPROTO_IP,
               IP_MULTICAST_TTL,
               &ttl,
               sizeof(ttl));

    for (;;) {

        printf("[ 🕛 ] Esperando... \n");

        int n = recvfrom(
            socketfd,
            buffer,
            sizeof(buffer),
            0,
            (struct sockaddr *)&client,
            &client_len
            );


        if (n < 0) {
            perror("recvfrom");
            exit(1);
        }

        printf("[ 🛑 ] %s:%d ",
               inet_ntoa(client.sin_addr),
               ntohs(client.sin_port));

        if (sendto(
            socketfd,
            ip,
            strlen(ip),
            0,
            (struct sockaddr *)&client,
            client_len
            )  == -1) {
                printf("❌\n"); 
        } else {
            
                printf("✅\n");
        }

        
    }

    close(socketfd);

    return 0;
}