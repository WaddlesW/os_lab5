/*
实现简单的 shell 以及 shell 命令以展示实现的功能
*/

#include "ext2.h"
#include "./ext2.c"

#define LS 0
#define MKDIR 1
#define TOUCH 2
#define CP 3
#define SHUTDOWN 4
// #include "cmd.h"
// #include "./cmd.c"
// #include "mystruct.h"


int getcmd(char *buf, int num)
{
    printf("$ ");
    memset(buf, 0, num);
    fgets(buf, num, stdin);
    if (buf[0] == 0)
        return -1;
    return 0;
}

int parse_cmd(char *buf)
{
    char path_buf[40];      //路径缓冲区
    char command_buf[20];       //命令缓冲区
    int mode = 5;   //任务模式

    int cnt = 0;
    while (cnt <= strlen(buf) && buf[cnt] != ' ' && buf[cnt] != '\n')   cnt++;
    strncpy(command_buf, buf, cnt);
    command_buf[cnt] = '\0';

    if (strcmp("ls", command_buf) == 0)
        mode = LS;
    else if (strcmp("mkdir", command_buf) == 0)
        mode = MKDIR;
    else if (strcmp(command_buf, "touch") == 0)
        mode = TOUCH;
    else if (strcmp(command_buf, "cp") == 0)
        mode = CP;
    else if (strcmp(command_buf, "shutdown") == 0)
        mode = SHUTDOWN;
    
    switch (mode)
    {
    case LS:
        strcpy(path_buf, buf + cnt + 1);
        path_buf[strlen(path_buf) - 1] = '\0';
        ls(path_buf);
        break;

    case MKDIR:
        strcpy(path_buf, buf + cnt + 1);
        path_buf[strlen(path_buf) - 1] = '\0';
        mkdir(path_buf);
        break;

    case TOUCH:
        strcpy(path_buf, buf + cnt + 1);
        path_buf[strlen(path_buf) - 1] = '\0';
        touch(path_buf);
        break;

    case CP:
        strcpy(path_buf, buf + cnt + 1);
        path_buf[strlen(path_buf) - 1] = '\0';
        while (path_buf[cnt] != ' ')
            cnt++;
        path_buf[cnt] = '\0';
        cp(path_buf, path_buf + cnt + 1);
        break;

    case SHUTDOWN:
        shutdown();
        return 0;

    default:
        printf("Invalid input!\n");
        return 1;
    }

    return 1;
}
