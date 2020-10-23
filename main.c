#include "headers.h"
#include "shell.h"
#include <signal.h>

char *username, *homedir;

char *getUser(void)
{
    return username;
}

char *getHomeDir(void)
{
    return homedir;
}

int exit_code = 0, bg_exec = 0;

char *prevdir = NULL;

int main(void)
{
    /* clears screen */
    printf("\e[1;1H\e[2J");
    /* signal handler for Ctrl + C and Ctrl + Z */
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    /* signal handler for background processes */
    signal(SIGCHLD, background_exit);
    history();
    int status;
    username = (char *)malloc(LOGIN_NAME_MAX);
    /* homedir of the shell */
    homedir = (char *)malloc(PATH_MAX);
    status = getlogin_r(username, LOGIN_NAME_MAX);
    if (status)
    {
        perror("getlogin_r");
        return EXIT_FAILURE;
    }
    homedir = current_directory(0);
    /**/
    prevdir = tilde(current_directory(0), homedir);
    /**/
    /* backgound processes data structure initialization */
    bg_init();
    while (!status)
    {
        /* load the configuration */
        display();

        /* ash-shell REPL */
        status = parse(homedir);
    }
    return EXIT_SUCCESS;
}