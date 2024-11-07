#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int pipefd[2])
{
    int p[2];
    int buf;
    int tmp_buf;
    // 从左侧邻居获取一个数字，作为取余
    if (read(pipefd[0], &buf, 1) == 0) {
        exit(0);
    }
    printf("prime %d\n", buf);
    pipe(p);
    if (fork() == 0) {
        close(pipefd[0]);
        close(p[1]);
        prime(p);  // 递归
        exit(0);
    }
    else {
        while (read(pipefd[0], &tmp_buf, 1) != 0) {
            if (tmp_buf % buf != 0) {
                write(p[1], &tmp_buf, 1);
            }
        }
        close(p[1]);
        close(pipefd[0]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    int pipefd[2];
    int i;
    pipe(pipefd);

    if (fork() == 0) {
        close(pipefd[1]);
        prime(pipefd);
    }
    else {
        for (i = 2; i <= 35; i++) {
            write(pipefd[1], &i, 1);
        }
        close(pipefd[1]);
        wait(0);
    }

    exit(0);
}