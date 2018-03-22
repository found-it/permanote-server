/**
 *  \file   server.c
 *  \author James Petersen
 *
 *  \brief  This is the main file for the server functions.
 */

#include "../include/server.h"
#include "../include/base.h"


/**
 *
 */
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

/**
 *
 */
int get_client(int sockfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    return accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
}

/**
 *
 */
int get_msg(int fd, char buf[], size_t size)
{
    memset(buf, '\0', size);
    return recv(fd, buf, size-1, 0);
}

/**
 *
 */
int parse_command(char *input)
{
    if (strcmp(input, "hello") == 0)
        return Hello;
    
    if (strcmp(input, "help") == 0)
        return Help;
    
    return ERROR;
}

/**
 *
 */
int exec_command(int fd, int cmd)
{
    switch (cmd)
    {
    case Hello:
        hello(fd);
        break;
    case Help:
        help(fd);
        break;
    default:
        printf("command failure\n");
        break;
    }
    return SUCCESS;
}

/**
 *
 */
int hello(int fd)
{
    char hello[] = "Oh hai Mark\n";
    int hello_len = strlen(hello);
    if (send(fd, hello, hello_len, 0) != hello_len)
    {
        printf("Hello Command Failed: %d\n", fd);
        return ERROR;
    }
    return SUCCESS;
}

/**
 *
 */
int help(int fd)
{
    char help[] =   "--------------------------------\n"
                    "          HELP MENU\n"
                    "--------------------------------\n"
                    "\n"
                    "help   - prints help menu\n"
                    "hello  - prints a hello message\n"
                    "\n";
    int help_len = strlen(help);
    if (send(fd, help, help_len, 0) != help_len)
    {
        printf("Help Command Failed: %d\n", fd);
        return ERROR;
    }
    return SUCCESS;
}
