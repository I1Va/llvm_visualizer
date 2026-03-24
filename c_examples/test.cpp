#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t func(uint64_t a) {
  return a * a;
}

int main(int argc, char **argv) {
   uint64_t arg = atoi(argv[1]);
   printf("func(%lu) = %lu\n", arg, func(arg));
  return 0;
}
