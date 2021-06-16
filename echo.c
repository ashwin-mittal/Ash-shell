#include "headers.h"
#include "shell.h"

void echo(int number, char **str) {
    for (int i = 1; i < number; i++) {
        if (str[i] != NULL) {
            printf("%s ", str[i]);
        }
    }
    printf("\n");
}