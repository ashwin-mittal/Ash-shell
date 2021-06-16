#include <fcntl.h>

#include "headers.h"
#include "shell.h"

void jobs(void) {
    int fd;
    struct subprocess *bg = getProc();
    char *buf = (char *)malloc(PATH_MAX);
    struct subprocess *run = (struct subprocess *)calloc(MAX_PROC, sizeof(struct subprocess));
    for (int i = 0; i < MAX_PROC; i++) {
        if (bg[i].pid != -1) {
            run[bg[i].ash_id - 1] = bg[i];
        }
    }
    for (int i = 0; i < MAX_PROC; i++) {
        if (run[i].pid <= 0) {
            continue;
        }
        sprintf(buf, "/proc/%d/stat", run[i].pid);
        fd = open(buf, O_RDONLY);
        if (fd < 0) {
            exit_code = EXIT_FAILURE;
            fprintf(stderr, "couldn't find relevant files for pid %d.\n", run[i].pid);
            perror("jobs");
            continue;
        }
        read(fd, buf, PATH_MAX);
        char *status, *procStat = split(2, buf);
        if (strcmp("T", procStat) == 0) {
            status = "Stopped";
        } else if (strcmp("Z", procStat) == 0) {
            status = "Zombie";
        } else {
            status = "Running";
        }
        printf("[%d] %s", i + 1, status);
        int idx = 0;
        while ((run[i].name)[idx] != NULL) {
            printf(" %s", (run[i].name)[idx]);
            idx++;
        }
        printf(" [%d]\n", run[i].pid);
        close(fd);
    }
    free(buf);
    free(run);
}