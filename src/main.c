/**
 *  \file   main.c
 *  \author James Petersen
 *
 *  \brief  This is the main file running the server
 */

#include "../include/server.h"
#include "../include/base.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int get_client(int sockfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    return accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
/*
    if ((client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0)
    {
        printf("Accept failed: %d\n", errno);
        return ERROR;
    }
*/
}

int main(int argc, char **argv)
{
    hello_s();

    int sockfd = server_setup();
    if (sockfd < 0)
    {
        printf("Server Setup Failed: %d\n", sockfd);
        return ERROR;
    }

    int clientfd = get_client(sockfd);
    if (clientfd < 0)
    {
        printf("Server Setup Failed: %d\n", clientfd);
        return ERROR;
    }


    char buf[MAX_LEN];
    memset(buf, '\0', MAX_LEN);
    int bytes;
    if ((bytes = recv(clientfd, buf, MAX_LEN-1, 0)) < 0)
    {
        printf("Receive failed: %d\n", errno);
        close(sockfd);
        return ERROR;
    }

    printf("R: %s\n", buf);

    close(sockfd);

    return SUCCESS;
}
