/*
 * @filename:    get_libc_version.c
 * @author:      Crow
 * @date:        06/06/2018 00:07:10
 * @description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <gnu/libc-version.h>
#include <unistd.h>

void func_get_libc_version(void)      // 使用<gnu/libc-version.h>中提供函数进行输出
{
    const char *version = gnu_get_libc_version();

    puts(version);
}

void func_confstr(void)      // 使用<unistd.h>中的confstr()进行指定宏参数的获取
{
    char *version = NULL;
    size_t len;

    len = confstr(_CS_GNU_LIBC_VERSION, NULL, (size_t)0);  // 先使用NULL,来获取宏字符长度
    version = (char *)malloc(len);           // 分配内存
    if (version == NULL)               // 断言
        abort();
    confstr(_CS_GNU_LIBC_VERSION, version, len);      // 实际进行宏字符串的获取
    puts(version);
}

int main(int argc, char *argv[])
{
    func_get_libc_version();
    func_confstr();
    system("/lib64/libc.so.6");       // 第三种方法,直接使用动态链接库进行输出

    return 0;
}
