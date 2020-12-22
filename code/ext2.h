#ifndef EXT2_H
#define EXT2_H

/*
函数功能：读取数据块
*/
int read_block(unsigned int block_num, char *buf);

/*
函数功能：写入数据块
*/
int write_block(unsigned int block_num, char *buf);

/*
函数功能：读入inode_table
*/
void read_inode(inode *inode_table);

/*
函数功能：写入inode_table
*/
void write_inode(inode *inode_table);

/*
函数功能：读入spblock
*/
void read_sp(sp_block *spBlock);

/*
函数功能：写入spBlock
*/
void write_sp(sp_block *spBlock);

/*
函数功能：寻找空闲inode
*/
int find_finode();

/*
函数功能：寻找空闲块
*/
int find_fblock();

/*
函数功能：查找路径对应的inode
*/
int find_inode(char *path);

/*
函数功能：初始化文件系统
*/
void init_ext2();

#endif