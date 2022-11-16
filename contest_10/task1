//удаляет последнюю строчку в файле

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

enum { BUF_SIZE = 1024 };

int
main(int argc, char *argv[])
{
    char *file_path = argv[1];
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        return 0;
    }

    char tmp_file[7] = "XXXXXX"; 
    int tmp_fd = mkstemp(tmp_file);
     
    unlink(tmp_file);

    int character = 0;
    lseek(fd, -1, SEEK_END);
    read(fd, &character, 1);

    if (character == '\n') {
        lseek(fd, -1, SEEK_CUR);
        character = '\0';
        write(fd, &character, 1);
    }

    lseek(fd, 0, SEEK_SET);

    char buffer[BUF_SIZE];
    int num = 0;
    char *pos = NULL;
    char *string = buffer;

    int cnt_blocks = 0;
    int cnt_end = 0;

    int success = 0;

    while ((num = read(fd, buffer, BUF_SIZE - 1)) > 0) {
        buffer[num] = '\0';
        ++cnt_blocks;
        while ((pos = strchr(string, '\n')) != NULL) {
            success = 1;
            string = pos + 1;
            cnt_end = cnt_blocks; 
        }
        if (buffer[num - 1] != '\0') {
            write(tmp_fd, buffer, num);
        } else {
            write(tmp_fd, buffer, num - 1);
        }
    }
    
    if (success == 0) {
        close(tmp_fd);
        close(fd);
        fd = open(file_path, O_WRONLY | O_TRUNC);
        close(fd);
        return 0;
    }
    
    close(fd);

    fd = open(file_path, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        return 0;
    }

    int count = 0;
    lseek(tmp_fd, 0, SEEK_SET);

    while ((num = read(tmp_fd, buffer, BUF_SIZE - 1)) > 0) {
        ++count;
        if (count != cnt_end) {
            write(fd, buffer, num);
        } else {
            write(fd, buffer, string - buffer);
            break;
        }
    }

    close(fd);
    close(tmp_fd);

    return 0;
}
