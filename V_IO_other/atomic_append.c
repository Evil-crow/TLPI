/*
 * @filename:    atomic_append.c
 * @author:      Crow
 * @date:        06/23/2018 22:12:20
 * @description: TLPI 5-3 很明显,本题是为了验证O_APPEND的原子操作性
 *               -rw-r--r-- 1 Crow Crow 2000000 Jun 23 23:02 f1
 *               -rw-r--r-- 1 Crow Crow 1144088 Jun 23 23:02 f2
 *               结果如上, 不使用O_APPEND,不能将文件指针移动和write()作为原子操作
 *               最后,就会使得文件写入的内容大于预期,因为多个进程/线程同时写入,会产生竞争状态
 *               在内核进程调度时,A进程时间轮转完,交由B进程写入,但是A刚执行完lseek(),并未写入
 *               B写入后,改变了文件末尾的位置. 再回到A中,fp位置并未改变,A会覆盖B中的内容
 *               所以,总文件大小小于两个进程操作之和
 *               解决办法: 就是封装为原子操作, O_APPEND 或者文件锁...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    _Bool flag;
    int fd, flags;
    ssize_t num_bytes;
    mode_t mode;

    flag = 0;
    if ((argc < 3) || ((argc == 4) && strcmp(argv[3], "x") != 0)) {
        printf("Error Usage\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 4 && strcmp(argv[3], "x") == 0)
        flag = 1;
    
    flags = O_RDWR | O_CREAT;
    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    flags = flag ? flags : (flags | O_APPEND);

    fd = open(argv[1], flags, mode);
    if (fd == -1)
        perror("open");

    num_bytes = atoll(argv[2]);
    while (num_bytes--) {
        if (flag)
            lseek(fd, 0L, SEEK_END);
        write(fd, "s", 1);
    }

    if (close(fd) == -1)
        perror("close");

    exit(EXIT_SUCCESS);
}
