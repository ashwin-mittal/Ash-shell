#include <signal.h>

#include "headers.h"
#include "shell.h"

void kjob(int ash_id, int signum) {
    blockSIGCHLD();
    int pid = -1;
    struct subprocess *run = getProc();
    for (int i = 0; i < MAX_PROC; i++) {
        if (run[i].pid != -1 &&
            run[i].ash_id == ash_id) {
            pid = run[i].pid;
            break;
        }
    }
    if (pid == -1) {
        error("invalid job number");
        return;
    }
    if (kill(pid, signum) < 0) {
        exit_code = EXIT_FAILURE;
        perror("kjob");
    }
    unblockSIGCHLD();
}