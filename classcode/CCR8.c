#include <stdio.h>
#include <stdlib.h>

/* Read a number from 0 to 100. which: 1 = original grade, 2 = extra credit */
double readValue(int which) {
  double value;
  int result;
  int c;

  for (;;) {
    if (which == 1)
      printf("Original Grade: ");
    else
      printf("Extra Credit: ");

    result = scanf("%lf", &value);

    /* Discard the rest of the line */
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    if (result == EOF || (result != 1 && c == EOF))
      exit(1); /* input ended */

    if (result == 1 && value >= 0 && value <= 100)
      return value;

    printf("Invalid input. Enter a number from 0 to 100.\n");
  }
}

int main(void) {
  double originalGrade = readValue(1);
  double extraCredit = readValue(2);
  double updatedGrade = originalGrade + extraCredit;
  int confirm = 0;

  if (updatedGrade > 120)
    updatedGrade = 120;

  printf("\n----------------------------------------\n");
  printf("Sunshine Elementary School\n");
  printf("Grade Adjustment Report\n");
  printf("----------------------------------------\n");
  printf("Original Grade : %.2f\n", originalGrade);
  printf("Extra Credit   : %.2f\n", extraCredit);
  printf("Updated Grade  : %.2f\n", updatedGrade);
  printf("----------------------------------------\n");

  while (confirm != 1 && confirm != 2) {
    printf("Confirm updated grade? (1 = Yes, 2 = No): ");
    if (scanf("%d", &confirm) != 1)
      confirm = 0;
    while (getchar() != '\n')
      ;
  }

  if (confirm == 1)
    printf("Updated grade confirmed.\n");
  else
    printf("Updated grade was not confirmed.\n");

  return 0;
}
