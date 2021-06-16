#include "headers.h"
#include "shell.h"

/* implementation of options for the `ls` command */
void optionsCheck(int number, char **paths, int *val) {
    for (int i = 1; i < number; i++) {
        if (paths[i] != NULL &&
            paths[i][0] == '-') {
            if (strlen(paths[i]) == 3) {
                if (strcmp("al", paths[i] + 1) == 0 ||
                    strcmp("la", paths[i] + 1) == 0) {
                    val[0] = val[1] = 1;
                } else {
                    val[0] = val[1] = -1;
                }
            } else if (strlen(paths[i]) == 2) {
                if (strcmp("a", paths[i] + 1) == 0) {
                    val[1] = 1;
                } else if (strcmp("l", paths[i] + 1) == 0) {
                    val[0] = 1;
                } else {
                    val[0] = val[1] = -1;
                }
            }
        }
    }
}