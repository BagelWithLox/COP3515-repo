/*Student Information Management System (SIMS)
COP 3515 - Advanced Program Design
Client Change Request: project 2 (builds on project 1)*/

#include <stdbool.h>
#include <stdio.h>

int main(void) {
  /* ---- Constants describing this program ---- */
  const char COURSE_TITLE[] = "Student Information Management System";
  const char PROGRAMMER_NAME[] = "Samuel Shivarig";
  const char VERSION_NUMBER[] = "2.0";

  /* ---- Variables to hold the student's data---- */
  long studentID;
  char studentName[50];
  double gpa;
  bool idIsValid;
  bool nameIsValid;
  bool gpaIsValid;
  char nextChar; /* holds the character right after the ID digits */

  /* ---- Variables to hold the course grades ---- */
  double grades[5];
  bool gradesAreValid;
  bool gradeOk;
  double averageGrade;
  double highestGrade;
  double lowestGrade;

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

  /* Student ID cannot be negative. This is a simple range check on
     the value already read, so it does not need a loop. */
  if (idIsValid && studentID < 0) {
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
    printf("\nError: Student ID must be a positive whole number.\n");
    printf("Program terminated.\n");
    return 1;
  }

  /* ---- Collect student name (must begin with a letter) ---- */
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

  /* ---- Collect GPA (must be numeric and not negative) ---- */
  printf("Current GPA: ");
  gpaIsValid = (scanf("%lf", &gpa) == 1);

  if (!gpaIsValid) {
    printf("\nError: GPA must be a number.\n");
    printf("Program terminated.\n");
    return 1;
  }

  if (gpa < 0) {
    printf("\nError: GPA cannot be negative.\n");
    printf("Program terminated.\n");
    return 1;
  }

  /* ---- Collect the five course grades (CCR-002) ----
     Grades are whole or fractional numbers on a 0-100 scale (see
     "Answers to Questions for the Customer"). Each grade gets one
     re-prompt if the first entry is invalid (see the trade-off
     note at the top of this file), then is rejected outright. */
  printf("\nCourse Grades\n");
  gradesAreValid = true;

  printf("Course 1: ");
  gradeOk =
      (scanf("%lf", &grades[0]) == 1) && grades[0] >= 0 && grades[0] <= 100;
  if (!gradeOk) {
    scanf("%*[^\n]");
    scanf("%*c");
    printf("Invalid grade. Please enter a number between 0 and 100.\n");
    printf("Course 1: ");
    gradeOk =
        (scanf("%lf", &grades[0]) == 1) && grades[0] >= 0 && grades[0] <= 100;
    if (!gradeOk) {
      scanf("%*[^\n]");
      scanf("%*c");
    }
  }
  if (!gradeOk)
    gradesAreValid = false;

  printf("Course 2: ");
  gradeOk =
      (scanf("%lf", &grades[1]) == 1) && grades[1] >= 0 && grades[1] <= 100;
  if (!gradeOk) {
    scanf("%*[^\n]");
    scanf("%*c");
    printf("Invalid grade. Please enter a number between 0 and 100.\n");
    printf("Course 2: ");
    gradeOk =
        (scanf("%lf", &grades[1]) == 1) && grades[1] >= 0 && grades[1] <= 100;
    if (!gradeOk) {
      scanf("%*[^\n]");
      scanf("%*c");
    }
  }
  if (!gradeOk)
    gradesAreValid = false;

  printf("Course 3: ");
  gradeOk =
      (scanf("%lf", &grades[2]) == 1) && grades[2] >= 0 && grades[2] <= 100;
  if (!gradeOk) {
    scanf("%*[^\n]");
    scanf("%*c");
    printf("Invalid grade. Please enter a number between 0 and 100.\n");
    printf("Course 3: ");
    gradeOk =
        (scanf("%lf", &grades[2]) == 1) && grades[2] >= 0 && grades[2] <= 100;
    if (!gradeOk) {
      scanf("%*[^\n]");
      scanf("%*c");
    }
  }
  if (!gradeOk)
    gradesAreValid = false;

  printf("Course 4: ");
  gradeOk =
      (scanf("%lf", &grades[3]) == 1) && grades[3] >= 0 && grades[3] <= 100;
  if (!gradeOk) {
    scanf("%*[^\n]");
    scanf("%*c");
    printf("Invalid grade. Please enter a number between 0 and 100.\n");
    printf("Course 4: ");
    gradeOk =
        (scanf("%lf", &grades[3]) == 1) && grades[3] >= 0 && grades[3] <= 100;
    if (!gradeOk) {
      scanf("%*[^\n]");
      scanf("%*c");
    }
  }
  if (!gradeOk)
    gradesAreValid = false;

  printf("Course 5: ");
  gradeOk =
      (scanf("%lf", &grades[4]) == 1) && grades[4] >= 0 && grades[4] <= 100;
  if (!gradeOk) {
    scanf("%*[^\n]");
    scanf("%*c");
    printf("Invalid grade. Please enter a number between 0 and 100.\n");
    printf("Course 5: ");
    gradeOk =
        (scanf("%lf", &grades[4]) == 1) && grades[4] >= 0 && grades[4] <= 100;
    if (!gradeOk) {
      scanf("%*[^\n]");
      scanf("%*c");
    }
  }
  if (!gradeOk)
    gradesAreValid = false;

  if (!gradesAreValid) {
    printf("\nError: All course grades must be numbers between 0 and 100.\n");
    printf("Program terminated.\n");
    return 1;
  }

  /* ---- Calculate average, highest, and lowest grade ----
     No loop is used: with exactly five fixed array elements,
     the average is a straight-line sum, and the highest/lowest
     are found by comparing each element in turn. */
  averageGrade =
      (grades[0] + grades[1] + grades[2] + grades[3] + grades[4]) / 5.0;

  highestGrade = grades[0];
  if (grades[1] > highestGrade)
    highestGrade = grades[1];
  if (grades[2] > highestGrade)
    highestGrade = grades[2];
  if (grades[3] > highestGrade)
    highestGrade = grades[3];
  if (grades[4] > highestGrade)
    highestGrade = grades[4];

  lowestGrade = grades[0];
  if (grades[1] < lowestGrade)
    lowestGrade = grades[1];
  if (grades[2] < lowestGrade)
    lowestGrade = grades[2];
  if (grades[3] < lowestGrade)
    lowestGrade = grades[3];
  if (grades[4] < lowestGrade)
    lowestGrade = grades[4];

  /* ---- Display formatted summary (CCR-001 + CCR-002 together) ---- */
  printf("\n");
  printf("Student Summary\n");
  printf("%-14s: %ld\n", "Student ID", studentID);
  printf("%-14s: %s\n", "Student Name", studentName);
  printf("%-14s: %.2lf\n", "Current GPA", gpa);
  printf("----------------------------------------\n");
  printf("Course Grades\n");
  printf("%-14s: %.2f\n", "Course 1", grades[0]);
  printf("%-14s: %.2f\n", "Course 2", grades[1]);
  printf("%-14s: %.2f\n", "Course 3", grades[2]);
  printf("%-14s: %.2f\n", "Course 4", grades[3]);
  printf("%-14s: %.2f\n", "Course 5", grades[4]);
  printf("----------------------------------------\n");
  printf("%-14s: %.2lf\n", "Average Grade", averageGrade);
  printf("%-14s: %.2f\n", "Highest Grade", highestGrade);
  printf("%-14s: %.2f\n", "Lowest Grade", lowestGrade);

  return 0;
}
