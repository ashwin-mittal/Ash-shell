#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "headers.h"
#include "shell.h"

/* to print the errors on the strderr */
void error(char *error) {
    exit_code = EXIT_FAILURE;
    fprintf(stderr, "*** ERROR: %s\n", error);
    fflush(stderr);
}

int isNumber(char *tmp) {
    for (int i = 0; i < strlen(tmp); i++) {
        if (tmp[i] > '9' || tmp[i] < '0') {
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

void foreground(char **argv) {
    int status;
    pid_t cpid = fork();
    if (cpid == -1) {
        exit_code = EXIT_FAILURE;
        perror("fork");
        return;
    }
    if (cpid == 0) {
        setpgid(0, 0);
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        if (execvp(*argv, argv) < 0) {
            error("execution failed");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    } else {
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(0, cpid);
        waitpid(cpid, &status, WUNTRACED);
        tcsetpgrp(0, getpgid(0));
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        if (WIFSTOPPED(status)) {
            if (current == MAX_PROC) {
                error("maximum background processes");
                return;
            }
            printf("PID\t--\t%d\n", cpid);
            exit_code = bg_exec = EXIT_FAILURE;
            struct subprocess *run = getProc();
            for (int i = 0; i < MAX_PROC; i++) {
                if (run[i].pid == -1) {
                    run[i].pid = cpid;
                    run[i].name = argv;
                    run[i].pid = cpid;
                    run[i].ash_id = ++current;
                    break;
                }
            }
        }
        exit_code = WEXITSTATUS(status);
    }
}

/* execute the commands */
int execute(int size, char *homedir, char **paths) {
    int status = 0;
    if (size <= 2 && strcmp(paths[0], "cd") == 0) {
        if (size > 1) {
            change_directory(homedir, paths[1]);
        } else {
            change_directory("~", homedir);
        }
    } else if (size <= 2 && strcmp(paths[0], "pinfo") == 0) {
        if (size > 1) {
            pinfo(homedir, paths[1]);
        } else {
            char *pid = (char *)malloc(PATH_MAX);
            sprintf(pid, "%d", getpid());
            pinfo(homedir, pid);
            free(pid);
        }
    } else if (size == 1 && strcmp(paths[0], "pwd") == 0) {
        current_directory(1);
    } else if (strcmp(paths[0], "echo") == 0) {
        echo(size, paths);
    } else if (strcmp(paths[0], "nightswatch") == 0) {
        if (size < 4) {
            status = EXIT_FAILURE;
        }
        if (strcmp(paths[1], "-n") != 0) {
            status = EXIT_FAILURE;
        }
        if (isNumber(paths[2]) == 0) {
            status = EXIT_FAILURE;
        }
        if (strcmp(paths[3], "interrupt") != 0) {
            status = EXIT_FAILURE;
        }
        if (status == 0) {
            nightswatch(atoi(paths[2]));
        }
    } else if (strcmp(paths[0], "history") == 0) {
        if (size == 1) {
            printHistory(MAX_HISTORY_SIZE / 2);
        } else if (size == 2 && isNumber(paths[1])) {
            printHistory(atoi(paths[1]));
        } else {
            status = EXIT_FAILURE;
        }
    } else if (strcmp(paths[0], "ls") == 0) {
        int arguements = 0;
        for (int i = 1; i < size; i++) {
            if (paths[i][0] != '-') {
                arguements = EXIT_FAILURE;
                break;
            }
        }
        if (arguements == 0) {
            char path[] = ".";
            paths[size] = path;
            size++;
        }
        status = list(size, homedir, paths);
    } else if (size == 1 && strcmp(paths[0], "jobs") == 0) {
        jobs();
    } else if (size == 1 && strcmp(paths[0], "overkill") == 0) {
        overkill();
    } else if (strcmp(paths[0], "setenv") == 0) {
        if (size == 1 || size > 3) {
            error("command not found");
        } else {
            setenv_ash(paths[1], paths[2]);
        }
    } else if (strcmp(paths[0], "unsetenv") == 0) {
        if (size == 1 || size > 2) {
            error("command not found");
        } else {
            unsetenv_ash(paths[1]);
        }
    } else if (strcmp(paths[0], "kjob") == 0) {
        if (size != 3 || !isNumber(paths[1]) || !isNumber(paths[2])) {
            error("command not found");
        } else {
            kjob(atoi(paths[1]), atoi(paths[2]));
        }
    } else if (strcmp(paths[0], "fg") == 0) {
        if (size != 2 || !isNumber(paths[1])) {
            error("command not found");
        } else {
            fg(atoi(paths[1]));
        }
    } else if (strcmp(paths[0], "bg") == 0) {
        if (size != 2 || !isNumber(paths[1])) {
            error("command not found");
        } else {
            kjob(atoi(paths[1]), SIGCONT);
        }
    } else {
        paths[size] = NULL;
        foreground(paths);
        return EXIT_SUCCESS;
    }
    if (status) {
        error("command doesn't exist");
    }
    return EXIT_SUCCESS;
}