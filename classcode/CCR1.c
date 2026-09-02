#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main() {

  char firstName[50];
  int age;
  double monthlyFee;

  // --- First Name Input ---
  while (true) {
    printf("First name: ");

    fgets(firstName, sizeof(firstName), stdin);

    // Check if the input was too long
    if (strchr(firstName, '\n') == NULL) {
      printf("Error: First name cannot exceed 49 characters.\n");

      // Clear the rest of the input
      while (getchar() != '\n')
        ;

      continue;
    }

    // Remove the newline
    firstName[strcspn(firstName, "\n")] = '\0';

    // Check if the name is empty
    if (strlen(firstName) == 0) {
      printf("Error: First name cannot be empty.\n");
      continue;
    }

    break;
  }

  // --- Age Input ---
  while (true) {
    printf("Age: ");

    if (scanf("%d", &age) != 1) {
      printf("Error: Please enter a valid age.\n");

      // Clear invalid input
      while (getchar() != '\n')
        ;
      continue;
    }

    if (age < 18) {
      printf("Error: Age must be at least 18.\n");
      continue;
    }

    break;
  }

  // --- Monthly Fee Input ---
  while (true) {
    printf("Monthly membership price: $");

    if (scanf("%lf", &monthlyFee) != 1) {
      printf("Error: Please enter a valid price.\n");

      // Clear invalid input
      while (getchar() != '\n')
        ;
      continue;
    }

    if (monthlyFee <= 0) {
      printf("Error: Monthly membership price must be greater than 0.\n");
      continue;
    }

    break;
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
