/*
函数mkdir功能: 新建文件夹
输入：新文件夹路径（/dir/newdir）
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// #include "mystruct.h"
// #include "./ext2.c"

int mkdir(char *path)
{
    inode inode_table[1024];
    read_inode(inode_table);
    dirent block_buffer[8];
    char path_buf[20], name_buf[20];
    int father_inode_id = 1;
    int len = strlen(path);
    int inode_id = find_finode();
    int flag = 0;
    inode_table[inode_id].file_type = 1;
    memset(name_buf, 0, sizeof(name_buf));
    memset(path_buf, 0, sizeof(path_buf));
    for (int i = len - 1; i >= 0; i--)
    {
        if (i == 0 || path[i] == '/')
        {
            strncpy(name_buf, path + i + 1, len - i - 1);
            strncpy(path_buf, path, i);
            break;
        }
    }
    if (strlen(path_buf) > 1)
    {
        father_inode_id = find_inode(path_buf);
    }
    else
    {
        father_inode_id = 0;
    }
    if (inode_table[father_inode_id].size > 0)
    {
        write_inode(inode_table);
        memset(block_buffer, 0, sizeof(block_buffer));
        read_block(inode_table[father_inode_id].block_point[inode_table[father_inode_id].size - 1], (char *)block_buffer);
        for (int j = 0; j < 8; j++)
        {
            if (block_buffer[j].valid == 0)
            {
                block_buffer[j].valid = 1;
                block_buffer[j].inode_id = inode_id;
                block_buffer[j].type = 1;
                strcpy(block_buffer[j].name, name_buf);
                write_block(inode_table[father_inode_id].block_point[inode_table[father_inode_id].size - 1], (char *)block_buffer);
                flag = 1;
                break;
            }
        }
    }
    if (flag == 0)
    {
        inode_table[father_inode_id].size++;
        inode_table[father_inode_id].block_point[inode_table[father_inode_id].size - 1] = find_fblock();
        write_inode(inode_table);
        memset(block_buffer, 0, sizeof(block_buffer));
        block_buffer[0].valid = 1;
        block_buffer[0].inode_id = inode_id;
        block_buffer[0].type = 1;
        strcpy(block_buffer[0].name, name_buf);
        write_block(inode_table[father_inode_id].block_point[inode_table[father_inode_id].size - 1], (char *)block_buffer);
    }
    return inode_id;
}