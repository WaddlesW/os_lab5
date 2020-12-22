/*
完成五条shell命令：ls、mkdir、touch、cp、shutdown
*/

// #include <stdlib.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <string.h>
// #include "mystruct.h"
// #include "ext2.h"

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

int touch(char *path)
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
                block_buffer[j].type = 0;
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
        block_buffer[0].type = 0;
        strcpy(block_buffer[0].name, name_buf);
        write_block(inode_table[father_inode_id].block_point[inode_table[father_inode_id].size - 1], (char *)block_buffer);
    }
    return inode_id;
}

void cp(char *from, char *to)
{
    inode inode_table[1024];
    read_inode(inode_table);
    int inode_id = touch(to);
    int from_id = find_inode(from);
    char buf[1024];
    for (int i = 0; i < inode_table[from_id].size; i++)
    {
        read_block(inode_table[from_id].block_point[i], buf);
        inode_table[inode_id].size++;
        int block_id = inode_table[inode_id].block_point[i] = find_fblock();
        write_block(block_id, buf);
    }
}

void shutdown()
{
    close_disk();
}
