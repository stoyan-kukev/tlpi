#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "lib/error_functions.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char* argv[]) {
    char* input_name = argv[1];
    char* output_name = argv[2];

    if (argc != 3 || strcmp(input_name, "--help") == 0)
        usage_err("%s old-file new-file\n", argv[0]);

    int input_fd = open(input_name, O_RDONLY);
    if (input_fd == -1)
        err_exit("opening file %s", input_name);

    int open_flags = O_CREAT | O_WRONLY | O_APPEND;
    int file_perms = S_IRUSR | S_IWUSR |
            S_IRGRP | S_IWGRP |
            S_IROTH | S_IWOTH;

    int output_fd = open(output_name, open_flags, file_perms);
    if (output_fd == -1)
        err_exit("opening file %s", output_name);

    char buffer[BUF_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(input_fd, buffer, BUF_SIZE)) > 0) {
        if (write(output_fd, buffer, bytes_read) != bytes_read) {
            fatal("couldn't write whole buffer");
        }
    }

    if (bytes_read == -1)
        err_exit("read");

    if (close(input_fd) == -1)
        err_exit("close input");
    
    if (close(output_fd) == -1)
        err_exit("close output");
  
    return 0;
}
