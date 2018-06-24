/*
 * @filename:    test.c
 * @author:      Crow
 * @date:        06/06/2018 16:21:47
 * @description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    char *fstr;
    int afnd, opt;

    fstr = NULL;
    afnd = 0;

    while ((opt = getopt(argc, argv, ":f:a")) != -1) {
        switch (opt) {
            case 'a': afnd++; break;
            case 'f': fstr = optarg; break;
            default: printf("Nothing is needed!\n");
        }
    }

    printf("-a is %d times\n", afnd);
    printf("-f's arugments is %s\n",fstr);

    return 0;
}
