#ifndef _SHELL_H_
#define _SHELL_H_

/* macros */
#define PATH_MAX 4096
#define LOGIN_NAME_MAX 256
#define HOST_NAME_MAX 64
/* history max size that can be stored */
#define MAX_HISTORY_SIZE 20
#define MAX_PROC 1024

struct subprocess {
    char **name;
    int pid, ash_id;
};

extern int exit_code,
    bg_exec, current;

extern char *prevdir;

/* functions shared between the files */
void reset(void);

void history(void);

void addHistory(char *);

void printHistory(int);

void StoreHistory(void);

int display(void);

char *getUser(void);

char *getHomeDir(void);

void echo(int, char **);

int pinfo(char *, char *);

int parse(char *);

char *tilde(char *, char *);

int list(int, char *, char **);

char *split(int, char *);

void setenv_ash(char *, char *);

void unsetenv_ash(char *);

int change_directory(char *, char *);

void optionsCheck(int, char **, int *);

char **split_commands(char *, int *, char *);

void kjob(int, int);

void error(char *);

int redirection(char *, char *);

int piping(char *, char *);

void blockSIGCHLD(void);

void unblockSIGCHLD(void);

int chaining(char *, char *);

int split_exec(char *, char *);

int execute(int, char *, char **);

char *tilde_path(char *, char *);

void background_execute(char **);

struct subprocess *getProc(void);

char *current_directory(int);

void fg(int);

void background_exit(int);

void overkill(void);

void nightswatch(int);

void bg_init(void);

void jobs(void);

#endif
