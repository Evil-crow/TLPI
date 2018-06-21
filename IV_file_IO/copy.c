/*
 * @filename:    copy.c
 * @author:      Crow
 * @date:        06/21/2018 22:12:39
 * @description: This file is a copy of TLPI about using file IO syscall to copy files
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    int input_fd, output_fd, flags;
    ssize_t read_num, write_num;
    char buffer[BUFSIZE];
    mode_t file_mode;

    if (argc != 3) {
        printf("Usage: ./copy src dest\n");
        exit(EXIT_FAILURE);
    }

    input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1)
        perror("open");

    flags = O_RDWR | O_CREAT | O_TRUNC ;
    file_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH ;
    
    output_fd = open(argv[2], flags, file_mode);
    if (output_fd == -1)
        perror("open");

    while ((read_num = read(input_fd, buffer, BUFSIZE)) > 0)
        if (write_num != write(output_fd, buffer, read_num))         // WARNING: write(int _fd, const void * buffer, int size), this size must equal to read_num ! ! !
            perror("write");

    if (read_num == -1)
        perror("read");

    if (close(input_fd) == -1)
        perror("close");
    
    if (close(output_fd) == -1)
        perror("close");

    exit(EXIT_SUCCESS);
}
