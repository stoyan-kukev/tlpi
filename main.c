#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "lib/tlpi_common.h"
#include "lib/error_functions.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        usage_err("%s [-a/--append] file\n", argv[0]);
    }
    
    Bool using_append = (strcmp(argv[1], "-a") == 0) || (strcmp(argv[1], "--append") == 0);

    if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) {
            usage_err("%s [-a/--append] file\n", argv[0]);
        }
    } else if (argc == 3) {
        if (!using_append) {
            usage_err("%s [-a/--append] file\n", argv[0]);
        }
    }

    const char* output_file = using_append ? argv[2] : argv[1];

    int open_flags = O_CREAT | O_RDWR;
    if (using_append) {
        open_flags |= O_APPEND;
    }

    int perm_flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    
    int fd = open(output_file, open_flags, perm_flags);
    if (fd == -1) {
        err_exit("open");
    }

    char buffer[1];
    int bytes_read;
    while((bytes_read = read(0, buffer, sizeof(buffer)) > 0)) {
        printf("%s", buffer);

        if(write(fd, buffer, bytes_read) != bytes_read) {
            err_exit("couldn't write whole buffer");
        }
    }

    if(close(fd) == -1) {
        err_exit("close");
    }
    
    return 0;
}
