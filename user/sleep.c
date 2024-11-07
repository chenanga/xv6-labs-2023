#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int time_to_sleep;
    if (argc < 2 || argc > 2) {
        fprintf(2, "usage: sleep time...\n");
        exit(1);
    }
    time_to_sleep = atoi(argv[1]);
    sleep(time_to_sleep);
    exit(0);
}