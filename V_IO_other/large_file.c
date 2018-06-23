/*
 * @filename:    large_file.c
 * @author:      Crow
 * @date:        06/23/2018 18:08:00
 * @description: 此题目是TLPI 5-1习题,是为了实现不适用LFS扩展API情况下,完成大文件操作
 *               就是定义_FILE_OFFSET_BITS 宏,在不修改源文件的情况下,便可以完成移植
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* 仅仅加上这一个宏,便可以完整的实现了从32位到64位的迁移
 * 如果不使用 _FILE_OFFSET_BITS 宏
 * 就得使用 LFS的特征API, 其最大的特征是所有原syscall加上64标识,如open64
 * 同时还要定义 _LARGEFILE64_SOURCE宏
 * 我们可以通过 文件顶部定义或者使用 gcc -D选项进行宏定义添加,使其可见
 */

#define _FILE_OFFSET_BITS 64     

int main(int argc, char *argv[])
{
    int fd;
    off_t off;

    if (argc != 3 && strcmp(argv[1], "--help") != 0) {
        printf("Error Usage\n");
        exit(EXIT_FAILURE);
    }
    
    fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        perror("open");

    off = atoll(argv[2]);
    if (lseek(fd, off, SEEK_SET) == -1)
        perror("lseek");

    if (write(fd, "abcd", 4) == -1)
        perror("write");
    
    if (close(fd) == -1)
        perror("close");

    exit(EXIT_SUCCESS);
}

// PS: 我本地机器,glibc封装open是__libc_open64(), 也即是说,默认使用open64()....
