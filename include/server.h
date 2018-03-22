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

enum commands {Hello = 1, Help = 2};

void hello_s();
int server_setup();
int get_client(int sockfd);
int get_msg(int fd, char buf[], size_t size);

int parse_command(char *input);
int exec_command(int fd, int cmd);

int hello(int fd);
int help(int fd);


#endif
