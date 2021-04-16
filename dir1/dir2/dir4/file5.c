#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <dirent.h>

#define tasks "./tasks"
#define results "./results"

void findKeyword(char *dirpath, char *keyword, int i);

int main(int argc, char *argv[])
{
    int c, processCnt = 2, flag_c = 0, flag_a = 0;
    char *dir, *keyword;

    while ((c = getopt(argc, argv, "p:ca")) != -1)
    {
        switch (c)
        {
        case 'p':
            processCnt = atoi(optarg);
            break;
        case 'c':
            flag_c = 1;
            break;
        case 'a':
            flag_a = 1;
            break;
        case '?':
            printf("Unknown flag : %c\n", optopt);
            break;
        }
    }

    printf("%d, %d, %d\n", processCnt, flag_c, flag_a);
    int k = 0;
    while (optind < argc)
    {
        if (k == 0)
        {
            dir = (char *)malloc(strlen(argv[optind]) + 1);
            strcpy(dir, argv[optind++]);
            k++;
        }
        else if (k == 1)
        {
            keyword = (char *)malloc(strlen(argv[optind]) + 1);
            strcpy(keyword, argv[optind++]);
            k++;
        }
    }
    printf("%s, %s\n", dir, keyword);

    if ((-1 == mkfifo(tasks, 0666)) || (-1 == mkfifo(results, 0666)))
    {
        perror("mkfifo() 실행에러");
        exit(1);
    }

    int process[processCnt];
    for (int i = 0; i < processCnt; i++)
    {
        process[i] = fork();
    }

    // write(pipes[1], "hello", 6);
    // write(pipes[1], "world", 6);
    // write(pipes[1], "handong", 8);
    // printf("부모 실행\n");

    int fd;
    for (int i = 0; i < processCnt; i++)
    {
        if (process[i] == 0)
        {
            if (-1 == (fd = open(tasks, O_RDWR)))
            {
                perror("open() 실행에러");
                exit(1);
            }
            char d_path[100];
            memset(d_path, 0, 100);
            while (1)
            {
                if (read(fd, d_path, 100) != -1)
                {
                    findKeyword(d_path, keyword, i);
                }
            }
            close(fd);
            // exit(1);
        }
    }

    if (-1 == (fd = open(tasks, O_WRONLY)))
    {
        perror("open() 실행에러");
        exit(1);
    }
    write(fd, dir, strlen(dir) + 1);
    sleep(1);
    // write(fd, "world", 6);
    // sleep(1);
    // write(fd, "handong", 8);
    // sleep(1);
    printf("부모 실행\n");
    close(fd);

    free(dir);
    free(keyword);
    for (int i = 0; i < processCnt; i++)
    {
        kill(process[i], SIGINT);
    }
    return 0;
}

void findKeyword(char *dirpath, char *keyword, int i)
{
    struct dirent *item;
    DIR *dp;
    dp = opendir(dirpath);
    struct stat buf;
    int fd;
    char str[100];

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
                if (-1 == (fd = open(tasks, O_WRONLY)))
                {
                    perror("open() 실행에러");
                    exit(1);
                }
                printf("%d에서 디렉토리 전달 %s\n", i, str);
                write(fd, str, strlen(str) + 1);
                close(fd);
            }
            else
            {
                printf("%d에서 파일 실행 %s\n", i, str);
            }
        }
    }
    closedir(dp);
}

/* 부모 프로세스에서 첫 폴더을 fifo에 주고, 
자식 프로세스에서는 폴더를 열어서 만약 폴더면 다시 fifo에 주고 
파일이면 파일을 열어서 키워드와 같은게 있는지 확인한다.
*/