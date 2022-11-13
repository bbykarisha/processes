#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
 
enum
{
    FNAME_SIZE = 7,
    BUF_SIZE = 1024
};

int
my_read(int fd, char *buf, size_t *line)
{
    int character = 0;
    int cnt = 0;

    while (1) {
        if (cnt == BUF_SIZE) {
            break;
        }
        if (read(fd, &character, 1) != 1) {
            break;
        }
        if (*line != 2) {
            buf[cnt] = character;
            ++cnt;
        }
        if ((character == '\n') && (*line <= 2)) {
            ++(*line);
        }
    }

    return cnt;
}

int
main(int argc, char *argv[])
{
    char *file_name = argv[1];
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        return 0;
    }

    char tmp_file[FNAME_SIZE] = "XXXXXX";
    int tmp_fd = mkstemp(tmp_file);
     
    unlink(tmp_file); 

    char buffer[BUF_SIZE];
    int num = 0;
    size_t line = 1;

    while ((num = my_read(fd, buffer, &line)) > 0) {   
        write(tmp_fd, buffer, num);
    }

    close(fd);

    fd = open(file_name, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        return 0;
    }

    lseek(tmp_fd, 0, SEEK_SET);

    while((num = read(tmp_fd, buffer, BUF_SIZE)) > 0) {
        write(fd, buffer, num);
    }

    close(fd);
    close(tmp_fd);

    return 0;
}