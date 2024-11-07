#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int  pid;
    int  p_to_c_pipe[2];
    int  c_to_p_pipe[2];
    char byte = 'a';
    if (argc < 1 || argc > 1) {
        fprintf(2, "usage: pingpong\n");
        exit(1);
    }

    if (pipe(p_to_c_pipe) < 0 || pipe(c_to_p_pipe) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    if (fork() == 0) {
        pid = getpid();
        read(p_to_c_pipe[0], &byte, 1);
        printf("%d: received ping\n", pid);
        write(c_to_p_pipe[1], &byte, 1);
    }
    else {
        pid = getpid();
        write(p_to_c_pipe[1], &byte, 1);
        read(c_to_p_pipe[0], &byte, 1);
        printf("%d: received pong\n", pid);
    }

    exit(0);
}