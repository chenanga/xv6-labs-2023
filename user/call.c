#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

__attribute__((noinline)) int g(int x) {
  return x+3;
}

__attribute__((noinline)) int f(int x) {
  return g(x);
}

void main(void) {
  printf("%d %d\n", f(8)+1, 13);
  unsigned int i = 0x00646c72;
  // 高地址  <--> 低地址
  // 64:d 6c:l 72:r

  // 打印时候从i的低地址开始
	printf("H%x Wo%s", 57616, &i);
  // 57616: 0xE110

  printf("x=%d y=%d", 3);
  exit(0);
}
