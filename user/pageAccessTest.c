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
  buf = malloc(32 * PGSIZE);
  if (buf == 0)
    err("malloc failed");
   //----------- 
  // printf("buf allocated at %p\n", buf);
  //------------
  
  // First call to clear any stale PTE_A bits
  if (pgaccess(buf, 32, &abits) < 0)
    err("pgaccess failed");

//-------------- debugging zone
  //   printf("After malloc, accessed pages:");
  //   for (int i = 0; i < 32; i++) {
  //     if (abits & (1 << i)) {
  //       printf(" %d", i);
  //     }
  //   }
  //   printf("\n");


  //   char *dummy = malloc(64 * PGSIZE);
  //   if (dummy == 0)
  //     err("dummy malloc failed");
  //   printf("dummy allocated at %p\n", dummy);
  //   for (int i = 0; i < 64; i++) {
  //     dummy[i * PGSIZE] = 1;
  //   }

  // // Check access bits after dummy access
  // if (pgaccess(buf, 32, &abits) < 0)
  // err("pgaccess failed");
  // printf("After dummy access, accessed pages:");
  // for (int i = 0; i < 32; i++) {
  // if (abits & (1 << i)) {
  //   printf(" %d", i);
  // }
  // }
  // printf("\n");

  // free(dummy);

  // // Check access bits after free(dummy)
  // if (pgaccess(buf, 32, &abits) < 0)
  // err("pgaccess failed");
  // printf("After free(dummy), accessed pages:");
  // for (int i = 0; i < 32; i++) {
  // if (abits & (1 << i)) {
  //   printf(" %d", i);
  // }
  // }
  // printf("\n");    


// -------------------
  // Access pages 1, 2, and 25
  buf[PGSIZE * 1] += 1;
  buf[PGSIZE * 2] += 1;
  buf[PGSIZE * 25] += 1;

  // Second call to check accessed pages
  if (pgaccess(buf, 32, &abits) < 0)
    err("pgaccess failed");

  // Print the list of accessed pages
  printf("Accessed pages:");
  for (int i = 0; i < 32; i++) {
    if (abits & (1 << i)) {
      printf(" %d", i);
    }
  }
  printf("\n");

  // Check the bitmask
  if (abits != ((1 << 1) | (1 << 2) | (1 << 25)))
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