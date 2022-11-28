// why do signals complicate the program?
// signals can come at any moment of time, i.e. in the
// middle of the execution of any function. if the signal  
// comes during function decrease_number(), then the program 
// may output an incorrect number, for example, if we process 
// the number 100 and the signal arrives during processing,  
// there is a chance of getting the number 109, which is incorrect.  
// to solve this problem, I create a global variable output_str,
// which stores the correct number after each subtraction. but so 
// that she keeps exactly a copy of the last correct number I had
// to use dynamic memory. also, in signal handlers, you better not 
// use not asynchronously-safe functions, so instead of using printf()
// I use the write() system call, which outputs by bytes, so to get 
// the number of seconds until the next alarm I had to translate int  
// into char.

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

int seconds = 0;
char *output_str = NULL;

sigset_t blockset;
 
int
count_zero(void)
{
    int count = 0;
    int length = strlen(output_str);

    for (int i = 0; i < length; ++i) {
        if (output_str[i] == '0') {
            ++count;
        } else {
            return 0;
        }
    }

    return count;
}

void
alarm_handler(int sig_num)
{
    ++seconds;
    char new_line = '\n';
    
    if (seconds == 5) {
        int count = count_zero();
        int length = strlen(output_str);
        
        write(STDOUT_FILENO, output_str + count, length - count);
        write(STDOUT_FILENO, &new_line, sizeof new_line);
    
        seconds = 0;
    }

    alarm(1);
}

void
sigint_handler(int sig_num)
{
    char rest = 5 - seconds + '0';
    char new_line = '\n';

    write(STDOUT_FILENO, &rest, sizeof rest);
    write(STDOUT_FILENO, &new_line, sizeof new_line);
}

int
decrease_number(char *str)
{
    int length = strlen(str);
    int is_null = 1;

    for (int i = length - 1; i >= 0; --i) {
        if (str[i] == '0') {
            str[i] = '9';
        } else {
            str[i] = str[i] - 1;
            is_null = 0;
            break;
        }
    }

    sigprocmask(SIG_BLOCK, &blockset, NULL);
    strcpy(output_str, str);
    sigprocmask(SIG_UNBLOCK, &blockset, NULL);

    return is_null;
}

int
main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, alarm_handler);

    sigemptyset(&blockset);
    sigaddset(&blockset, SIGINT);
    sigaddset(&blockset, SIGALRM);

    char *string = argv[1];

    int length = strlen(string);
    output_str = calloc(length + 1, sizeof(char));
    strcpy(output_str, string);

    alarm(1);

    int res = 0; 
    while (res == 0) {
        res = decrease_number(string);
    }

    free(output_str);

    return 0;
}
