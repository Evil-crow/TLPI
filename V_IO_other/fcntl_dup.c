/*
 * @filename:    fcntl_dup.c
 * @author:      Crow
 * @date:        06/23/2018 23:34:34
 * @description: TLPI 5-4 使用fcntl()实现dup()以及dup2()
 *               其中需要注意的点是: 使用fcntl(fd, F_GETFL) 来判断oldFd的有效性
 *               同时应该显式关闭正在使用的newFd,判断方法同理,直接关闭会error(dup2()的实现)
 *               最后使用fcntl(fd, F_DUPFD, new_fd); 获取到复制的文件描述符
 *               不用担心局部返回,已经保存在三级描述表中,fd仅仅是一个文件描述符号码的标识
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int fcntl_dup(int old_fd);

int fcntl_dup2(int old_fd, int new_fd);

int main(int argc, char *argv[])
{
    int fd;

    if (argc == 2) {
        fd = fcntl_dup(atoi(argv[1]));
        if (fd == -1) {
            printf("error, old_fd is vilid\n");
            exit(EXIT_FAILURE);
        }
        printf("This is dup result: %d\n", fd);      // 查看fd
    } else {
        fd = fcntl_dup2(atoi(argv[1]), atoi(argv[2]));
        if (fd == -1) {
            printf("error\n");
            exit(EXIT_FAILURE);
        }
        printf("This is dup2 result: %d\n", fd);
    }

    exit(EXIT_SUCCESS);
}

int fcntl_dup(int old_fd)
{
    int temp_fd, fd;

    if (fcntl(old_fd, F_GETFL) == -1) {         // 判断oldFd的有效性
        perror("fcntl");
        errno = EBADF;
        return -1;
    }

    // 使用打开临时文件的方法获取当前最小的未使用的fd
    temp_fd = open("./temp", O_RDONLY | O_CREAT, 0644);    
    if (temp_fd == -1)
        perror("open");
    if (close(temp_fd) == -1)
        perror("close");

    fd = fcntl(old_fd, F_DUPFD, temp_fd);
    if (fd == -1)
        perror("fcntl");

    return fd;
}

int fcntl_dup2(int old_fd, int new_fd)
{
    int ret_fd;

    if (fcntl(old_fd, F_GETFL) == -1) {
        perror("fcntl");
        errno = EBADF;
        return -1;
    }

    if (old_fd == new_fd)
        return new_fd;

    if (fcntl(old_fd, F_GETFL)) {
        printf("new_fd: %d is really used\n", new_fd);
        close(new_fd);
    }
    
    ret_fd = fcntl(old_fd, F_DUPFD, new_fd);
    return ret_fd;
}
