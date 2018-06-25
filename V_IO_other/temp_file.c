/*
 * @filename:    temp_file.c
 * @author:      Crow
 * @date:        06/25/2018 23:46:57
 * @description: 本例子是mkstemp()产生临时文件的示例
 *               注意,templete要使用字符数组,因为修改后的是新的文件名
 *               字符串常量值不能修改的(在常量区)
 *               其中产生临时文件后,立马unlink(),使名字不可见,保证其他调用成功,且使用了O_EXEL标识,保证独占
 *               之后进行正常的IO操作
 *               文件在close(),后,真正的被删除
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    int fd;

    char templete[100] = "/tmp/stringXXXXXX";
    fd = mkstemp(templete);
    if (fd == -1)
        perror("mkstemp");
    printf("The temp file name is %s\n", templete);
    
    unlink(templete);       // 名字立马消失, 文件删除发生在close()之后

    //.....                 IO操作
    
    if (close(fd) == -1)
        perror("close");

    exit(EXIT_SUCCESS);
}
