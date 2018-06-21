/*
 * @filename:    fd.c
 * @author:      Crow
 * @date:        06/22/2018 00:04:05
 * @description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    if(close(STDIN_FILENO) == -1)
        perror("close");

    int fd = open("copy.c", O_RDONLY);
    printf("%d\n", fd);                    // #=> fd == 0, 由此可知,内核优先分配尽可能小的文件描述符
    close(fd);

    exit(EXIT_SUCCESS);
}
