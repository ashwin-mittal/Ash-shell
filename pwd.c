#include "headers.h"
#include "shell.h"

char *current_directory(int number) {
    char *pwd = (char *)malloc(PATH_MAX + 5);
    getcwd(pwd, PATH_MAX);
    if (number > 0) {
        printf("%s\n", pwd);
    }
    return pwd;
}