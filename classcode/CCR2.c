/*int overflow when inputting -9999...check later*/

#include <stdio.h>
int main() {

  int sales[5];
  int total;
  int highest;
  int lowest;
  int average;
  int valid;
  char input[50];

  /* Enter sales for each hour */
  for (int i = 0; i < 5; i++) {

    valid = 0;

    while (valid == 0) {

      printf("Hour %d: ", i + 1);
      fgets(input, 50, stdin);

      /* Check that every character is a number */
      valid = 1;

      for (int j = 0; input[j] != '\0' && input[j] != '\n'; j++) {

        if (input[j] < '0' || input[j] > '9') {
          valid = 0;
        }
      }

      if (valid == 1) {

        sales[i] = 0;

        for (int j = 0; input[j] != '\0' && input[j] != '\n'; j++) {
          sales[i] = sales[i] * 10 + (input[j] - '0');
        }

      } else {
        printf("Invalid input. Please enter a whole number 0 or greater.\n");
      }
    }
  }

  /* Calculate total */
  total = 0;

  for (int i = 0; i < 5; i++) {
    total = total + sales[i];
  }

  /* Calculate average and round to nearest whole number */
  average = (total + 2) / 5;

  /* Find highest and lowest */
  highest = sales[0];
  lowest = sales[0];

  for (int i = 1; i < 5; i++) {

    if (sales[i] > highest) {
      highest = sales[i];
    }

    if (sales[i] < lowest) {
      lowest = sales[i];
    }
  }

  /* Display final report */
  printf("\n----------------------------------------\n");
  printf("Sunrise Coffee Company\n");
  printf("Daily Sales Summary\n");
  printf("----------------------------------------\n");

  printf("Sales by Hour\n");

  for (int i = 0; i < 5; i++) {
    printf("Hour %d : %d\n", i + 1, sales[i]);
  }

  printf("Total Cups Sold : %d\n", total);
  printf("Average per Hour: %d cups\n", average);

  /* Display all busiest hours */
  printf("Busiest Hour(s) : ");

  for (int i = 0; i < 5; i++) {

    if (sales[i] == highest) {
      printf("Hour %d (%d cups) ", i + 1, sales[i]);
    }
  }

  printf("\n");

  /* Display all slowest hours */
  printf("Slowest Hour(s) : ");

  for (int i = 0; i < 5; i++) {

    if (sales[i] == lowest) {
      printf("Hour %d (%d cups) ", i + 1, sales[i]);
    }
  }

  printf("\n");

  return 0;
}
