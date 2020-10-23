#include "shell.h"
#include "headers.h"
#include <sys/wait.h>

int piping(char *homedir, char *command)
{
    /**/
    int nargv = 0, in = dup(STDIN_FILENO),
        out = dup(STDOUT_FILENO);
    /**/
    int num = strlen(command);
    for (int i = 0; i < num; i++)
    {
        if (command[i] == ' ')
        {
            continue;
        }
        else if (command[i] == '|')
        {
            error("command not found");
            return EXIT_SUCCESS;
        }
        break;
    }
    for (int i = num - 1; i >= 0; i--)
    {
        if (command[i] == ' ')
        {
            continue;
        }
        else if (command[i] == '|')
        {
            error("command not found");
            return EXIT_SUCCESS;
        }
        break;
    }
    char **commands = split_commands(command, &nargv, "|");
    int filep[2];
    int status = 0;
    if (nargv <= 1)
    {
        redirection(homedir, command);
        free(commands);
        return EXIT_SUCCESS;
    }
    for (int i = 0; i < nargv; i++)
    {
        if (i > 0)
        {
            if (dup2(filep[0], STDIN_FILENO) < 0)
            {
                exit_code = EXIT_FAILURE;
                perror("Unable to duplicate file descriptor");
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_SUCCESS;
            }
            close(filep[0]);
        }
        if (i < nargv - 1)
        {
            if (pipe(filep) < 0)
            {
                exit_code = EXIT_FAILURE;
                perror("Could not execute pipe");
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_SUCCESS;
            }
            if (dup2(filep[1], STDOUT_FILENO) < 0)
            {
                exit_code = EXIT_FAILURE;
                perror("Unable to duplicate file descriptor");
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_SUCCESS;
            }
            close(filep[1]);
        }
        pid_t cpid = fork();
        if (cpid == -1)
        {
            perror("fork");
            exit_code = EXIT_FAILURE;
            dup2(in, STDIN_FILENO);
            dup2(out, STDOUT_FILENO);
            return EXIT_SUCCESS;
        }
        if (cpid == 0)
        {
            /* Code executed by child */
            status = redirection(homedir, commands[i]);
            exit(status);
        }
        else
        {
            /* Code executed by parent */
            waitpid(cpid, &status, WUNTRACED);
            dup2(in, STDIN_FILENO);
            dup2(out, STDOUT_FILENO);
            if (WEXITSTATUS(status))
            {
                exit_code = EXIT_FAILURE;
                return EXIT_SUCCESS;
            }
        }
    }
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    free(commands);
    return status;
}