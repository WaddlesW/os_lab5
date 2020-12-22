/*
实现ext2系统
*/

// #include <stdlib.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <string.h>
// #include "mystruct.h"
#include "./disk.c"

int read_block(unsigned int block_num, char *buf)
{
    block_num = 2 * block_num;
    disk_read_block(block_num, buf);
    buf += DEVICE_BLOCK_SIZE;
    disk_read_block(block_num + 1, buf);
}

int write_block(unsigned int block_num, char *buf)
{
    block_num = 2 * block_num;
    disk_write_block(block_num, buf);
    buf += DEVICE_BLOCK_SIZE;
    disk_write_block(block_num + 1, buf);
}

void read_inode(inode *inode_table)
{
    for (int i = 1; i < 32; i++)
    {
        read_block(i, (char *)&(inode_table[32 * (i - 1)]));
    }
}

void write_inode(inode *inode_table)
{
    for (int i = 1; i < 32; i++)
    {
        write_block(i, (char *)&(inode_table[32 * (i - 1)]));
    }
}

void read_sp(sp_block *spBlock)
{
    char buf[1024];
    char *p;
    read_block(0, buf);
    p = (char *)spBlock;
    for (int i = 0; i < sizeof(sp_block); i++)
    {
        *p = buf[i];
        p++;
    }
}

void write_sp(sp_block *spBlock)
{
    char buf[1024];
    char *p;
    p = (char *)spBlock;
    for (int i = 0; i < sizeof(sp_block); i++)
    {
        buf[i] = *p;
        p++;
    }
    write_block(0, buf);
}

int find_finode()
{
    int num = -1;
    uint32_t buf;
    sp_block *spBlock;
    spBlock = (sp_block *)malloc(sizeof(sp_block));
    read_sp(spBlock);
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            buf = (spBlock->inode_map[i] >> (31 - j)) & (uint16_t)1;
            if (buf == 0)
            {
                num = i * 32 + j;
                spBlock->inode_map[i] = spBlock->inode_map[i] | (1 << (31 - j));
                break;
            }
        }
        if (buf == 0)
            break;
    }
    spBlock->free_inode_count--;
    write_sp(spBlock);
    return num;
}

int find_fblock()
{
    int num = -1;
    uint32_t buf;
    sp_block *spBlock;
    spBlock = (sp_block *)malloc(sizeof(sp_block));
    read_sp(spBlock);
    for (int i = 0; i < 128; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            buf = (spBlock->block_map[i] >> (31 - j)) & (uint16_t)1;
            if (buf == 0)
            {
                num = 32 * i + j;
                spBlock->block_map[i] = spBlock->block_map[i] | (1 << (31 - j));
                break;
            }
        }
        if (buf == 0)
            break;
    }
    spBlock->free_block_count--;
    write_sp(spBlock);
    return num;
}

int find_inode(char *path)
{
    if (strlen(path) <= 1)
        return 0;
    inode inode_table[1024];
    read_inode(inode_table);
    dirent block_buffer[8];
    int a = 0;
    int last = 0;
    char name_buf[20];
    int len = strlen(path);
    for (int o = 0; o <= len; o++)
    {
        if (o > 0 && path[o] == '/' || o == len)
        {
            memset(name_buf, 0, sizeof(name_buf));
            strncpy(name_buf, path + last + 1, o - last - 1);
            last = o;
            for (int i = 0; i < 6; i++)
            {
                int flag = 0;
                memset(block_buffer, 0, sizeof(block_buffer));
                read_block(inode_table[a].block_point[i], (char *)block_buffer);
                for (int j = 0; j < 8; j++)
                {
                    if (block_buffer[j].valid == 0)
                        break;
                    if (strcmp(name_buf, block_buffer[j].name) == 0)
                    {
                        a = block_buffer[j].inode_id;
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1)
                    break;
            }
        }
    }
    return a;
}

void init_file_system()
{
    char *p;
    char buf[1024];
    sp_block *spBlock;
    inode inode_table[1024];
    dirent block_buffer[8];
    spBlock = (sp_block *)malloc(sizeof(sp_block));
    open_disk();
    read_sp(spBlock);
    // 发现不是本文件系统，则进行初始化
    if (spBlock->magic_num != 0xdec0de)
    {
        puts("Create new file system");
        // 初始化超级块
        spBlock->magic_num = 0xdec0de;
        spBlock->free_block_count = 4096 - 1 - 32 - 1;
        spBlock->free_inode_count = 1024 - 1;
        spBlock->dir_inode_count = 1;

        memset(spBlock->block_map, 0, sizeof(spBlock->block_map));
        memset(spBlock->inode_map, 0, sizeof(spBlock->inode_map));

        spBlock->inode_map[0] = (1 << 31);
        spBlock->block_map[0] = ~(spBlock->block_map[0]);
        spBlock->block_map[1] = (1 << 31) | (1 << 30);

        // 初始化inode
        memset(inode_table, 0, sizeof(inode_table));
        inode_table[0].block_point[0] = 33;
        inode_table[0].file_type = 1;
        inode_table[0].size = 0;
        memset(block_buffer, 0, sizeof(block_buffer));

        write_block(33, (char *)block_buffer);
        write_inode(inode_table);
        write_sp(spBlock);
    }
    return;
}