/*
 * @filename:    append_seek_set.c
 * @author:      Crow
 * @date:        06/23/2018 20:47:54
 * @description: TLPI 5-2 习题,为了验证O_APPEND的原子操作性
 *               设定O_APPEND标志位之后,每次write()之前,都会使用lseek()
 *               将文件指针指向文件尾,同时将这两步封装为原子操作
 *               Before each write(2), the file offset is positioned at the end of the file, as ifwith lseek(2).  
 *               The modification of the file offset and the write operation are performed as a single atomic step.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    ssize_t write_num;

    if (argc != 3 && strcmp(argv[1], "--help") != 0) {
        printf("Error Usage!\n");
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_WRONLY |O_APPEND);
    if (fd == -1)
        perror("open");

    if (lseek(fd, 0L, SEEK_SET) == -1)
        perror("lseek");
    
    #define BUFSIZE 1024
    if ((write_num = write(fd, argv[2], strlen(argv[2]))) != strlen(argv[2])){
        printf("%jd, %jd\n",write_num, strlen(argv[2]));
        perror("write");
    }

    lseek(fd, 0L, SEEK_SET);
    write(fd, "aga", 3);

    if (close(fd) == -1)
        perror("close");

    exit(EXIT_SUCCESS);
}
