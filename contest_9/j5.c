#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void
check(int character, int fst_char) 
{
    if (character == fst_char) {
        printf("%c%c", character, character);
    }
}

int
main(void)
{
    setbuf(stdin, 0);
    int first_char = getchar();
    int character = 0;

    while ((character = getchar()) != EOF) {
        pid_t pid = fork();
        if (pid == 0) {
            check(character, first_char);
            exit(0);
        } else if (pid == -1) {
            wait(NULL);
            if (fork() == 0) {
                check(character, first_char);
                exit(0);
            }
        }
        waitpid(-1, NULL, WNOHANG);
    }

    while (wait(NULL) > 0);
    printf("\n");

    return 0;
}