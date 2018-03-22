/**
 *  \file   server.c
 *  \author James Petersen
 *
 *  \brief  This is the main file for the server functions.
 */

#include "../include/server.h"
#include "../include/base.h"


/**
 *  \enum of commands used by the server
 */
enum commands 
{
    Hello = 1, 
    Help  = 2,
    Write = 3
};


/**
 *  server_setup()
 *
 *  \brief  This function sets up the server socket and returns
 *          the file descriptor that identifies it.
 */
int server_setup()
{
    int sockfd;
    int yes = 1;
    struct sockaddr_in addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Socket creation failed: %d\n", errno);
        return ERROR;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    {
        fprintf(stderr, "Socket option failed: %d\n", errno);
        return ERROR;
    }

    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(TCP_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "Socket binding failed: %d\n", errno);
        return ERROR;
    }

    if (listen(sockfd, NUM_CONN) < 0)
    {
        fprintf(stderr, "Listen failed: %d\n", errno);
        return ERROR;
    }
    return sockfd;
} /* function server_setup */


/**
 *  get_client()
 *
 *  \brief  This function gets a new connected client.
 *  \return Client fd on success, -1 on error.
 */
int get_client(int sockfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    return accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
} /* function get_client */


/**
 *  get_msg()
 *
 *  \brief  This function clears the buffer that it will
 *          fill then returns the number of bytes received.
 */
int get_msg(int fd, char buf[], size_t size)
{
    memset(buf, '\0', size);
    return recv(fd, buf, size-1, 0);
} /* function get_msg */
