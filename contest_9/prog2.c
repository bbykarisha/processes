#include <stdio.h>
#include <unistd.h>

int
main(void)
{
    int i = 1;
    if (fork() == 0) {
        i = 2;
        printf("%d\n", i);
    }
    printf("%d\n", i);
}
