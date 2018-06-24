/*
 * @filename:    dup_fp.c
 * @author:      Crow
 * @date:        06/24/2018 13:34:00
 * @description: TLPI 5-5 使用fcntl()的 F_GETFL命令获取到新的fd的标识
 *               通过 & 运算进行判断标识是否一致,结果是否
 *               同时,判定是否共享文件指针,通过移动新文件描述符指针,然后操作就文件描述符指针
 *               来说明他们是共享文件指针的
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int old_fd, new_fd;
    off_t old_off, new_off;
    int old_flags, new_flags;
    mode_t mode;

    old_flags = O_RDWR | O_CREAT;
    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    old_fd = open(argv[1], old_flags, mode);
    if (old_fd == -1)
        perror("open");

    new_fd = dup(old_fd);             // 使用dup2(). dup3()同理,duplicate产生的fd,默认是关闭O_CLOEXCL的
    if (new_fd == -1)
        perror("dup");

    new_flags = fcntl(new_fd, F_GETFL);
    if (new_flags & O_CLOEXEC)
        printf("flags is same\n");
    else
        printf("Can't duplicate O_CLOEXEC\n");

    old_off = lseek(old_fd, 0L, SEEK_SET);                 // 移动old文件指针
    printf("oldFd offset is %lld\n", (long long)old_off);

    new_off = lseek(new_fd, 0L, SEEK_END);                 // 移动新文件指针
    printf("newFd offset is %lld\n", (long long)new_off);

    write(old_fd, "test", 4);                              // 向old中写入文件, 若结果是依据上面的文件指针操作,说明共享文件指针
    
    if (close(old_fd) == -1)
        perror("close");
    if (close(new_fd) == -1)
        perror("close");

    exit(EXIT_SUCCESS);
}
