/*
 * @filename:    file_hole.c
 * @author:      Crow
 * @date:        06/21/2018 22:29:14
 * @description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE 1024
#define READ_BUFSIZE 2048

void write_hole(int argc, char *argv[]);

void read_hole(int argc, char *argv[]);

void print_char(char *buffer, ssize_t num);

int main(int argc, char *argv[])
{
    write_hole(argc, argv);
    read_hole(argc, argv);
    exit(EXIT_SUCCESS);
}

void write_hole(int argc, char *argv[])
{
    int fd;
    ssize_t write_num;
    char buffer[BUFSIZE];

    if (argc != 2) {
        printf("Usage: ./hole _fd\n");
        exit(EXIT_FAILURE);
    }

    printf("PLease input what you want to write: ");
    scanf("%s", buffer);
    if (strcmp(buffer, "NULL") == 0) {              // scanf()的实现问题,不会读入空,所以特殊方法输入空
        printf("The string is empty!\n");
        strcpy(buffer, "");
    }

    fd = open(argv[1], O_WRONLY);
    if (fd == -1)
        perror("open");

    lseek(fd, 10, SEEK_END);                        // 从文件末尾向后移动10个字节,调整文件指针
    
    write_num = write(fd, buffer, strlen(buffer));  // 写入内容,这里是不会写入 '\0'的,因为使用的是strlen(), 并非sizeof
    if (write_num == -1)
        perror("write");

    if (close(fd) == -1)
        perror("close");
}

void read_hole(int argc, char *argv[])
{
    int fd;
    char buffer[BUFSIZE];
    ssize_t read_num;

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        perror("Open");
    
    read_num = read(fd, buffer, READ_BUFSIZE);  // read的时候,hole也会读入,hole是使用 '\0'填充的,

    /* 这里切记,EOF于'\0'的区别
     * EOF标识文件结尾,是-1宏
     * '\0'是结束符,C中常用的标识, ASCII 为 0
     * read是读取到EOF停止,并非是'\0'停止 ! ! !
     */

    if (read_num == -1)
        perror("read");
    else {
        printf("%zd characters: ",read_num);
        print_char(buffer, read_num);
    }
    
    if (close(fd) == -1)
        perror("close");
}

void print_char(char *buffer, ssize_t num)
{
    /* 这里存在一个问题: 其中会有'\n'
     * 是的,没错这是,我的开发环境问题
     * Vim写入,会自动在行尾加上'\n'
     * echo,不加上 -n 选项,也是默认进行换行的
     */

    for (int i = 0; i < num; ++i){
        if (buffer[i] == '\0')
            printf("\\0 ");
        else if (buffer[i] == '\n')         
            printf("\\n ");
        else  
            printf("%c ", buffer[i]);

    }
    printf("EOF\n");
}


/* file hole的作用:
 * 上面我们说了这么多文件空洞的作用到底是什么?
 * 我们看到的,还是'\0',
 * 这是从编程角度看到的,实际上,在未真正使用之前,file hole的内容实际上是没有的
 * 也就是说实际磁盘大小,小于看到的大小,对于稀疏文件十分有用,比如core dump文件
 */
