#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>

#include "headers.h"
#include "shell.h"

char *tilde_path(char *path, char *homedir) {
    if (path[0] == '~') {
        char *p_new = (char *)malloc(PATH_MAX + 5);
        strcpy(p_new, homedir);
        if (strlen(path) > 1) {
            strcat(p_new, &path[1]);
        }
        path = p_new;
    }
    return path;
}

int list(int number, char *homedir, char **paths) {
    int actual[2] = {0};
    optionsCheck(number, paths, actual);
    if (actual[0] == -1 && actual[1] == -1) {
        return EXIT_FAILURE;
    }
    int count = 0;
    for (int i = 1; i < number; i++) {
        if (paths[i] == NULL ||
            paths[i][0] == '-') {
            continue;
        }
        char *path = paths[i];
        path = tilde_path(path, homedir);
        struct stat stats;
        if (count) {
            printf("\n");
        }
        if (stat(path, &stats) < 0) {
            exit_code = EXIT_FAILURE;
            fprintf(stderr, "*** ERROR: directory doesn't exist\n");
            return EXIT_SUCCESS;
        }
        count++;
        DIR *d;
        struct dirent *dir;
        d = opendir(path);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (actual[1] == 0 &&
                    (!(strcmp(".", dir->d_name) &&
                       strcmp("..", dir->d_name) &&
                       (dir->d_name)[0] != '.') ||
                     !strcmp("/", dir->d_name))) {
                    continue;
                }
                if (actual[0] == 0) {
                    printf("\r\033[1;36m%s\033[1;0m\n", dir->d_name);
                    continue;
                }
                char *name = (char *)malloc(PATH_MAX + PATH_MAX + 5);
                strcpy(name, path);
                if (path[strlen(path) - 1] != '/') {
                    strcat(name, "/");
                }
                strcat(name, dir->d_name);
                stat(name, &stats);
                printf("\033[1;35m");
                printf((S_ISDIR(stats.st_mode)) ? "d" : "-");
                /* permissions */
                printf((stats.st_mode & S_IRUSR) ? "r" : "-");
                printf((stats.st_mode & S_IWUSR) ? "w" : "-");
                printf((stats.st_mode & S_IXUSR) ? "x" : "-");
                printf((stats.st_mode & S_IRGRP) ? "r" : "-");
                printf((stats.st_mode & S_IWGRP) ? "w" : "-");
                printf((stats.st_mode & S_IXGRP) ? "x" : "-");
                printf((stats.st_mode & S_IROTH) ? "r" : "-");
                printf((stats.st_mode & S_IWOTH) ? "w" : "-");
                printf((stats.st_mode & S_IXOTH) ? "x" : "-");
                printf("\033[1;0m");
                /* User */
                struct passwd *usr = getpwuid(stats.st_uid);
                /* Group */
                struct group *grp = getgrgid(stats.st_gid);
                /* Last Modified Time */
                const double YEAR = 365 * 24 * 3600;
                struct tm *time_struct;
                time_t now, modified = stats.st_mtime;
                char buf[80];
                time(&now);
                time_struct = localtime(&modified);
                double time_diff = difftime(now, modified);
                char format[15] = " %b %d ";
                /* Display files that are over a year old using year rather than time */
                if (time_diff > YEAR)
                    strcat(format, " %Y");
                else
                    strcat(format, "%R");
                strftime(buf, sizeof(buf), format, time_struct);
                printf("\r\t%3ld\t%s\t%s\t%7ld\t%s\t\033[1;36m%s\033[1;0m\n",
                       stats.st_nlink, usr->pw_name, grp->gr_name, stats.st_size, buf, dir->d_name);
                free(name);
            }
            closedir(d);
        }
    }
    return EXIT_SUCCESS;
}