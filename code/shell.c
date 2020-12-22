/*
实现简单的 shell 以及 shell 命令以展示实现的功能
*/

#include "ext2.h"
#include "./ext2.c"
// #include "cmd.h"
// #include "./cmd.c"
// #include "mystruct.h"


int getcmd(char *buf, int nbuf)
{
    printf("$ ");
    memset(buf, 0, nbuf);
    fgets(buf, nbuf, stdin);
    if (buf[0] == 0)
        return -1;
    return 0;
}

int parse_cmd(char *buf)
{
    char path_buf[40];
    char command_buf[20];
    int mod = 4;
    int flag = 0;
    int i;
    for (i = 0; i <= strlen(buf); i++)
    {
        if (buf[i] == ' ' || buf[i] == '\n')
        {
            strncpy(command_buf, buf, i);
            command_buf[i] = '\0';
            break;
        }
    }
    if (strcmp("ls", command_buf) == 0)
        mod = 2;
    else if (strcmp("mkdir", command_buf) == 0)
        mod = 0;
    else if (strcmp(command_buf, "touch") == 0)
        mod = 1;
    else if (strcmp(command_buf, "cp") == 0)
        mod = 3;
    switch (mod)
    {
    case 0:
        strcpy(path_buf, buf + i + 1);
        path_buf[strlen(path_buf) - 1] = '\0';
        mkdir(path_buf);
        break;
    case 1:
        strcpy(path_buf, buf + i + 1);
        path_buf[strlen(path_buf) - 1] = '\0';
        touch(path_buf);
        break;
    case 2:
        strcpy(path_buf, buf + i + 1);
        path_buf[strlen(path_buf) - 1] = '\0';
        ls(path_buf);
        break;
    case 3:
        strcpy(path_buf, buf + i + 1);
        path_buf[strlen(path_buf) - 1] = '\0';
        while (path_buf[i] != ' ')
            i++;
        path_buf[i] = '\0';
        cp(path_buf, path_buf + i + 1);
        break;
    default:
        shutdown();
        return 0;
    }
    return 1;
}
