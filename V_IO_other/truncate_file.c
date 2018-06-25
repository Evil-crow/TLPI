/*
 * @filename:    truncate_file.c
 * @author:      Crow
 * @date:        06/25/2018 23:29:41
 * @description: 此例子是截断文件的API用法
 *               truncate(), 是可以不打开文件,直接进行文件截断的函数
 *               ftruncate(), 必须保证打开是按照可写的方式,否则还是没办法操作的
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd, ret_val;

    if (argc != 4 || strcmp(argv[1], "--help") == 0) {
        printf("ErrorUsage\n");
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[3],"t") == 0) {
        if ((ret_val = truncate(argv[1], atoll(argv[2]))) == -1)
            perror("truncate");

        printf("truncate file %s success\n",argv[1]);
    } else {
        if ((fd = open(argv[1], O_RDWR)) == -1)                     // 保证是可写的方式打开
            perror("open");

        if ((ret_val = ftruncate(fd, atoll(argv[2]))) == -1)
            perror("ftruncate");
        printf("ftruncate file %s success\n", argv[1]);
    }
    exit(EXIT_SUCCESS);
}
