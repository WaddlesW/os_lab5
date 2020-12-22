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
typedef struct super_block {
    int32_t magic_num;                  // 幻数
    int32_t free_block_count;           // 空闲数据块数
    int32_t free_inode_count;           // 空闲inode数
    int32_t dir_inode_count;            // 目录inode数
    uint32_t block_map[128];            // 数据块占用位图
    uint32_t inode_map[32];             // inode占用位图
} sp_block;

/*
结构体inode：
2字节无符号整型：文件类型、连接数；
4字节无符号整型：文件大小；
4字节无符号整型数组：数据块指针（6位）。
*/
typedef struct inode
{
    uint32_t size;              // 文件大小
    uint16_t file_type;         // 文件类型（文件/文件夹）
    uint16_t link;              // 连接数
    uint32_t block_point[6];    // 数据块指针
} inode;

/*
结构体dirent（目录项）：
1字节无符号整型：目录项类型
2字节无符号整型：目录项有效性
4字节无符号整型：目录项对应inode
字符串：文件/目录名
*/
typedef struct dir_item {               // 目录项一个更常见的叫法是 dirent(directory entry)
    uint32_t inode_id;          // 当前目录项表示的文件/目录的对应inode
    uint16_t valid;             // 当前目录项是否有效 
    uint8_t type;               // 当前目录项类型（文件/目录）
    char name[121];             // 目录项表示的文件/目录的文件名/目录名
} dir_item;

#endif