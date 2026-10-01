#include <stdio.h>
void main() {

#define N 10
  for (;;) {
    int a[N], *p;
    int sum = 0;

    for (p = &a[0]; p < &a[N]; p++)
      sum += *p;
    printf("Sum: %d\n", sum);
    printf("Address of a: %p\n", (void *)&a);
  }
}
