#include <sys/stat.h>
#include "headers.h"
#include "shell.h"

int change_directory(char *homedir, char *path)
{
    if (strcmp(path, "-") == 0)
    {
        path = prevdir;
        printf("\r%s\n", prevdir);
        fflush(stdout);
    }
    path = tilde_path(path, homedir);
    struct stat stats;
    if (stat(path, &stats) == 0)
    {
        if (S_ISDIR(stats.st_mode))
        {
            char *tmp = tilde(current_directory(0), homedir);
            int status = chdir(path);
            if (status == -1)
            {
                free(tmp);
                exit_code = EXIT_FAILURE;
                perror("*** ERROR");
                return EXIT_FAILURE;
            }
            free(prevdir);
            prevdir = tmp;
            return EXIT_SUCCESS;
        }
        else
        {
            exit_code = EXIT_FAILURE;
            fprintf(stderr, "*** ERROR: %s is not a directory.\n", path);
            return EXIT_FAILURE;
        }
    }
    exit_code = EXIT_FAILURE;
    fprintf(stderr, "*** ERROR: %s doesn't exist.\n", path);
    return EXIT_FAILURE;
}