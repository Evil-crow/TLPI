/*
 * @filename:    tee.c
 * @author:      Crow
 * @date:        06/22/2018 00:09:07
 * @description: TLPI IV练习题 tee实现, 其中主要使用getopt()获取命令行选项
 *               使用IO系统调用来实现,tee的功能
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>                     // 坑点: 对于getopt(),使用的时候,只有<unistd.h>,还不够

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    int fd, flags, opt;
    ssize_t read_num, write_num;
    _Bool a_opt = 0;                         // C11的_Bool还是比较砢碜的,实现了boolean,但是没有true,false的支持...
    char *file, buffer[BUFSIZE];
    mode_t mode;

    /* 使用getopt()来获取命令行选项
     * 其中注意的一点是,对于string,或者说optarg的获取
     * ":X:Y",X是可以获取optarg的, 坑点之一
     */
    while ((opt = getopt(argc, argv, ":f:a")) != -1) {
        switch (opt) {
            case 'a': a_opt = 1; break;
            case 'f': file = optarg; break;
            default: printf("Error option\n");
        }
    }

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;         // 预先设定标志位和权限
    flags = O_RDWR | O_CREAT;

    flags = a_opt ? (flags | O_APPEND) : (flags | O_TRUNC);     // 根据是否设置append选项来决定flags设置
    
    fd = open(file, flags, mode);
    if (fd == -1)
        perror("open");
    
    // 正常读写文件,并且在终端输出
    while ((read_num = read(STDIN_FILENO, buffer, BUFSIZE)) > 0) { 
        printf("%s",buffer);
        write_num = write(fd, buffer, read_num);
        if (write_num != read_num)
            perror("write");
    }
    
    // 关闭文件并退出
    if (close(fd) == -1)
        perror("close");

    exit(EXIT_SUCCESS);
}
