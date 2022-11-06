#include <unistd.h>
#include <stdio.h>

int
main(void)
{
    printf("A\n");
    fork();
    printf("B\n");
    printf("C\n");
}