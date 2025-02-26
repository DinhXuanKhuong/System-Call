#include "kernel/sysinfo.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

void print_sysinfo()
{
    struct sysinfo in4;
    if (sysinfo(&in4) < 0) {
        printf("sysinfo failed\n");
        exit(1);
    }

    printf("Free memory: %ld bytes\n", in4.freemem);
    printf("Number of processes: %ld\n", in4.nprocess);
    printf("Number of open files: %ld\n", in4.nopenfiles);
}

int main()
{
    printf("=== Running sysinfo tests ===\n");

    // **Initial sysinfo check**
    printf("\n[TEST 1] Initial sysinfo:\n");
    print_sysinfo();

    // **Create a new process and check nproc**
    printf("\n[TEST 2] Fork a process and check sysinfo:\n");
    int pid = fork();
    if (pid == 0) {
        // Child process: Just exit
        exit(0);
    } else {
        wait(0); // Parent waits for child to finish
    }
    print_sysinfo();

    // **Allocate memory and check freemem**
    printf("\n[TEST 3] Allocate memory and check sysinfo:\n");
    char *mem = sbrk(4096 * 10);  // Allocate 10 pages (should reduce freemem)
    if (mem == (char *)-1) {
        printf("sbrk failed\n");
        exit(1);
    }
    print_sysinfo();

    // **Open some files and check nopenfiles**
    printf("\n[TEST 4] Open files and check sysinfo:\n");
    int fd1 = open("README", O_RDONLY);
    int fd2 = open("README", O_RDONLY);
    int fd3 = open("README", O_RDONLY);
    if (fd1 < 0 || fd2 < 0 || fd3 < 0) {
        printf("open failed\n");
        exit(1);
    }
    print_sysinfo();
    close(fd1);
    close(fd2);
    close(fd3);

    // **Free memory and check freemem again**
    printf("\n[TEST 5] Free memory and check sysinfo:\n");
    sbrk(-4096 * 10);  // Free the 10 pages allocated earlier
    print_sysinfo();

    printf("\n=== All sysinfo tests passed! ===\n");
    exit(0);
}
