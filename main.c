#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "lib/error_functions.h"
#include "lib/get_num.h"

int main(int argc, char* argv[]) {
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        usage_err("%s file {r<len>|R<len>|w<str>|s<offset>}...\n", argv[0]);
    }

    const char* file_name = argv[1];
    int fd = open(
        file_name,
        O_RDWR | O_CREAT,
        S_IRUSR | S_IWUSR |
        S_IRGRP | S_IWGRP |
        S_IROTH | S_IWOTH
    );

    if (fd == -1) {
        err_exit("opening file");
    }
    
    size_t len;
    char* buffer;
    int bytes_written;
    int offset;
    for (int arg_pair = 2; arg_pair < argc; arg_pair += 1) {
        switch(argv[arg_pair][0]) {
            case 'r':
            case 'R':
                len = get_long(&argv[arg_pair][1], GN_ANY_BASE, argv[arg_pair]); 
                buffer = malloc(len);
                if (buffer == NULL) {
                    err_exit("malloc");
                }

                int bytes_read = read(fd, buffer, len);
                if (bytes_read == -1) {
                    err_exit("read");
                }

                if (bytes_read == 0) {
                    printf("%s EOF\n", argv[arg_pair]);
                } else {
                    printf("%s: ", argv[arg_pair]);
                    for (int i = 0; i < bytes_read; i++) {
                        if (argv[arg_pair][0] == 'r') {
                            printf("%c", isprint((unsigned char) buffer[i]) ? buffer[i] : '?');
                        } else {
                            printf("%02x ", (unsigned int) buffer[i]);
                        }
                    }
                    printf("\n");
                }

                free(buffer);
                break;
            case 'w':
                bytes_written = write(fd, &argv[arg_pair][1], strlen(&argv[arg_pair][1]));
                if (bytes_written == -1) {
                    err_exit("write");
                }

                printf("%s: wrote %ld bytes\n", argv[arg_pair], (long) bytes_written);
                break;
            case 's':
                offset = get_long(&argv[arg_pair][1], GN_ANY_BASE, argv[arg_pair]);
                if (lseek(fd, offset, SEEK_SET) == -1) {
                    err_exit("lseek");
                }

                printf("%s: seek succeeded\n", argv[arg_pair]);
                break;

            default:
                cmd_line_err("Argument must start with [rRws]: %s\n", argv[arg_pair]);
        }
    }
    
    return 0;
}
