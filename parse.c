//
// Created by ashwin on 21/09/20.
//
#include "headers.h"
#include "shell.h"

/* function to split commands */
char **split_commands(char *line, int *count, char *delim)
{
    char **commands = (char **)calloc(strlen(line) + 1, sizeof(char *));
    char *command = strtok(line, delim);
    while (command != NULL)
    {
        commands[*count] = command;
        command = strtok(NULL, delim);
        (*count)++;
    }
    return commands;
}

/* read and parse the commands */
int parse(char *homedir)
{
    char *line = NULL, *history = NULL;
    size_t len = 0;
    ssize_t nread;
    nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        StoreHistory();
        printf("\n");
        /*perror("*** ERROR");*/
        return EXIT_FAILURE;
    }
    line[--nread] = '\0';
    if (strcmp(line, "quit") == 0 ||
        strcmp(line, "exit") == 0)
    {
        StoreHistory();
        return EXIT_FAILURE;
    }
    history = (char *)malloc(strlen(line) + 1);
    strcpy(history, line);
    char *temp = history;
    history = strtok(history, " \r\n\t\a\b\f;");
    if (history != NULL)
    {
        addHistory(line);
        line[nread] = '\0';
    }
    free(temp);
    int count = 0, status = 0;
    char **commands = split_commands(line, &count, ";");
    for (int i = 0; i < count; i++)
    {
        if ((nread = strlen(commands[i])) > 0)
        {
            int flag = 0;
            for (int k = nread - 1; k >= 0; k--)
            {
                if (commands[i][k] == ' ')
                {
                    continue;
                }
                else if (commands[i][k] == '&')
                {
                    commands[i][k] = '\0';
                    flag = EXIT_FAILURE;
                    break;
                }
                break;
            }
            if (flag)
            {
                int nargv = 0;
                char **argv = split_commands(commands[i], &nargv, " \r\n\t\a\b\f");
                background_execute(argv);
                continue;
            }
        }
        /**/
        status = chaining(homedir, commands[i]);
        /**/
    }
    free(commands);
    return status;
}

int split_exec(char *homedir, char *command)
{
    int size = 0;
    char **paths = NULL;
    if (command == NULL)
    {
        return EXIT_FAILURE;
    }
    paths = split_commands(command, &size, " \r\n\t\a\b\f");
    if (size <= 0)
    {
        return EXIT_FAILURE;
    }
    exit_code = bg_exec = 0;
    int status = execute(size, homedir, paths);
    /* clean up in the corresponding files of every feature */
    if (!bg_exec)
        free(paths);
    return status;
}
