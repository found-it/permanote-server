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
    Read  = 4,
    Enter = 5
};

/* static function prototypes */
static int invalid(int fd);
static int help(int fd);
static int hello(int fd);
static int write_file(int fd, const char *username);
static int read_file(int fd, const char *username);


/**
 *  parse_command()
 *
 *  \brief  This function compares the string that is
 *          sent by the client and returns a value from
 *          the command enum.
 */
int parse_command(const char *input)
{
    if (strcmp(input, "hello") == 0)
        return Hello;

    else if (strcmp(input, "help") == 0)
        return Help;

    else if (strcmp(input, "write") == 0)
        return Write;

    else if (strcmp(input, "read") == 0)
        return Read;

    else if (strcmp(input, "") == 0) /* allow user to hit enter to get space */
        return Enter;

    else
        return ERROR;
} /* function parse_command */


/**
 *  exec_command()
 *
 *  \brief  This function executes the parsed command
 *          using the command enum to ID each case.
 */
int exec_command(int fd, const int cmd, const char *username)
{
    int status = SUCCESS;
    switch (cmd)
    {
        case Hello:
            status = hello(fd);
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
        case Enter:
            /* do nothing */
            break;
        default:
            status = invalid(fd);
            break;
    }
    return status;
} /* function exec_command */


/**
 *  hello()
 *
 *  \brief  This function sends the hello string to
 *          the client.
 */
static int hello(int fd)
{
    const char hello[] = "\n"
                         "Oh hai there!\n"
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


/**
 *  read_file()
 *
 *  \brief  This function displays the users notes file
 *          to them.
 */
static int read_file(int fd, const char *username)
{
    /* initialized variables */
    char  *buf        = NULL;
    size_t buf_len    = MAX_LEN;
    size_t len        = strlen(username) + 12; /* +12 for path & name */
    const char sep[]  = "-------------------------\n";
    const int sep_len = strlen(sep);

    /* uninitialized variables */
    int   bytes;
    char  filepath[len];
    FILE *file;

    /* create the filepath */
    snprintf(filepath, len, "./files/%s.md", username);

    /* open the file */
    file = fopen(filepath, "r");
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

    /* send separator */
    if (send(fd, sep, sep_len, 0) != sep_len)
    {
        fprintf(stderr, "Reading line failed: %d\n", fd);
        return ERROR;
    }

    /* main loop to read the file */
    while ((bytes = getline(&buf, &buf_len, file)) != -1)
    {
        if (send(fd, buf, strlen(buf), 0) != strlen(buf))
        {
            fprintf(stderr, "Reading line failed: %d\n", fd);
            return ERROR;
        }
    }

    /* free buf allocated by getline */
    free(buf);

    /* send separator */
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
 *  \brief  This function allows the user to write to their
 *          notes file.
 */
static int write_file(int fd, const char *username)
{
    /* initialized variables */
    const char header[] = "\n"
                          "Opened your notes sheet. Type your notes\n"
                          "as you please and they will be saved line\n"
                          "by line. Beware, you can only enter 1024\n"
                          "characters on one line, any more will be\n"
                          "cut off. If you have added notes to your\n"
                          "notes sheet before, any lines you add now\n"
                          "will be appended to your previous notes.\n"
                          "\n"
                          "To stop recording, type ;;; on a blank line\n"
                          "and hit enter.\n"
                          "\n";
    const int header_len = strlen(header);
    size_t len           = strlen(username) + 12; /* +12 for path & name */

    /* uninitialized variables */
    char  filepath[len];
    char  buf[MAX_LEN];
    FILE *file;

    /* create the path for the file */
    snprintf(filepath, len, "./files/%s.md", username);

    /* open the file */
    file = fopen(filepath, "a");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", filepath);
        return ERROR;
    }

    /* print the header for the client */
    if (send(fd, header, header_len, 0) != header_len)
    {
        fprintf(stderr, "Header printing Failed: %d\n", fd);
        return ERROR;
    }

    /* save lines of input to file until ';;;' is received */
    while (get_msg(fd, buf, MAX_LEN) != 0)
    {
        if (strncmp(buf, ";;;\n", MAX_LEN) == 0)
            break;
        fprintf(file, "%s", buf);
    }

    fclose(file);
    return SUCCESS;
} /* function write_file */
