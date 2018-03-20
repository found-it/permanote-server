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

#define TCP_PORT  4444
#define IADDR    "127.0.0.1"
#define NUM_CONN  20

void hello_s();
int server_setup();
int get_client(int sockfd);
int __get_msg__(int fd, char buf[], size_t size);
#define get_msg(fd, buf) __get_msg__(fd, buf, sizeof(buf)/sizeof(buf[0]))

#endif
