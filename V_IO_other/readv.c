#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct test {
    int a;
    float b;
}Test;

int main(int argc, char *argv[])
{
    int fd, count;
    ssize_t read_num, totalrequired = 0;         // 血泪史,一定要记得初始化... 尤其是totalreauired,这种被默认以为是0的
    struct iovec iov[3];
    count = 3;

    int x;
    Test test_temp;
#define CHAR_NUM 100                             // 文件内部 #define有时也是必要的
    char temp[CHAR_NUM];

    iov[0].iov_base = &x;
    iov[0].iov_len = sizeof(int);
    totalrequired += iov[0].iov_len;

    iov[1].iov_base = &test_temp;
    iov[1].iov_len = sizeof(Test);
    totalrequired += iov[1].iov_len;

    iov[2].iov_base = temp;
    iov[2].iov_len = sizeof(temp);
    totalrequired += iov[2].iov_len;

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        perror("open");

    read_num = readv(fd, iov, count);        // struct iovc (*)[3] 与 struct iovc * 是不同的
    if (read_num != totalrequired)
        printf("It's not equal!\n");

    // 此处结果是不同的,因为写入大小是小于的,write会得到相同的结果

    printf("We need %ld, really, we get %ld\n", (long)totalrequired, (long)read_num);
    printf("x: %d\n test_temp.a: %d, test_temp.b: %f\n temp: %s\n ", x, test_temp.a, test_temp.b, temp);

    exit(EXIT_SUCCESS);
}
