#include <sys/wait.h>
#include "headers.h"
#include "shell.h"
#include <signal.h>

int current = 0;

struct subprocess run[MAX_PROC + 5];

void background_execute(char **argv)
{
    pid_t cpid;
    if (current == MAX_PROC)
    {
        exit_code = EXIT_FAILURE;
        error("background processes limit reached");
        return;
    }
    /* forking a child process */
    cpid = fork();
    if (cpid == -1)
    {
        exit_code = EXIT_FAILURE;
        perror("fork");
        return;
    }
    if (cpid == 0)
    {
        /* Code executed by child */
        setpgid(0, 0);
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        if (execvp(*argv, argv) < 0)
        {
            error("execution failed");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        /* Code executed by parent */
        printf("PID\t--\t%d\n", cpid);
        for (int i = 0; i < MAX_PROC; i++)
        {
            if (run[i].pid == -1)
            {
                run[i].name = argv;
                run[i].pid = cpid;
                run[i].ash_id = ++current;
                break;
            }
        }
    }
}

void background_exit(int signum)
{
    /* Code executed by parent */
    int count = 0;
    for (int i = 0; i < MAX_PROC && current; i++)
    {
        if (run[i].pid == -1)
        {
            continue;
        }
        int wstatus;
        pid_t cpid;
        if ((cpid = waitpid(run[i].pid, &wstatus, WUNTRACED | WCONTINUED | WNOHANG)) <= 0 ||
            WIFSTOPPED(wstatus) || WIFCONTINUED(wstatus))
        {
            continue;
        }
        count++;
        if (WIFEXITED(wstatus))
        {
            fprintf(stderr, "\r\033[1;33m%s with pid %d exited normally, status = %d.\033[1;0m\n",
                    *(run[i].name), cpid, WEXITSTATUS(wstatus));
            free(run[i].name);
            run[i].name = NULL;
            run[i].pid = -1;
            current--;
        }
        else if (WIFSIGNALED(wstatus))
        {
            fprintf(stderr, "\r\033[1;31m%s with pid %d killed by signal %d.\033[1;0m\n",
                    *(run[i].name), cpid, WTERMSIG(wstatus));
            free(run[i].name);
            run[i].name = NULL;
            run[i].pid = -1;
            current--;
        }
        for (int j = 0; j < MAX_PROC; j++)
        {
            if (run[j].pid != -1 &&
                run[j].ash_id > run[i].ash_id)
            {
                run[j].ash_id--;
            }
        }
        run[i].ash_id = -1;
        fflush(stderr);
        fflush(stdout);
    }
    /* prompt after signal handling */
    if (count)
        display();
}

void bg_init(void)
{
    for (int i = 0; i < MAX_PROC; i++)
    {
        run[i].name = NULL;
        run[i].pid = -1;
    }
}

struct subprocess *getProc(void)
{
    return run;
}