#ifndef CMD_H
#define CMD_H

/*
函数mkdir功能: 新建文件夹
输入：新文件夹路径（/dir/newdir）
*/
int mkdir(char *path);

/*
函数touch功能: 新建文件
输入：新文件路径（/dir/newfile）
*/
int touch(char *path);

/*
函数cp功能:复制文件粘贴到另一目录
输入：源文件及目标文件
*/
void cp(char *from, char *to);

/*
函数ls功能: 展示文件夹内容/读取文件

输入格式：
ls /dir —— 展示文件夹内容;
ls /dir/file —— 读取文件;

输出格式（文件夹时）：目录下文件夹+空行+目录下文件(每个文件/文件夹占一行)
*/
void ls(char *path);

/*
函数功能：关闭系统
*/
void shutdown();

#endif