//
// Created by ashwin on 28/09/20.
//
#include "shell.h"
#include "headers.h"

int chaining(char *homedir, char *command)
{
    int argc = strlen(command), status = 0,
        st = 0, len = 0, till_now = 0, number = 1;
    for (int i = 0; i <= argc; i++)
    {
        if (command[i] == '@' || command[i] == '$' ||
            command[i] == '\0')
        {
            char *line = (char *)calloc(PATH_MAX, sizeof(char));
            line = strncpy(line, &command[st], len);
            line[len] = '\0';
            if (number == 1 && till_now == 0)
            {
                status = piping(homedir, line);
                till_now |= exit_code > 0;
            }
            if (number == 0 && till_now > 0)
            {
                status = piping(homedir, line);
                till_now &= exit_code > 0;
            }
            number = command[i] == '@' ? 1 : 0;
            len = 0;
            st = i + 1;
            free(line);
            continue;
        }
        len++;
    }
    return status;
}
