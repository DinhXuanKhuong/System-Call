#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: vmprint <command>\n");
        exit(1);
    }
    exec_vmprint(argv[1], &argv[1]);
    printf("exec_vmprint failed\n");
    exit(1);
}
