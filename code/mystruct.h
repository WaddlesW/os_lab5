/*
定义结构体：spblock、inode、dirent
*/

#ifndef MYSTRUCT_H
#define MYSTRUCT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/*
spblock（超级块）：
4字节整型：幻数、空闲数据块数、空闲inode数、目录inode数；
4字节无符号整型数组：数据块占用位图（128位）、inode占用位图（32位）。
*/
typedef struct sp_block
{
    int32_t magic_num;
    int32_t free_block_count;
    int32_t free_inode_count;
    int32_t dir_inode_count;
    uint32_t block_map[128];
    uint32_t inode_map[32];
} sp_block;

/*
结构体inode：
2字节无符号整型：文件类型、连接数；
4字节无符号整型：文件大小；
4字节无符号整型数组：数据块指针（6位）。
*/
typedef struct inode
{
    uint16_t file_type;
    uint16_t link;
    uint32_t size;
    uint32_t block_point[6];
} inode;

/*
结构体dirent（目录项）：
1字节无符号整型：目录项类型
2字节无符号整型：目录项有效性
4字节无符号整型：目录项对应inode
字符串：文件/目录名
*/
typedef struct dirent
{
    uint8_t type;
    uint16_t valid;
    uint32_t inode_id;
    char name[121];
} dirent;

#endif