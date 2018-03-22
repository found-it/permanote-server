/**
 *  \file   server.h
 *  \author James Petersen
 *
 *  \brief  This is the main header file for the server functions.
 */

#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* Server defines */
#define TCP_PORT  4444
#define NUM_CONN  20


/**
 *  server_setup()
 *
 *  \brief  This function sets up the server socket and returns
 *          the file descriptor that identifies it.
 */
int server_setup();


/**
 *  get_client()
 *
 *  \brief  This function gets a new connected client.
 *  \return Client fd on success, -1 on error.
 */
int get_client(int sockfd);


/**
 *  get_msg()
 *
 *  \brief  This function clears the buffer that it will
 *          fill then returns the number of bytes received.
 */
int get_msg(int fd, char buf[], size_t size);

#endif
