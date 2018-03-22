/**
 *  \file   commands.c
 *  \author James Petersen
 *
 *  \brief  This is the main file for the server commands.
 */

#include "../include/commands.h"
#include "../include/server.h"
#include "../include/base.h"


/**
 *  \enum of commands used by the server
 */
enum commands
{
    Hello = 1,
    Help  = 2,
    Write = 3,
    Read  = 4
};

static int invalid(int fd);
static int help(int fd);
static int hello(int fd, const char *username);
static int write_file(int fd, const char *username);
static int read_file(int fd, const char *username);

/**
 *  parse_command()
 *
 *  \brief  This function compares the string that is
 *          sent by the client and returns a value from
 *          the command enum.
 */
int parse_command(char *input)
{
    if (strcmp(input, "hello") == 0)
        return Hello;

    if (strcmp(input, "help") == 0)
        return Help;

    if (strcmp(input, "write") == 0)
        return Write;

    if (strcmp(input, "read") == 0)
        return Read;

    return ERROR;
} /* function parse_command */


/**
 *  exec_command()
 *
 *  \brief  This function executes the parsed command
 *          using the command enum to ID each case.
 */
int exec_command(int fd, int cmd, const char *username)
{
    int status = SUCCESS;
    switch (cmd)
    {
        case Hello:
            status = hello(fd, username);
            break;
        case Help:
            status = help(fd);
            break;
        case Write:
            status = write_file(fd, username);
            break;
        case Read:
            status = read_file(fd, username);
            break;
        default:
            status = invalid(fd);
            break;
    }
    return status;
} /* function exec_command */

/**
 *  read_file()
 *
 *  \brief  
 */
static int read_file(int fd, const char *username)
{
    int read;
    char *buf;
    char filepath[len];
    size_t buf_len = MAX_LEN;
    size_t len = strlen(username) + 12;
    const char sep[] = "-------------------------\n";
    const int sep_len = strlen(sep);

    snprintf(filepath, len + 8, "./files/%s.md", username);

    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        const char err[] = "\n"
                           "You have not created a note yet\n"
                           "so there is nothing to read.\n"
                           "\n";
        const int err_len = strlen(err);

        send(fd, err, err_len, 0);
        return NOFILE;
    }

    if (send(fd, sep, sep_len, 0) != sep_len)
    {
        fprintf(stderr, "Reading line failed: %d\n", fd);
        return ERROR;
    }

    while ((read = getline(&buf, &buf_len, file)) != -1)
    {
        if (send(fd, buf, strlen(buf), 0) != strlen(buf))
        {
            fprintf(stderr, "Reading line failed: %d\n", fd);
            return ERROR;
        }
    }

    if (send(fd, sep, sep_len, 0) != sep_len)
    {
        fprintf(stderr, "Reading line failed: %d\n", fd);
        return ERROR;
    }

    fclose(file);
    return SUCCESS;
} /* function read_file */

/**
 *  write_file()
 *
 *  \brief  
 */
static int write_file(int fd, const char *username)
{
    size_t len = strlen(username) + 4;
    char filename[len];
    char filepath[len + 8];
    char response[MAX_LEN];
    char buf[MAX_LEN];
    int l;
    int  response_len;
    snprintf(filename, len, "%s.md", username);
    snprintf(filepath, len + 8, "./files/%s", filename);

    FILE *file = fopen(filepath, "a");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return ERROR;
    }

    snprintf(response, MAX_LEN, "Opening file: %s\n", filename);
    response_len = strlen(response);

    if (send(fd, response, response_len, 0) != response_len)
    {
        fprintf(stderr, "Response Failed: %d\n", fd);
        return ERROR;
    }

    while ((l = get_msg(fd, buf, MAX_LEN)) != 0) 
    {
        if (strcmp(buf, "...\n") == 0)
            break;
        fprintf(file, "%s", buf);
    }

    fclose(file);
    return SUCCESS;
} /* function write_file */

/**
 *  hello()
 *
 *  \brief  This function sends the hello string to
 *          the client.
 */
static int hello(int fd, const char *username)
{
    const char hello[] = "\n"
                         "Oh hai Mark\n"
                         "\n";
    const int hello_len = strlen(hello);
    if (send(fd, hello, hello_len, 0) != hello_len)
    {
        fprintf(stderr, "Hello Command Failed: %d\n", fd);
        return ERROR;
    }
    return SUCCESS;
} /* function hello */


/**
 *  help()
 *
 *  \brief  This function sends the help menu to
 *          the client.
 */
static int help(int fd)
{
    const char help[] = "\n"
                        "           HELP MENU\n"
                        " --------------------------------\n"
                        "\n"
                        " help  - prints help menu\n"
                        " hello - prints a hello message\n"
                        " write - write text to your notes document\n"
                        " read  - read your current notes sheet\n"
                        "\n"
                        " To exit simply use ctrl-c.\n"
                        " --------------------------------\n"
                        "\n";
    const int help_len = strlen(help);
    if (send(fd, help, help_len, 0) != help_len)
    {
        fprintf(stderr, "Help Command Failed: %d\n", fd);
        return ERROR;
    }
    return SUCCESS;
} /* function help */


/**
 *  invalid()
 *
 *  \brief  This function responds to the client
 *          if the command is invalid.
 */
static int invalid(int fd)
{
    const char invalid[] = "\n"
                           "Invalid command.\n"
                           "Use 'help' to view a list of valid commands.\n"
                           "\n";
    const int invalid_len = strlen(invalid);
    if (send(fd, invalid, invalid_len, 0) != invalid_len)
    {
        fprintf(stderr, "Hello Command Failed: %d\n", fd);
        return ERROR;
    }
    return SUCCESS;

} /* function invalid */
