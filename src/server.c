/**
 *  \file   server.c
 *  \author James Petersen
 *
 *  \brief  This is the main file for the server functions.
 */

#include "../include/server.h"
#include "../include/base.h"

void hello_s()
{
    printf("Hello from %s\n", __FILE__);
}

int server_setup()
{
    int sockfd;
    int yes = 1;
    struct sockaddr_in addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation failed: %d\n", errno);
        return ERROR;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    {
        printf("Socket option failed: %d\n", errno);
        return ERROR;
    }

    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(TCP_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("Socket binding failed: %d\n", errno);
        return ERROR;
    }

    if (listen(sockfd, NUM_CONN) < 0)
    {
        printf("Listen failed: %d\n", errno);
        return ERROR;
    }

    return sockfd;

}

int get_client(int sockfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    return accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
}


int __get_msg__(int fd, char buf[], size_t size)
{
    memset(buf, '\0', size);
    return recv(fd, buf, size-1, 0);
}
