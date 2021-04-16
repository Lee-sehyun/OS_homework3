#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>
#include <dirent.h>
#include <time.h>

#define tasks "./tasks"
#define results "./results"

int sumDir = 0, sumFile = 0, sumLine = 0;
int tasks_FIFO, results_FIFO;
char absolutePath[100];

struct timespec beginTime, endTime;
// const long long NANOS = 1000000000LL;

//잠금
int lock(int fd)
{
    struct flock filelock;

    filelock.l_type = F_WRLCK;
    filelock.l_whence = SEEK_SET;
    filelock.l_start = 0;
    filelock.l_len = 0;

    return fcntl(fd, F_SETLKW, &filelock);
}

//열기
int unlock(int fd)
{
    struct flock filelock;

    filelock.l_type = F_UNLCK;
    filelock.l_whence = SEEK_SET;
    filelock.l_start = 0;
    filelock.l_len = 0;

    return fcntl(fd, F_SETLKW, &filelock);
}

// 잠금 상태 알기
int getlock(int fd)
{
    struct flock filelock;

    filelock.l_type = F_WRLCK;
    filelock.l_whence = SEEK_SET;
    filelock.l_start = 0;
    filelock.l_len = 0;

    fcntl(fd, F_GETLK, &filelock);
    return (filelock.l_pid == fd) ? 1 : 0;
}

void strwrSub(char str[])
{
    int i = 0;
    while (str[i])
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = str[i] + 32;
        }
        i++;
    }
}

// 큐를 만들어준다.
char queue[100][100];
int first = 0, end = 0;

// 옵션을 받아 온다.
char **GetOption(int argc, char *argv[], int *processCnt, int *flag_c, int *flag_a, char **dir, int *keywordSize);
// 결과를 출력한다.
void result();
// process들을 킬한다.
void KillProcess(int sig);
// keyword에 대한 값을 출력한다.
void findKeyword(char *dirpath, char **keyword, int i, int keywordSize, int flag_c);

int main(int argc, char *argv[])
{
    int processCnt = 2, flag_c = 0, flag_a = 0, keywordSize = 0;
    char *dir, **keyword;
    int process[processCnt];
    clock_gettime(CLOCK_REALTIME, &beginTime);

    signal(SIGINT, (void *)KillProcess);

    keyword = GetOption(argc, argv, &processCnt, &flag_c, &flag_a, &dir, &keywordSize);

    if ((-1 == mkfifo(tasks, 0666)) || (-1 == mkfifo(results, 0666)))
    {
        perror("mkfifo() 실행에러");
        exit(1);
    }

    int K = processCnt;

    for (int i = 0; i < processCnt; i++)
    {
        process[i] = fork();
    }

    // int tasks_FIFO, results_FIFO;
    char d_path[100];
    for (int i = 0; i < processCnt; i++)
    {
        if (process[i] == 0)
        {
            signal(SIGINT, SIG_IGN);
            if (-1 == (tasks_FIFO = open(tasks, O_RDWR)))
            {
                perror("open() 실행에러");
                exit(1);
            }
            while (1)
            {
                memset(d_path, 0, 100);
                read(tasks_FIFO, d_path, 100);
                unlock(tasks_FIFO);
                // flock(tasks_FIFO, LOCK_UN);
                if (strlen(d_path) != 0)
                {
                    findKeyword(d_path, keyword, i, keywordSize, flag_c);
                    // kill(getppid(), SIGALRM);
                }
            }
            close(tasks_FIFO);
            // exit(1);
        }
    }

    if (-1 == (tasks_FIFO = open(tasks, O_WRONLY)) || -1 == (results_FIFO = open(results, O_RDWR)))
    {
        perror("open() 실행에러");
        exit(1);
    }

    write(tasks_FIFO, dir, strlen(dir) + 1);
    lock(tasks_FIFO);
    K--;
    while (K != processCnt)
    {
        // strcpy(d_path, "");
        memset(d_path, 0, 100);
        read(results_FIFO, d_path, 100);
        unlock(results_FIFO);
        if (strcmp(d_path, "end") == 0)
        {
            K++;
        }
        else if (strcmp(d_path, "findFile") == 0)
            sumFile++;
        else if (strcmp(d_path, "findLine") == 0)
        {
            sumLine++;
        }
        else
        {
            sumDir++;
            strcpy(queue[end++], d_path);
        }
        if (K > 0 && first != end)
        {
            write(tasks_FIFO, queue[first], 100);
            first++;
            K--;
        }
    }
    // printf("부모 실행\n");
    KillProcess(SIGINT);

    free(dir);
    for (int i = 0; i < keywordSize; i++)
    {
        free(keyword[i]);
    }
    free(keyword);
    return 0;
}

char **GetOption(int argc, char *argv[], int *processCnt, int *flag_c, int *flag_a, char **dir, int *keywordSize)
{
    int c;
    char **keyword = (char **)malloc(sizeof(char *) * 1);

    while ((c = getopt(argc, argv, "p:ca")) != -1)
    {
        switch (c)
        {
        case 'p':
            *processCnt = atoi(optarg);
            break;
        case 'c':
            *flag_c = 1;
            break;
        case 'a':
            *flag_a = 1;
            break;
        case '?':
            printf("Unknown flag : %c\n", optopt);
            break;
        }
    }

    if (*flag_a)
    {
        char *res = realpath(".", absolutePath);
    }

    c = 0;
    while (optind < argc)
    {
        // printf("%d\n", c);
        if (c == 0)
        {
            if (*flag_a)
            {
                *dir = (char *)malloc(strlen(absolutePath) + strlen(argv[optind]) + 2);
                strcpy(*dir, absolutePath);
                strcat(strcat(*dir, "/"), argv[optind++]);
            }
            else
            {
                *dir = (char *)malloc(strlen(argv[optind]) + 1);
                strcpy(*dir, argv[optind++]);
            }
            c++;
        }
        else
        {
            keyword = (char **)realloc(keyword, (*keywordSize + 1) * sizeof(char *));
            keyword[*keywordSize] = (char *)malloc(strlen(argv[optind]) + 1);
            strcpy(keyword[*keywordSize], argv[optind++]);
            if (*flag_c)
            {
                strwrSub(keyword[*keywordSize]);
            }
            (*keywordSize)++;
        }
    }
    return keyword;
}

// 결과를 출력한다.
void result()
{
    clock_gettime(CLOCK_REALTIME, &endTime);
    long time = (endTime.tv_sec - beginTime.tv_sec) + (endTime.tv_nsec - beginTime.tv_nsec);
    printf("\nFind %d directory, Find %d file, Find %d line, %lf times\n", sumDir, sumFile, sumLine, (double)time / 1000000000);
}

// process들을 킬한다.
void KillProcess(int sig)
{
    signal(SIGINT, SIG_IGN);
    result();
    kill(0, SIGINT);
    close(tasks_FIFO);
    close(results_FIFO);
    exit(0);
}

void findKeyword(char *dirpath, char **keyword, int i, int keywordSize, int flag_c)
{
    struct dirent *item;
    DIR *dp;
    dp = opendir(dirpath);
    // printf("열림!");
    struct stat buf;
    char str[100], sendFile[10] = "findFile", sendLine[10] = "findLine";

    if (-1 == (results_FIFO = open(results, O_WRONLY)))
    {
        perror("open() 실행에러");
        exit(1);
    }

    while (1)
    {
        item = readdir(dp);
        if (item == NULL)
            break;
        if (strcmp(item->d_name, ".") != 0 && strcmp(item->d_name, "..") != 0)
        {
            strcpy(str, "");
            strcat(strcat(strcat(str, dirpath), "/"), item->d_name);
            lstat(str, &buf);
            if (S_ISDIR(buf.st_mode))
            {
                // printf("%d에서 디렉토리 전달 %s\n", i, str);

                while (getlock(results_FIFO) == 1)
                    ;
                write(results_FIFO, str, strlen(str) + 1);
                lock(results_FIFO);
                sleep(2);
            }
            else
            {
                // printf("%d에서 파일 실행 %s\n", i, str);
                while (getlock(results_FIFO) == 1)
                    ;
                write(results_FIFO, sendFile, 9);
                lock(results_FIFO);
                sleep(2);
                FILE *fp = fopen(str, "r");
                char str2[100];
                i = 1;
                while (!feof(fp))
                {
                    fgets(str2, 100, fp);
                    if (flag_c)
                    {
                        strwrSub(str2);
                    }
                    for (int j = 0; j < keywordSize; j++)
                    {
                        if (strstr(str2, keyword[j]) != NULL)
                        {
                            while (getlock(results_FIFO) == 1)
                                ;
                            write(results_FIFO, sendLine, 9);
                            lock(results_FIFO);
                            sleep(2);
                            printf("%s:%d:%s", str, i, str2);
                            break;
                        }
                    }
                    i++;
                }
                fclose(fp);
            }
        }
    }
    closedir(dp);
    while (getlock(results_FIFO) == 1)
        ;
    write(results_FIFO, "end", 4);
    lock(results_FIFO);
    sleep(1);
    close(results_FIFO);
}

// 2432