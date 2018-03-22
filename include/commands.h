/**
 *  \file   commands.h
 *  \author James Petersen
 *
 *  \brief  This is the main header file for the server commands.
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 *  parse_command()
 *
 *  \brief  This function compares the string that is
 *          sent by the client and returns a value from
 *          the command enum.
 */
int parse_command(const char *input);


/**
 *  exec_command()
 *
 *  \brief  This function executes the parsed command
 *          using the command enum to ID each case.
 */
int exec_command(int fd, const int cmd, const char *username);

#endif
