/*
 * @filename:    process_name.c
 * @author:      Crow
 * @date:        06/24/2018 21:09:47
 * @description: 可以在程序任意处获得程序名的方法,需要定义_GNU_SOURCE
 *               同时降低了程序的可移植性, 只有GNU平台可以使用
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    printf("complete name: %s\n", program_invocation_name);
    printf("short name: %s\n", program_invocation_short_name);
    
    return 0;
}
