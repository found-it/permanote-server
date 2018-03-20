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
    get_msg(clientfd, buf);


    printf("R: %s\n", buf);

    close(sockfd);

    return SUCCESS;
}
