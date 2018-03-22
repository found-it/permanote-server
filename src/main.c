/**
 *  \file   main.c
 *  \author James Petersen
 *
 *  \brief  This is the main file running the server
 */

#include "../include/commands.h"
#include "../include/server.h"
#include "../include/base.h"
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/* shutdown variable for threads */
static int _shutdown = 0;

/* static function definitions */
static void *handle_client(void *client);


/**
 *  int_handler()
 *
 *  \brief  This function handles the ctrl-C interrupt.
 */
static void int_handler(int sig)
{
    _shutdown = 1;
} /* function int_handler */


/**
 *  main()
 *
 *  \brief  This is the main server loop function.
 */
int main(int argc, char **argv)
{
    int *newfd;
    int  sockfd;
    int  clientfd;
    pthread_t client_thread;
    struct sigaction sigact;

    /* set up signal handling */
    sigact.sa_handler = int_handler;
    sigaction(SIGINT, &sigact, NULL);

    /* set up the server socket */
    if ((sockfd = server_setup()) < 0)
    {
        fprintf(stderr, "Server Setup Failed: %d\n", sockfd);
        return ERROR;
    }

    /* loop for each client */
    while ((clientfd = get_client(sockfd)))
    {
        /* check for shutdown */
        if (_shutdown == 1)
            break;

        /* check for errors */
        if (clientfd < 0)
        {
            fprintf(stderr, "Server Setup Failed: %d\n", clientfd);
            return ERROR;
        }

        /* allocate memory for the new client id */
        newfd = malloc(sizeof(int));
        *newfd = clientfd;

        /* create the new client thread */
        if (pthread_create(&client_thread, NULL, handle_client, newfd) != 0)
        {
            fprintf(stderr, "Client Thread Creation Failed: %d\n", clientfd);
            return ERROR;
        }
    }

    printf("Shutting down..\n");
    close(sockfd);

    return SUCCESS;
} /* function main */



/**
 *  handle_client()
 *
 *  \brief  This function is the main thread entry function
 *          that handles the communication with each client.
 */
static void *handle_client(void *client)
{
    int *clientfd = (int *)client;
    int  rc;                        /* return count for get_msg */
    int  cmd;                       /* command value */
    char buf[MAX_LEN];              /* buffer received from client */
    char prompt[MAX_LEN];           /* prompt sent to client */
    int  prompt_len;                /* length of prompt */
    char username[MAX_LEN];         /* username */

    /* prompt for username */
    const char user_prompt[] = "\n"
                               "Hello!\n"
                               "\n"
                               "You have been connected to James' TCP Server\n"
                               "\n"
                               "Please enter your username below.\n"
                               "If this is your first time here, feel free\n"
                               "to enter a username of your choice.\n"
                               "\n"
                               "Username: ";
    const int  user_p_len = strlen(user_prompt);

    /* prompt for username */
    if (send(*clientfd, user_prompt, user_p_len, 0) != user_p_len)
    {
        fprintf(stderr, "Server Prompt Failed: %d\n", *clientfd);
        free(clientfd);
        pthread_exit(NULL);
    }

    /* get username */
    if (get_msg(*clientfd, username, MAX_LEN) < 0)
    {
        fprintf(stderr, "Failed to get message: %d\n", errno);
        free(clientfd);
        pthread_exit(NULL);
    }

    /* strip off the \n */
    username[strlen(username)-1] = '\0';

    /* construct the prompt */
    strncpy(prompt, username, strlen(username));
    strncat(prompt, "@open-note$ ", MAX_LEN);
    prompt_len = strnlen(prompt, MAX_LEN);

    /* loop to communicate with client */
    while (_shutdown != 1)
    {
        /* send the prompt to the client */
        if (send(*clientfd, prompt, prompt_len, 0) != prompt_len)
        {
            fprintf(stderr, "[%s] Server Prompt Failed: %d\n", username, *clientfd);
            break;
        }

        /* get the command from the client */
        rc = get_msg(*clientfd, buf, MAX_LEN);
        if (rc < 0)
        {
            fprintf(stderr, "[%s] Failed to get message: %d\n", username, rc);
            break;
        }
        else if (rc == 0) /* client quit */
        {
            break;
        }
        else /* parse the command */
        {
            buf[strlen(buf)-1] = '\0';
            cmd = parse_command(buf);
        }

        if (exec_command(*clientfd, cmd, username))
        {
            fprintf(stderr, "[%s] Error executing command\n", username);
            break;
        }
    }
    free(clientfd);
    pthread_exit(NULL);
}/* function handle_client */
