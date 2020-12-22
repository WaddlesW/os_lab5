/*
主函数
*/

#include <stdio.h>
#include <string.h>
#include "mystruct.h"
#include "shell.h"
#include "./shell.c"


int main(void)
{
    static char buf[100];

    init_ext2();

    while (getcmd(buf, sizeof(buf)) >= 0)
    {
        if (!parse_cmd(buf))
            return 0;
    }
    return -1;
}