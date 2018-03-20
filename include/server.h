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

#define TCP_PORT 4444
#define IADDR    "127.0.0.1"
#define NUM_CONN 20

void hello_s();
int server_setup();

#endif
