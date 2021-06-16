#include <sys/stat.h>
#include <sys/types.h>

#include "headers.h"
#include "shell.h"

char *split(int number, char *buf) {
    char *token;
    token = strtok(buf, " \r\n\t\a\b\f");
    while (number-- && token != NULL) {
        token = strtok(NULL, " \r\n\t\a\b\f");
    }
    return token;
}

int pinfo(char *homedir, char *pid) {
    size_t len = 0;
    char *proc = (char *)malloc(PATH_MAX + 5);
    char *buf = NULL, *bufm = (char *)malloc(PATH_MAX);
    sprintf(proc, "/proc/%s/stat", pid);
    FILE *file;
    file = fopen(proc, "r");
    if (file == NULL) {
        exit_code = EXIT_FAILURE;
        fprintf(stderr, "process with pid %s not found\n", pid);
        return (EXIT_FAILURE);
    }
    printf("PID\t\t-- %s\n", pid);
    ssize_t nread;
    nread = getline(&buf, &len, file);
    if (nread == -1) {
        exit_code = EXIT_FAILURE;
        perror("getline");
        return (EXIT_FAILURE);
    }
    strcpy(bufm, buf);
    /* third arguement */
    char *status = split(2, buf);
    printf("PROCESS STATUS\t-- %s\n", status);
    /* twenty third arguement */
    char *memory = split(22, bufm);
    printf("MEMORY\t\t-- %s {VIRTUAL MEMORY}\n", memory);
    fclose(file);
    sprintf(proc, "/proc/%s/exe", pid);
    free(buf);
    buf = NULL;
    struct stat sb;
    ssize_t nbytes, bufsize;
    if (lstat(proc, &sb) == -1) {
        exit_code = EXIT_FAILURE;
        perror("lstat");
        return (EXIT_FAILURE);
    }
    bufsize = sb.st_size + 1;
    if (sb.st_size == 0) {
        bufsize = PATH_MAX;
    }
    buf = malloc(bufsize);
    if (buf == NULL) {
        exit_code = EXIT_FAILURE;
        perror("malloc");
        return (EXIT_FAILURE);
    }
    nbytes = readlink(proc, buf, bufsize);
    buf[nbytes] = '\0';
    if (nbytes == -1) {
        exit_code = EXIT_FAILURE;
        perror("readlink");
        return (EXIT_FAILURE);
    }
    buf = tilde(buf, homedir);
    printf("EXECUTABLE PATH\t-- %.*s\n", (int)nbytes, buf);
    free(buf);
    free(proc);
    return (EXIT_SUCCESS);
}
