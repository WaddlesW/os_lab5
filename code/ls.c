#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// #include "mystruct.h"
// #include "./ext2.c"

/*
函数ls功能: 展示文件夹内容/读取文件

输入格式：
ls /dir —— 展示文件夹内容;
ls /dir/file —— 读取文件;

输出格式（文件夹时）：目录下文件夹+空行+目录下文件(每个文件/文件夹占一行)
*/

void ls(char *path)
{
    inode inode_table[1024];
    read_inode(inode_table);
    int len = strlen(path);
    int inode_id = 0;
    dirent block_buffer[8];
    if (len > 1)
    {
        inode_id = find_inode(path);
    }

    for (int i = 0; i < inode_table[inode_id].size; i++)
    {
        int flag = 0;
        memset(block_buffer, 0, sizeof(block_buffer));
        read_block(inode_table[inode_id].block_point[i], (char *)block_buffer);
        for (int j = 0; j < 8; j++)
        {
            if (block_buffer[j].valid == 0)
            {
                flag = 1;
                break;
            }
            printf("%s  \n", block_buffer[j].name);
        }
        if (flag == 1)
            break;
    }
    return;
}