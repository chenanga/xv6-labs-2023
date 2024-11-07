#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(2, "xargs argc is too fewer\n");
    }
    /*
    atgv [1]
     */
    char *args[MAXARG] = {};
    char  buf[512], tmp_buf[1];
    int   i, idx, ret, stop = 0;

    for (i = 0; i < argc - 1; i++) {
        args[i] = argv[i + 1];
    }

    while (stop == 0) {
        idx = 0;
        while (idx < sizeof(buf) - 1) {
            ret = read(0, tmp_buf, 1);
            if (ret == 0) {
                exit(0);
            }
            else if (ret != 1) {
                printf("read error \n");
                exit(0);
            }

            if (tmp_buf[0] == '\n') {
                break;
            }
            buf[idx++] = tmp_buf[0];
        }
        buf[idx]    = '\0';
        args[i]     = buf;
        args[i + 1] = 0;

        if (fork() == 0) {
            exec(argv[1], args);
            exit(0);
        }
        else {
            wait(0);
        }
    }

    exit(0);
}