/*
 * @filename:    writev.c
 * @author:      Crow
 * @date:        06/25/2018 22:51:10
 * @description: 此例同readv.c一起作为分散输入和集中输出的例子
 *               readv(), 将文件内容输出到多个缓冲区中
 *               writev(), 将多个缓冲区的内容组合写入一个文件,
 *               其中注意, struct iovc *与, struct iovc (*)[3] 区别
 *               用法还是有点讲究的.
 *
 *               不过我最好奇的时,写好的文件,怎么就成了那个逼样子....
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

struct temp {
    int a;
    float b;
};

int main(int argc, char *argv[])
{
    int fd, count;
    struct iovec iov[3];
    ssize_t write_num, totRequired = 0;        // 很关键的初始化内容...
    int x;
    struct temp t = {
        .a = 4,
        .b = 4.5
    };
#define STR_LEN 20
    char str[STR_LEN] = "hello World";

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("ErrorUsage!\n");
        exit(EXIT_FAILURE);
    }


    x =4;
    count = 3;

    fd = open(argv[1], O_RDWR | O_CREAT);
    if (fd == -1)
        perror("open");

    iov[0].iov_base = &x;
    iov[0].iov_len = sizeof(x);
    totRequired += iov[0].iov_len;

    iov[1].iov_base = &t;
    iov[1].iov_len = sizeof(struct temp);
    totRequired += iov[1].iov_len;

    iov[2].iov_base = str;
    iov[2].iov_len = STR_LEN;
    totRequired += iov[2].iov_len;

    write_num = writev(fd, iov, count);
    if (write_num == -1)
        perror("writev");
    if (write_num != totRequired)
        printf("It's not equal\n");

    printf("real: %ld, required: %ld\n", (long)write_num, (long)totRequired);
    exit(EXIT_SUCCESS);
}
