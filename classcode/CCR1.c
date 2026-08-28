#include <stdio.h>
#include <string.h>

int main(void) {
  char firstName[50];
  int age;
  double monthlyFee;

  // --- Input Section ---
  printf("First name: ");
  scanf("%49s", firstName);

  printf("Age: ");
  scanf("%d", &age);

  printf("Monthly membership price: $");
  scanf("%lf", &monthlyFee);

  // --- Receipt Output Section ---
  printf("\n");
  printf("========================================\n");
  printf("            GYM MEMBERSHIP RECEIPT       \n");
  printf("========================================\n");
  printf("Customer Name:      %s\n", firstName);
  printf("Customer Age:       %d\n", age);
  printf("Monthly Fee:        $%.2f\n", monthlyFee);
  printf("----------------------------------------\n");

  if (age >= 65) {
    printf("Status:             SENIOR PROGRAM MEMBER\n");
    printf("This customer qualifies for the Senior\n");
    printf("Program benefits.\n");
  } else {
    printf("Status:             STANDARD MEMBER\n");
  }

  printf("========================================\n");
  printf("       Thank you for joining us!         \n");
  printf("========================================\n");

  return 0;
}