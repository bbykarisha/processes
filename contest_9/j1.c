#include <stdio.h>

enum
{
    BUF_SIZE_FST = 2,
    BUF_SIZE_SND = 3
};

int
main(void)
{
    char *fst_output[] = {"A\nB\nB\nC\nC\n", "A\nB\nC\nB\nC\n"};
    char *snd_output[] = {"1\n2\n2\n", "2\n1\n2\n", "2\n2\n1\n"};

    int n = 0;
    scanf("%d", &n);

    if ((n >= 0) && (n < BUF_SIZE_FST)) {
        printf("%s", fst_output[n]);
    } else {
        printf("UNKNOWN\n");
    }

    if ((n >= 0) && (n < BUF_SIZE_SND)) {
        printf("%s", snd_output[n]);
    } else {
        printf("UNKNOWN\n");
    }

    return 0;
}
