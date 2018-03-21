/**
 *  \file   main.c
 *  \author James Petersen
 *
 *  \brief  This is the main file running the server
 */

#include "../include/server.h"
#include "../include/base.h"
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int _shutdown = 0;


/**
 *
 */
struct client
{
    pthread_t client_thread;
    int clientfd;
};


/**
 *
 */
static void int_handler(int sig)
{
    printf("\nIn %s\n", __func__);
    _shutdown = 1;
}


/**
 *
 */
void *handle_client(void *client)
{
    printf("in thread!\n");
    int *clientfd = (int *)client;
    char prompt[] = "you@thecoolest-tcpserver$ ";
    int prompt_len = strlen(prompt);

    while (_shutdown != 1)
    {
        char buf[MAX_LEN];

        if (send(*clientfd, prompt, prompt_len, 0) != prompt_len)
        {
            printf("Server Prompt Failed: %d\n", *clientfd);
            break;
        }
        
        if (get_msg(*clientfd, buf, MAX_LEN) < 0)
        {
            printf("Failed to get message: %d\n", errno);
            break;
        }
        printf("R: %s\n", buf);
    }
    return NULL;
}


/**
 *
 */
int main(int argc, char **argv)
{
    struct sigaction sigact;
    sigact.sa_handler = int_handler;
    sigaction(SIGINT, &sigact, NULL);


    hello_s();
    
    pthread_t client_thread;

    int sockfd = server_setup();
    if (sockfd < 0)
    {
        printf("Server Setup Failed: %d\n", sockfd);
        return ERROR;
    }

    int clientfd;
    while (_shutdown != 1 && clientfd = get_client(sockfd))
    {
        if (clientfd < 0)
        {
            printf("Server Setup Failed: %d\n", clientfd);
            return ERROR;
        }

        if (pthread_create(&client_thread, NULL, handle_client, &clientfd) != 0)
        {
            printf("Client Thread Creation Failed: %d\n", clientfd);
            return ERROR;
        }
    }

        
    printf("Shutting down..\n");
    close(sockfd);

    return SUCCESS;
}
