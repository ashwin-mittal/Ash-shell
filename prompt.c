#include "headers.h"
#include "shell.h"

void blue(void)
{
    printf("\033[1;34m");
}

void green(void)
{
    printf("\033[1;32m");
}

void reset(void)
{
    printf("\033[0m");
}

char *tilde(char *buf, char *homedir)
{
    if (strlen(homedir) <= strlen(buf) &&
        strncmp(homedir, buf, strlen(homedir)) == 0)
    {
        sprintf(buf, "~%s", &buf[strlen(homedir)]);
    }
    return buf;
}

int process_current(char *homedir)
{
    /* colour change */
    blue();
    int status;
    char *pwd = current_directory(0);
    pwd = tilde(pwd, homedir);
    status = printf("%s", pwd);
    green();
    status = printf(">:: ");
    reset();
    free(pwd);
    return status;
}

int display(void)
{
    int result;
    char *homedir = getHomeDir(), *username = getUser();
    char *hostname = (char *)malloc(HOST_NAME_MAX + 5);
    result = gethostname(hostname, HOST_NAME_MAX);
    if (result)
    {
        perror("gethostname");
        return EXIT_FAILURE;
    }
    green();
    if (exit_code)
    {
        exit_code = 0;
        result = printf("\r:'(");
    }
    else
    {
        result = printf("\r:')");
    }
    result = printf("<%s@%s:", username, hostname);
    result = process_current(homedir);
    if (result < 0)
    {
        perror("printf");
        return EXIT_FAILURE;
    }
    free(hostname);
    fflush(stdout);
    fflush(stderr);
    return EXIT_SUCCESS;
}