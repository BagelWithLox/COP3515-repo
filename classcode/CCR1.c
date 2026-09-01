#include <stdio.h>
#include <string.h>

int main() {
  char firstName[50];
  int age;
  double monthlyFee; // double for 2 points of precision

  // --- Input Section ---
  printf("First name: ");
  scanf("%s", firstName);

  printf("Age: ");
  scanf("%d", &age);

  printf("Monthly membership price: $");
  scanf("%lf", &monthlyFee);

  // input validation
  if (strlen(firstName) == 0) {
    printf("Error: First name cannot be empty.\n");
    return 1;
  }

  if (strlen(firstName) >= 50) {
    printf("Error: First name cannot exceed 50 characters.\n");
    return 1;
  }

  if (age < 0) {
    printf("Error: Age cannot be negative.\n");
    return 1;
  }

  if (monthlyFee <= 0) {
    printf("Error: Monthly membership price must be greater than 0.\n");
    return 1;
  }

  // --- Receipt Output Section ---
  printf("\n");
  printf("========================================\n");
  printf("           GYM MEMBERSHIP RECEIPT       \n");
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
