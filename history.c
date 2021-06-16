#include "headers.h"
#include "shell.h"

int en = 0, size = 0;

char **buf = NULL;

void history(void) {
    buf = (char **)calloc(MAX_HISTORY_SIZE, sizeof(char *));
    FILE *file;
    file = fopen("history.txt", "r");
    size_t len = 0;
    if (file != NULL) {
        while (getline(&buf[en], &len, file) != -1) {
            len = 0;
            size++;
            en++;
        }
        en %= MAX_HISTORY_SIZE;
        fclose(file);
    }
}

void addHistory(char *command) {
    if (size == MAX_HISTORY_SIZE) {
        size--;
        free(buf[en]);
    }
    size++;
    command[strlen(command)] = '\n';
    buf[en] = (char *)malloc(strlen(command) + 1);
    strncpy(buf[en], command, strlen(command) + 1);
    en = (en + 1) % MAX_HISTORY_SIZE;
}

void printHistory(int number) {
    int start;
    if (number > MAX_HISTORY_SIZE) {
        number = MAX_HISTORY_SIZE;
    }
    start = (en - number) % MAX_HISTORY_SIZE;
    start = (start + MAX_HISTORY_SIZE) % MAX_HISTORY_SIZE;
    for (int i = start; number; i = (i + 1) % MAX_HISTORY_SIZE, number--) {
        if (buf[i] != NULL)
            printf("%s", buf[i]);
    }
    fflush(stdout);
}

void StoreHistory(void) {
    FILE *file;
    file = fopen(tilde_path("~/history.txt", getHomeDir()), "w");
    if (file == NULL) {
        fprintf(stderr, "*** ERROR: unable to store history.\n");
        return;
    }
    int start = (en - size) % MAX_HISTORY_SIZE;
    start = (start + MAX_HISTORY_SIZE) % MAX_HISTORY_SIZE;
    for (int i = start; size; i = (i + 1) % MAX_HISTORY_SIZE, size--) {
        fwrite(buf[i], strlen(buf[i]), 1, file);
    }
    fclose(file);
}
