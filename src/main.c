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

static int _shutdown = 0;

static void *handle_client(void *client);

/**
 *
 */
static void int_handler(int sig)
{
    _shutdown = 1;
}




/**
 *
 */
int main(int argc, char **argv)
{
    struct sigaction sigact;
    sigact.sa_handler = int_handler;
    sigaction(SIGINT, &sigact, NULL);

    pthread_t client_thread;

    int sockfd = server_setup();
    if (sockfd < 0)
    {
        printf("Server Setup Failed: %d\n", sockfd);
        return ERROR;
    }

    int clientfd;
    int *newfd;
    while (clientfd = get_client(sockfd))
    {
        if (_shutdown == 1)
            break;

        if (clientfd < 0)
        {
            printf("Server Setup Failed: %d\n", clientfd);
            return ERROR;
        }

        newfd = malloc(sizeof(int));
        *newfd = clientfd;

        if (pthread_create(&client_thread, NULL, handle_client, newfd) != 0)
        {
            printf("Client Thread Creation Failed: %d\n", clientfd);
            return ERROR;
        }
    }

        
    printf("Shutting down..\n");
    close(sockfd);

    return SUCCESS;
}



/**
 *
 */
static void *handle_client(void *client)
{
    int *clientfd = (int *)client;
    char prompt[MAX_LEN];
    int prompt_len;

    char user_prompt[] = "Username: ";
    int user_p_len = strlen(user_prompt);
    char username[MAX_LEN];
    

    if (send(*clientfd, user_prompt, user_p_len, 0) != user_p_len)
    {
        printf("Server Prompt Failed: %d\n", *clientfd);
        free(clientfd);
        pthread_exit(NULL);
    }

    if (get_msg(*clientfd, username, MAX_LEN) < 0)
    {
        printf("Failed to get message: %d\n", errno);
        free(clientfd);
        pthread_exit(NULL);
    }

    /* strip off the \n */
    username[strlen(username)-1] = '\0';
    strncpy(prompt, username, strlen(username));
    strncat(prompt, "@thecoolest-tcpserver$ ", MAX_LEN);
    prompt_len = strnlen(prompt, MAX_LEN);
    
    int rc = 0;
    int cmd;
    while (_shutdown != 1)
    {
        char buf[MAX_LEN];

        if (send(*clientfd, prompt, prompt_len, 0) != prompt_len)
        {
            printf("Server Prompt Failed: %d\n", *clientfd);
            break;
        }
        
        rc = get_msg(*clientfd, buf, MAX_LEN);
        if (rc < 0)
        {
            printf("Failed to get message: %d\n", rc);
            break;
        }
        else if (rc == 0)
            break;
        else
        {
            buf[strlen(buf)-1] = '\0';
            cmd = parse_command(buf);
        }

        exec_command(*clientfd, cmd);
    }
    free(clientfd);
    pthread_exit(NULL);
}
