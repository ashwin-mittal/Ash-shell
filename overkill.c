#include "shell.h"
#include "headers.h"
#include <signal.h>

void overkill(void)
{
    blockSIGCHLD();
    struct subprocess *run = getProc();
    for (int i = 0; i < MAX_PROC; i++)
    {
        if (run[i].pid == -1)
        {
            continue;
        }
        if (kill(run[i].pid, SIGKILL) < 0)
        {
            exit_code = EXIT_FAILURE;
            perror("overkill");
        }
    }
    unblockSIGCHLD();
}