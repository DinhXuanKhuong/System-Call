#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

char *testname = "";

void err(char *msg) {
  printf("FAILED %s: %s\n", testname, msg);
  exit(1);
}

void pgaccess_test() {
  char *buf;
  uint64 abits;
  printf("pgaccess_test starting\n");
  testname = "pgaccess_test";
  int NumberOfPage = 32; // how many pages to test in one time
  buf = malloc(NumberOfPage * PGSIZE);
  if (buf == 0)
    err("malloc failed");  
  // First call to clear any stale PTE_A bits
  if (pgaccess(buf, NumberOfPage, &abits) < 0)
    err("pgaccess failed");

  // Access pages 1, 2, and 25
  buf[PGSIZE * 1] += 1;
  buf[PGSIZE * 2] += 1;
  buf[PGSIZE * 25] += 1;

  // Second call to check accessed pages
  if (pgaccess(buf, NumberOfPage, &abits) < 0)
    err("pgaccess failed");

  // Print the list of accessed pages
  printf("Accessed pages:");
  for (int i = 0; i < NumberOfPage; i++) {
    if (abits & (1L << i)) {
      printf(" %d", i);
    }
  }
  printf("\n");

  // Check the bitmask
  if (abits != ((1L << 1) | (1L << 2) | (1L << 25)))
    err("incorrect access bits set");

  free(buf);
  printf("pgaccess_test: OK\n");
}


int main(int argc, char *argv[]) {
  printf("pgtbltest starting\n");
  pgaccess_test();
  printf("ALL TESTS PASSED\n");
  exit(0);
}