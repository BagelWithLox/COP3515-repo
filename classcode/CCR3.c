#include <stdio.h>

int main() {

  int age;
  char day;
  int validAge;
  int validDay;

  /* Get customer age */
  printf("Customer Age: ");
  validAge = scanf("%d", &age);

  if (validAge != 1) {
    printf("Error: Invalid age data type.\n");
    printf("Please enter a whole number: ");

    /* Remove the invalid input */
    scanf("%*s");

    validAge = scanf("%d", &age);

    if (validAge != 1) {
      printf("Error: Invalid age.\n");
      return 0;
    }
  }

  /* Check age range */
  if (age <= 0 || age > 100) {
    printf("Error: Age must be between 1 and 100.\n");
    printf("Please enter a valid age: ");

    validAge = scanf("%d", &age);

    if (validAge != 1 || age <= 0 || age > 100) {
      printf("Error: Invalid age.\n");
      return 0;
    }
  }

  /* Get day type */
  printf("Day Type (w = weekday, e = weekend): ");
  validDay = scanf(" %c", &day);

  if (validDay != 1) {
    printf("Error: Invalid day data type.\n");
    return 0;
  }

  /* Convert uppercase to lowercase */
  if (day == 'W') {
    day = 'w';
  } else if (day == 'E') {
    day = 'e';
  }

  /* Check day type */
  if (day != 'w' && day != 'e') {
    printf("Error: Invalid day type.\n");
    printf("Please enter w for weekday or e for weekend: ");

    validDay = scanf(" %c", &day);

    if (validDay != 1) {
      printf("Error: Invalid day data type.\n");
      return 0;
    }

    /* Convert uppercase to lowercase again */
    if (day == 'W') {
      day = 'w';
    } else if (day == 'E') {
      day = 'e';
    }

    if (day != 'w' && day != 'e') {
      printf("Error: Invalid day type.\n");
      return 0;
    }
  }

  /* Display results */
  printf("\n----------------------------------------\n");
  printf("Grand Cinema Theater\n");
  printf("Ticket Classification\n");
  printf("----------------------------------------\n");

  printf("Customer Age : %d\n", age);

  if (day == 'w') {
    printf("Day Type     : Weekday\n");
  } else {
    printf("Day Type     : Weekend\n");
  }

  /* Determine ticket category */
  if (age >= 1 && age <= 12) {
    printf("Ticket Type  : Child Ticket\n");
  } else if (age >= 13 && age <= 59) {
    printf("Ticket Type  : Adult Ticket\n");
  } else {
    printf("Ticket Type  : Senior Ticket\n");
  }

  return 0;
}
