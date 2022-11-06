#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void
print_id(int id, char *fname) 
{
    FILE *fptr = NULL;

    if (id == 1) {
        while ((fptr = fopen(fname, "r")) == NULL) {
            usleep(1000);
        }
        printf("%d\n", id);
        fclose(fptr);
        remove(fname);
    } else {
        printf("%d\n", id);
        fptr = fopen(fname, "w");
        fprintf(fptr, "%d", id);
        fclose(fptr);
    }

    exit(0);
}

int
main(void)
{
    char file_name[11] = "XXXXXX";
    mktemp(file_name);
    strcat(file_name, ".txt");

    pid_t pid_1 = fork();
    int id_1 = 1;
    if (pid_1 == 0) {
        print_id(id_1, file_name);
    }

    pid_t pid_2 = fork();
    int id_2 = 2;
    if (pid_2 == 0) {
        print_id(id_2, file_name);
    }

    waitpid(pid_2, NULL, 0);
    waitpid(pid_1, NULL, 0);

    return 0;
}