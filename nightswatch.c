#include <termios.h>
#include <time.h>

#include "headers.h"
#include "shell.h"

int split_cnt(char *buf) {
    int count = 0;
    char *token;
    token = strtok(buf, " \r\n\t\a\b\f");
    while (token != NULL) {
        count++;
        token = strtok(NULL, " \r\n\t\a\b\f");
    }
    return count;
}

void nightswatch(int number) {
    FILE *file;
    file = fopen("/proc/interrupts", "r");
    if (file == NULL) {
        exit_code = EXIT_FAILURE;
        fprintf(stderr, "*** ERROR: execution failed\n");
        return;
    }
    size_t len = 0;
    char *buf = NULL;
    ssize_t nread;
    nread = getline(&buf, &len, file);
    if (nread == -1) {
        exit_code = EXIT_FAILURE;
        perror("getline");
        return;
    }
    int cpu_cnt = split_cnt(buf);
    char str[100];
    for (int i = 0; i < cpu_cnt; i++) {
        sprintf(str, "CPU-%d", i + 1);
        printf("%10s", str);
    }
    printf("\n");
    /**/
    struct termios info;
    tcgetattr(0, &info);
    info.c_lflag &= ~ICANON;
    info.c_cc[VMIN] = 0;
    info.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &info);
    /**/
    int ch, status = 0;
    while (!status) {
        fclose(file);
        file = fopen("/proc/interrupts", "r");
        for (int i = 0; i < 3; i++) {
            len = 0;
            free(buf);
            buf = NULL;
            nread = getline(&buf, &len, file);
            if (nread == -1) {
                /**/
                tcgetattr(0, &info);
                info.c_lflag |= ICANON;
                tcsetattr(0, TCSANOW, &info);
                /**/
                exit_code = EXIT_FAILURE;
                perror("getline");
                return;
            }
        }
        char **interrupts = (char **)malloc(cpu_cnt * sizeof(char *));
        if (interrupts == NULL) {
            /**/
            tcgetattr(0, &info);
            info.c_lflag |= ICANON;
            tcsetattr(0, TCSANOW, &info);
            /**/
            exit_code = EXIT_FAILURE;
            fprintf(stderr, "*** ERROR: memory allocation failed\n");
            return;
        }
        int count = 0;
        char *token;
        token = strtok(buf, " \r\n\t\a\b\f");
        token = strtok(NULL, " \r\n\t\a\b\f");
        while (count++ < cpu_cnt && token != NULL) {
            interrupts[count - 1] = token;
            token = strtok(NULL, " \r\n\t\a\b\f");
        }
        for (int i = 0; i < cpu_cnt; i++) {
            printf("%10s", interrupts[i]);
        }
        printf("\n");
        fflush(stdout);
        free(interrupts);
        /* delay(number); */
        long int milli_seconds = number * CLOCKS_PER_SEC;
        clock_t start_time = clock();
        while (clock() < start_time + milli_seconds) {
            if ((ch = getchar()) == 'q') {
                status = EXIT_FAILURE;
                break;
            }
            if (ch < 0) {
                clearerr(stdin);
            }
        }
        /* condition chech in loop */
        if ((ch = getchar()) == 'q') {
            status = EXIT_FAILURE;
            break;
        }
        if (ch < 0) {
            clearerr(stdin);
        }
    }
    /**/
    tcgetattr(0, &info);
    info.c_lflag |= ICANON;
    tcsetattr(0, TCSANOW, &info);
    /**/
    free(buf);
}