#include <stdbool.h>
#include <stdio.h>

int main(void) {
  /* ---- Constants describing this program ---- */
  const char COURSE_TITLE[] = "Student Information Management System";
  const char PROGRAMMER_NAME[] = "Samuel Shivarig";
  const char VERSION_NUMBER[] = "1.0";

  /* ---- Variables to hold the student's data ---- */
  long studentID;
  char studentName[50];
  double gpa;
  bool idIsValid;
  bool nameIsValid;
  bool gpaIsValid;
  char nextChar; /* holds the character right after the ID digits */

  /* ---- Program header / welcome message ---- */
  printf("----------------------------------------\n");
  printf("%s\n", COURSE_TITLE);
  printf("Version %s\n", VERSION_NUMBER);
  printf("Programmer: %s\n", PROGRAMMER_NAME);
  printf("Welcome to SIMS\n");
  printf("----------------------------------------\n");

  /* ---- Collect student ID (numbers only) ---- */
  printf("Student ID: ");
  idIsValid = (scanf("%ld", &studentID) == 1);

  /* Look at the very next character after the digits. If it is a
     decimal point, the ID is not a whole number and must be
     rejected (e.g. "123.45" is not a valid Student ID). */
  nextChar = 0;
  scanf("%c", &nextChar);

  if (nextChar == '.') {
    idIsValid = false;
  }

  /* Discard anything else left on the line (stray letters, extra
     digits after a rejected decimal, etc.) so it can't corrupt
     the next read. This uses scanf conversion specifiers, not a
     loop. If nextChar was already the newline, there is nothing
     left to discard. */
  if (nextChar != '\n') {
    scanf("%*[^\n]");
    scanf("%*c");
  }

  if (!idIsValid) {
    printf("\nError: Student ID must contain numbers only.\n");
    printf("Program terminated.\n");
    return 1;
  }

  printf("Student Name: ");
  nameIsValid = (scanf(" %49[^\n]", studentName) == 1);

  /* Check that the name starts with a letter. This only checks the
     first character (not every character in the name), since
     checking the whole string would require a loop, which is out
     of scope for this sprint. */
  if (nameIsValid) {
    nameIsValid = (studentName[0] >= 'A' && studentName[0] <= 'Z') ||
                  (studentName[0] >= 'a' && studentName[0] <= 'z');
  }

  if (!nameIsValid) {
    printf("\nError: Student Name must begin with a letter.\n");
    printf("Program terminated.\n");
    return 1;
  }

  printf("Current GPA: ");
  gpaIsValid = (scanf("%lf", &gpa) == 1);

  if (!gpaIsValid) {
    printf("\nError: GPA must be a number.\n");
    printf("Program terminated.\n");
    return 1;
  }

  /* GPA cannot be negative. This is a simple range check on the
     value already read, so it does not need a loop. */
  if (gpa < 0) {
    printf("\nError: GPA cannot be negative.\n");
    printf("Program terminated.\n");
    return 1;
  }

  /* ---- Display formatted summary ---- */
  printf("\n");
  printf("Student Summary\n");
  printf("%-14s: %ld\n", "Student ID", studentID);
  printf("%-14s: %s\n", "Student Name", studentName);
  printf("%-14s: %.2lf\n", "Current GPA", gpa);

  return 0;
}
