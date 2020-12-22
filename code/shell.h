#ifndef SHELL_H
#define SHELL_H

/*
函数功能：判断输入命令
*/
int getcmd(char *buf, int nbuf);

/*
函数功能：执行命令
*/
int parse_cmd(char *buf);


#endif