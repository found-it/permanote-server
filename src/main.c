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
#include <signal.h>

int _shutdown = 0;

static void int_handler(int sig)
{
    _shutdown = 1;
    printf("IN %s\n", __func__);
}


int main(int argc, char **argv)
{
    struct sigaction sigact;
    sigact.sa_handler = int_handler;
    sigaction(SIGINT, &sigact, NULL);


    hello_s();
    
    char prompt[] = "> ";
    int prompt_len = strlen(prompt);
    int bytes_sent;

    int sockfd = server_setup();
    if (sockfd < 0)
    {
        printf("Server Setup Failed: %d\n", sockfd);
        return ERROR;
    }

    while (_shutdown != 1)
    {
        printf("_shutdown: %d\n", _shutdown);
        char buf[MAX_LEN];

        int clientfd = get_client(sockfd);
        if (clientfd < 0)
        {
            printf("Server Setup Failed: %d\n", clientfd);
            break;
        }

        if (send(clientfd, prompt, prompt_len, 0) != prompt_len)
        {
            printf("Server Prompt Failed: %d\n", clientfd);
            break;
        }
        
        if (get_msg(clientfd, buf, MAX_LEN) < 0)
        {
            printf("Failed to get message: %d\n", errno);
            break;
        }
        printf("R: %s\n", buf);
    }
    printf("Shutting down..\n");
    close(sockfd);

    return SUCCESS;
}
