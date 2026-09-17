/* ============================================================
   Sweet Delights Bakery - Order Total Calculator
   CCR-006

   Calculates sales tax and an optional employee-entered
   percentage discount for a single customer order, then
   prints a formatted receipt.

   Order of operations:
     1. Sales tax (6%) is applied to the original order amount.
     2. Discount (if any) is applied to the subtotal (order + tax).

   Uses only C features through Lesson 5:
   variables/constants, I/O, if/switch, loops, functions,
   basic arithmetic, and formatted output.
   ============================================================ */

#include <stdio.h>

#define TAX_RATE 0.06
#define MIN_ORDER 0.00
#define MAX_ORDER 5000.00

/* ---------- Function Prototypes ---------- */
void clearInputBuffer(void);
double getOrderAmount(void);
char getDiscountChoice(void);
double getDiscountPercent(void);
double calculateTax(double orderAmount);
double calculateDiscount(double subtotal, double discountPercent);
double roundToNearestNickel(double amount);
void printReceipt(double orderAmount, double tax, char discountChoice,
                  double discountPercent, double discountAmount,
                  double finalTotal);

/* ---------- Main ---------- */
int main(void) {
  double orderAmount, tax, subtotal;
  double discountPercent = 0.0, discountAmount = 0.0;
  double finalTotal;
  char discountChoice;

  orderAmount = getOrderAmount();

  discountChoice = getDiscountChoice();
  if (discountChoice == 'Y') {
    discountPercent = getDiscountPercent();
  }

  tax = calculateTax(orderAmount);
  subtotal = orderAmount + tax;

  if (discountChoice == 'Y') {
    discountAmount = calculateDiscount(subtotal, discountPercent);
  } else {
    discountAmount = 0.0;
  }

  finalTotal = subtotal - discountAmount;
  finalTotal = roundToNearestNickel(finalTotal);

  printReceipt(orderAmount, tax, discountChoice, discountPercent,
               discountAmount, finalTotal);

  return 0;
}

/* ---------- Input Functions ---------- */

/* Clears any leftover characters from a failed numeric read
   so they do not corrupt the next input prompt. */
void clearInputBuffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    /* discard */
  }
}

/* Prompts for and validates the order amount. */
double getOrderAmount(void) {
  double amount;
  int valid = 0;
  int result;

  while (!valid) {
    printf("Enter order amount: $");
    result = scanf("%lf", &amount);

    if (result != 1) {
      printf("Error: Please enter a valid number.\n");
      clearInputBuffer();
    } else if (amount < MIN_ORDER) {
      printf("Error: Order amount cannot be negative. Please try again.\n");
    } else if (amount > MAX_ORDER) {
      printf("Error: Order amount cannot exceed $%.2f. Please try again.\n",
             MAX_ORDER);
    } else {
      valid = 1;
    }
  }

  return amount;
}

/* Prompts for and validates the Y/N discount choice. */
char getDiscountChoice(void) {
  char choice;
  int valid = 0;

  while (!valid) {
    printf("Does a discount apply? (Y/N): ");
    scanf(" %c", &choice);

    if (choice == 'y') {
      choice = 'Y';
    } else if (choice == 'n') {
      choice = 'N';
    }

    if (choice == 'Y' || choice == 'N') {
      valid = 1;
    } else {
      printf("Error: Please enter Y or N.\n");
    }
  }

  return choice;
}

/* Prompts for and validates the discount percentage. */
double getDiscountPercent(void) {
  double percent;
  int valid = 0;
  int result;

  while (!valid) {
    printf("Enter discount percentage (e.g., 10 for 10%%): ");
    result = scanf("%lf", &percent);

    if (result != 1) {
      printf("Error: Please enter a valid number.\n");
      clearInputBuffer();
    } else if (percent < 0) {
      printf(
          "Error: Discount percentage cannot be negative. Please try again.\n");
    } else if (percent > 100) {
      printf("Error: Discount percentage cannot exceed 100%%. Please try "
             "again.\n");
    } else {
      valid = 1;
    }
  }

  return percent;
}

/* ---------- Calculation Functions ---------- */

/* Calculates sales tax on the original order amount. */
double calculateTax(double orderAmount) { return orderAmount * TAX_RATE; }

/* Calculates the discount amount based on the subtotal (order + tax). */
double calculateDiscount(double subtotal, double discountPercent) {
  return subtotal * (discountPercent / 100.0);
}

/* Rounds a dollar amount to the nearest $0.05. */
double roundToNearestNickel(double amount) {
  double rounded;
  int nickels;

  nickels = (int)((amount / 0.05) + 0.5);
  rounded = nickels * 0.05;

  return rounded;
}

/* ---------- Output Function ---------- */

/* Prints the formatted customer receipt. */
void printReceipt(double orderAmount, double tax, char discountChoice,
                  double discountPercent, double discountAmount,
                  double finalTotal) {
  printf("\n----------------------------------------\n");
  printf("Sweet Delights Bakery\n");
  printf("Customer Receipt\n");
  printf("----------------------------------------\n");
  printf("Original Order    : $%.2f\n", orderAmount);
  printf("Sales Tax          : $%.2f\n", tax);

  if (discountChoice == 'Y') {
    printf("Discount (%.0f%%)     : $%.2f\n", discountPercent, discountAmount);
  } else {
    printf("Discount           : N/A\n");
  }

  printf("-------------------------------\n");
  printf("Final Total        : $%.2f\n", finalTotal);
}
