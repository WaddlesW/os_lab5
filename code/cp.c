/*
函数cp功能:复制文件粘贴到另一目录
输入：源文件及目标文件
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// #include "mystruct.h"
// #include "./ext2.c"
#include "./touch.c"

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