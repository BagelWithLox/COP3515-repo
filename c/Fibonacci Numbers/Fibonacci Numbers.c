/*
 * Fibonacci Numbers Exercise
 */

#include <stdio.h>

int fibbonacci(int n) {
   if(n == 0){
      return 0;
   } else if(n == 1) {
      return 1;
   } else {
      int value1 = fibbonacci(n-1);
      int value2 = fibbonacci(n-2);
      printf("%d %d \n",value2,value1);
      return (value1 + value2);
   }
}

int main() {
   int n;

   printf("Enter number to calculate Fibbonacci number for: \n");
  scanf("%d",&n);
   printf("\n\nFibbonacci of %d is %d\n" ,n,fibbonacci(n));
}
