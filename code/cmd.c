/*
完成五条shell命令：ls、mkdir、touch、cp、shutdown
*/

#include "disk.h"

// #include <stdlib.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <string.h>
// #include "mystruct.h"
// #include "ext2.h"


void ls(char *path)
{
    int path_len = strlen(path);
    int inode_id = 0;

    dir_item block_buf[8];
    inode inode_list[1024];
    read_inode(inode_list);
    
    if (path_len > 1)   inode_id = find_inode(path);

    for (int i = 0; i < inode_list[inode_id].size; i++)
    {
        int j = 0;
        memset(block_buf, 0, sizeof(block_buf));
        read_block(inode_list[inode_id].block_point[i], (char *)block_buf);
        
        while(j<8 && block_buf[j].valid != 0)   printf("%s  \n", block_buf[j++].name);

        if(block_buf[j].valid == 0) break;

    }

    return;
}

int mkdir(char *path)
{
    inode inode_list[1024];
    read_inode(inode_list);
    int inode_id = find_finode();
    inode_list[inode_id].file_type = 1;

    dir_item block_buf[8];

    char path_buf[20], name_buf[20];
    memset(name_buf, 0, sizeof(name_buf));
    memset(path_buf, 0, sizeof(path_buf));

    int father_inode_id = 1;
    int path_len = strlen(path);
    int flag = 0;
    
    for (int i = path_len - 1; i >= 0; i--)
    {
        if (i == 0 || path[i] == '/')
        {
            strncpy(name_buf, path + i + 1, path_len - i - 1);
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
    if (inode_list[father_inode_id].size > 0)
    {
        write_inode(inode_list);
        memset(block_buf, 0, sizeof(block_buf));
        read_block(inode_list[father_inode_id].block_point[inode_list[father_inode_id].size - 1], (char *)block_buf);
        for (int j = 0; j < 8; j++)
        {
            if (block_buf[j].valid == 0)
            {
                block_buf[j].valid = 1;
                block_buf[j].inode_id = inode_id;
                block_buf[j].type = 1;
                strcpy(block_buf[j].name, name_buf);
                write_block(inode_list[father_inode_id].block_point[inode_list[father_inode_id].size - 1], (char *)block_buf);
                flag = 1;
                break;
            }
        }
    }
    if (flag == 0)
    {
        inode_list[father_inode_id].size++;
        inode_list[father_inode_id].block_point[inode_list[father_inode_id].size - 1] = find_fblock();
        write_inode(inode_list);
        memset(block_buf, 0, sizeof(block_buf));
        block_buf[0].valid = 1;
        block_buf[0].inode_id = inode_id;
        block_buf[0].type = 1;
        strcpy(block_buf[0].name, name_buf);
        write_block(inode_list[father_inode_id].block_point[inode_list[father_inode_id].size - 1], (char *)block_buf);
    }
    return inode_id;
}

int touch(char *path)
{
    inode inode_list[1024];
    read_inode(inode_list);
    dir_item block_buf[8];
    char path_buf[20], name_buf[20];
    int father_inode_id = 1;
    int path_len = strlen(path);
    int inode_id = find_finode();
    int flag = 0;
    inode_list[inode_id].file_type = 1;
    memset(name_buf, 0, sizeof(name_buf));
    memset(path_buf, 0, sizeof(path_buf));
    for (int i = path_len - 1; i >= 0; i--)
    {
        if (i == 0 || path[i] == '/')
        {
            strncpy(name_buf, path + i + 1, path_len - i - 1);
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
    if (inode_list[father_inode_id].size > 0)
    {
        write_inode(inode_list);
        memset(block_buf, 0, sizeof(block_buf));
        read_block(inode_list[father_inode_id].block_point[inode_list[father_inode_id].size - 1], (char *)block_buf);
        for (int j = 0; j < 8; j++)
        {
            if (block_buf[j].valid == 0)
            {
                block_buf[j].valid = 1;
                block_buf[j].inode_id = inode_id;
                block_buf[j].type = 0;
                strcpy(block_buf[j].name, name_buf);
                write_block(inode_list[father_inode_id].block_point[inode_list[father_inode_id].size - 1], (char *)block_buf);
                flag = 1;
                break;
            }
        }
    }
    if (flag == 0)
    {
        inode_list[father_inode_id].size++;
        inode_list[father_inode_id].block_point[inode_list[father_inode_id].size - 1] = find_fblock();
        write_inode(inode_list);
        memset(block_buf, 0, sizeof(block_buf));
        block_buf[0].valid = 1;
        block_buf[0].inode_id = inode_id;
        block_buf[0].type = 0;
        strcpy(block_buf[0].name, name_buf);
        write_block(inode_list[father_inode_id].block_point[inode_list[father_inode_id].size - 1], (char *)block_buf);
    }
    return inode_id;
}

void cp(char *from, char *to)
{
    inode inode_list[1024];
    read_inode(inode_list);
    int inode_id = touch(to);
    int from_id = find_inode(from);
    char buf[1024];
    for (int i = 0; i < inode_list[from_id].size; i++)
    {
        read_block(inode_list[from_id].block_point[i], buf);
        inode_list[inode_id].size++;
        int block_id = inode_list[inode_id].block_point[i] = find_fblock();
        write_block(block_id, buf);
    }
}

void shutdown()
{
    close_disk();
}
