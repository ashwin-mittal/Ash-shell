#include "headers.h"
#include "shell.h"

void setenv_ash(char *name, char *val) {
    if (val == NULL) {
        val = "";
    }
    if (setenv(name, val, 1) < 0) {
        exit_code = EXIT_FAILURE;
        perror("setenv");
    }
}

void unsetenv_ash(char *name) {
    if (unsetenv(name) < 0) {
        exit_code = EXIT_FAILURE;
        perror("unsetenv");
    }
}