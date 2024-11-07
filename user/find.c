#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *dir, char *file)
{
    // 列出当前目录的所有文件和目录
    int           fd;
    struct dirent de;
    struct stat   st;
    char          buf[256], *p;

    if ((fd = open(dir, 0)) < 0) {
        fprintf(2, "ls: cannot open %s\n", dir);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
            fprintf(2, "argv[1] is not dir\n");
            break;

        case T_DIR:
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) continue;
                if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0) {
                    continue;
                }

                if (strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf) {
                    printf("ls: path too long\n");
                    break;
                }

                strcpy(buf, dir);
                p    = buf + strlen(buf);
                *p++ = '/';
                strcpy(p, de.name);

                if (stat(buf, &st) < 0) {
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }

                if (st.type == T_FILE) {
                    if (strcmp(file, de.name) == 0) {
                        printf("%s/%s\n", dir, de.name);
                    }
                }
                else if (st.type == T_DIR) {
                    find(buf, file);
                }
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
