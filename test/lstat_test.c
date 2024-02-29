#include <sys/stat.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    struct stat statbuf;

    if (lstat(argv[1], &statbuf))
    {
        perror("lstat");
        return 1;
    }
    printf("%ld", statbuf.st_mode);
    return 0;
}