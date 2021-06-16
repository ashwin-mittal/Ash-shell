//
// Created by ashwin on 24/09/20.
//
#include <signal.h>
#include <sys/wait.h>

#include "headers.h"
#include "shell.h"

static void toggleSIGCHLDBlock(int how) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(how, &mask, NULL);
}

void blockSIGCHLD(void) {
    toggleSIGCHLDBlock(SIG_BLOCK);
}

void unblockSIGCHLD(void) {
    toggleSIGCHLDBlock(SIG_UNBLOCK);
}

void fg(int ash_id) {
    int pid = -1, idx, wstatus;
    struct subprocess *run = getProc();
    for (int i = 0; i < MAX_PROC; i++) {
        if (run[i].pid != -1 &&
            run[i].ash_id == ash_id) {
            idx = i;
            pid = run[i].pid;
            break;
        }
    }
    if (pid == -1) {
        error("invalid job number");
        return;
    }
    blockSIGCHLD();
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    /**/
    tcsetpgrp(0, getpgid(pid));
    kill(pid, SIGCONT);
    waitpid(pid, &wstatus, WUNTRACED);
    tcsetpgrp(0, getpgrp());
    if (WIFEXITED(wstatus) || WIFSIGNALED(wstatus)) {
        free(run[idx].name);
        run[idx].name = NULL;
        run[idx].pid = -1;
        current--;
        for (int j = 0; j < MAX_PROC; j++) {
            if (run[j].pid != -1 &&
                run[j].ash_id > run[idx].ash_id) {
                run[j].ash_id--;
            }
        }
        run[idx].ash_id = -1;
    }
    exit_code = WEXITSTATUS(wstatus);
    printf("%d\n", exit_code);
    /**/
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    unblockSIGCHLD();
}