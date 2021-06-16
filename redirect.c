#include <fcntl.h>

#include "headers.h"
#include "shell.h"

int redirection(char *homedir, char *command) {
    int status;
    /**/
    int idx, num = strlen(command);
    char *line = (char *)malloc(num + 1);
    for (idx = 0; idx < num && command[idx] != '<' && command[idx] != '>'; idx++) {
        line[idx] = command[idx];
    }
    line[idx] = '\0';
    /**/
    int argc = 0, write_fd, read_fd, in = dup(STDIN_FILENO), out = dup(STDOUT_FILENO);
    char **commands = split_commands(command, &argc, " \r\n\t\a\b\f");
    for (int i = argc - 1; i >= 0; i--) {
        if (strcmp(commands[i], ">") == 0) {
            if (i == argc - 1) {
                error("invalid command");
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_FAILURE;
            }
            write_fd = open(commands[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (write_fd < 0) {
                exit_code = EXIT_FAILURE;
                perror(commands[i + 1]);
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_FAILURE;
            }
            if (dup2(write_fd, STDOUT_FILENO) < 0) {
                exit_code = EXIT_FAILURE;
                perror("unable to duplicate file descriptor");
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_FAILURE;
            }
            close(write_fd);
            break;
        }
        if (strcmp(commands[i], ">>") == 0) {
            if (i == argc - 1) {
                error("invalid command");
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_FAILURE;
            }
            write_fd = open(commands[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (write_fd < 0) {
                exit_code = EXIT_FAILURE;
                perror(commands[i + 1]);
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_FAILURE;
            }
            if (dup2(write_fd, STDOUT_FILENO) < 0) {
                exit_code = EXIT_FAILURE;
                perror("unable to duplicate file descriptor");
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_FAILURE;
            }
            close(write_fd);
            break;
        }
    }
    for (int i = argc - 1; i >= 0; i--) {
        if (strcmp(commands[i], "<") == 0) {
            if (i == argc - 1) {
                error("invalid command");
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_FAILURE;
            }
            read_fd = open(commands[i + 1], O_RDONLY);
            if (read_fd < 0) {
                exit_code = EXIT_FAILURE;
                perror(commands[i + 1]);
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_FAILURE;
            }
            if (dup2(read_fd, STDIN_FILENO) < 0) {
                exit_code = EXIT_FAILURE;
                perror("unable to duplicate file descriptor");
                dup2(in, STDIN_FILENO);
                dup2(out, STDOUT_FILENO);
                return EXIT_FAILURE;
            }
            close(read_fd);
            break;
        }
    }
    status = split_exec(homedir, line);
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    if (!bg_exec)
        free(line);
    bg_exec = 0;
    free(commands);
    return (status || exit_code);
}