/* ============================================================
   Student Information Management System (SIMS)
   COP 3515 - Advanced Program Design
   Client Change Request: CCR-006 (builds on CCR-001 through CCR-005)
   ============================================================
   PURPOSE: Refactoring only. The program behaves exactly like
   the CCR-005 version, but every major task is now its own
   function and main() just calls them.

   FUNCTION OVERVIEW:
     main
      +-- displayHeader
      +-- displayMainMenu
      +-- getMenuSelection
      |     +-- readLine
      +-- processMenuSelection
            +-- addStudent
            |     +-- readLine
            |     +-- readStudentName
            |     |     +-- readLine
            |     +-- calculateStanding
            +-- displayStudentReport
            |     +-- getStandingText
            +-- enterGrades
            |     +-- readLine
            |     +-- calculateGradeStatistics
            +-- saveStudentRecord
            |     +-- getStandingText
            +-- displayExitMessage
            +-- displayEndOfInputMessage
            +-- displayInvalidSelection

   DESIGN NOTES / ASSUMPTIONS:
     - Pointers are out of scope, so the one student's data is
       stored in file-scope (global) variables. That lets the
       functions share the data without pointer parameters.
       Arrays are passed as parameters where needed.
     - Validation errors never end the program. Each function
       prints its error and returns to the menu.
     - Add Student and Enter Grades read into temporary
       variables and only commit when everything is valid.
     - Reading student information from a file is NOT
       implemented (the CCR says "if implemented").
   ============================================================ */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define NUM_COURSES 5
#define NAME_SIZE 50
#define LINE_SIZE 256
#define STANDING_SIZE 30

enum AcademicStanding {
  HONORS,
  GOOD_STANDING,
  ACADEMIC_PROBATION,
  ACADEMIC_SUSPENSION
};

/* INPUT_ERROR and INPUT_EOF are special values that
   getMenuSelection() returns. 1 through 5 are real choices. */
enum MenuOption {
  INPUT_EOF = -1,
  INPUT_ERROR = 0,
  MENU_ADD = 1,
  MENU_DISPLAY,
  MENU_GRADES,
  MENU_SAVE,
  MENU_EXIT
};

enum LineStatus { LINE_OK, LINE_TOO_LONG, LINE_EOF };

/* ---- Constants describing this program ---- */
const char COURSE_TITLE[] = "Student Information Management System";
const char VERSION_NUMBER[] = "6.0";
const char FILE_NAME[] = "student_records.txt";

/* ---- The student's data (shared by the functions) ---- */
long studentID = 0;
char studentName[NAME_SIZE] = "";
double gpa = 0.0;
enum AcademicStanding standing = ACADEMIC_SUSPENSION;
double grades[NUM_COURSES] = {0, 0, 0, 0, 0};
double averageGrade = 0.0;
double highestGrade = 0.0;
double lowestGrade = 0.0;
bool studentAdded = false;
bool gradesEntered = false;

/* ---- Function prototypes ---- */
void displayHeader(void);
void displayMainMenu(void);
enum LineStatus readLine(char line[]);
int getMenuSelection(void);
bool processMenuSelection(int choice);
void addStudent(void);
bool readStudentName(char name[]);
enum AcademicStanding calculateStanding(double studentGpa);
void getStandingText(enum AcademicStanding level, char text[]);
void enterGrades(void);
void calculateGradeStatistics(void);
void displayStudentReport(void);
void saveStudentRecord(void);
void displayExitMessage(void);
void displayEndOfInputMessage(void);
void displayInvalidSelection(void);

int main(void) {
  bool keepRunning = true;
  int choice;

  while (keepRunning) {
    displayHeader();
    displayMainMenu();
    choice = getMenuSelection();
    keepRunning = processMenuSelection(choice);
  }

  return 0;
}

/* Prints the program banner. */
void displayHeader(void) {
  printf("----------------------------------------\n");
  printf("%s\n", COURSE_TITLE);
  printf("Version %s\n", VERSION_NUMBER);
  printf("----------------------------------------\n");
}

/* Prints the menu options. */
void displayMainMenu(void) {
  printf("1. Add Student\n");
  printf("2. Display Student\n");
  printf("3. Enter Grades\n");
  printf("4. Save Student Record\n");
  printf("5. Exit\n");
}

/* Reads one whole line from the keyboard into line[] (which must
   hold LINE_SIZE characters). If the line was too long, the extra
   characters are thrown away so they do not spill into the next
   prompt. Returns LINE_OK, LINE_TOO_LONG, or LINE_EOF. */
enum LineStatus readLine(char line[]) {
  size_t len;
  int ch;

  if (fgets(line, LINE_SIZE, stdin) == NULL) {
    return LINE_EOF;
  }

  len = strlen(line);
  if (len == LINE_SIZE - 1 && line[len - 1] != '\n') {
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
    return LINE_TOO_LONG;
  }

  return LINE_OK;
}

/* Prompts for a menu choice. Returns 1-5 for a valid choice,
   INPUT_ERROR for anything invalid, or INPUT_EOF if the input
   ran out. */
int getMenuSelection(void) {
  char line[LINE_SIZE];
  char junk;
  long value;
  enum LineStatus status;

  printf("Selection: ");
  status = readLine(line);

  if (status == LINE_EOF) {
    return INPUT_EOF;
  }
  if (status == LINE_TOO_LONG || sscanf(line, "%ld %c", &value, &junk) != 1) {
    return INPUT_ERROR;
  }
  if (value < MENU_ADD || value > MENU_EXIT) {
    return INPUT_ERROR;
  }

  return (int)value;
}

/* Runs the task that matches the menu choice.
   Returns true if the program should keep running, false if it
   should stop. */
bool processMenuSelection(int choice) {
  bool keepRunning = true;

  switch (choice) {
  case MENU_ADD:
    addStudent();
    break;
  case MENU_DISPLAY:
    displayStudentReport();
    break;
  case MENU_GRADES:
    enterGrades();
    break;
  case MENU_SAVE:
    saveStudentRecord();
    break;
  case MENU_EXIT:
    displayExitMessage();
    keepRunning = false;
    break;
  case INPUT_EOF:
    displayEndOfInputMessage();
    keepRunning = false;
    break;
  default:
    displayInvalidSelection();
    break;
  }

  return keepRunning;
}

/* Asks for the ID, name, and GPA. Nothing is saved unless all
   three are valid. */
void addStudent(void) {
  char line[LINE_SIZE];
  char junk;
  char newName[NAME_SIZE];
  long newID = 0;
  double newGpa = 0.0;
  bool idIsValid = false;
  bool gpaIsValid = false;
  enum LineStatus status;

  printf("\n");

  /* Student ID */
  printf("Student ID: ");
  status = readLine(line);
  if (status == LINE_OK) {
    idIsValid = (sscanf(line, "%ld %c", &newID, &junk) == 1) && newID >= 0;
  }
  if (!idIsValid) {
    printf("\nError: Student ID must be a positive whole number.\n\n");
    return;
  }

  /* Student Name */
  printf("Student Name: ");
  if (!readStudentName(newName)) {
    printf("\nError: Student Name must begin with a letter.\n\n");
    return;
  }

  /* GPA */
  printf("Current GPA: ");
  status = readLine(line);
  if (status == LINE_OK) {
    /* Written as an "in range" test so NaN is rejected. */
    gpaIsValid = (sscanf(line, "%lf %c", &newGpa, &junk) == 1) &&
                 (newGpa >= 0.00 && newGpa <= 4.00);
  }
  if (!gpaIsValid) {
    printf("\nERROR\n");
    printf("Invalid GPA entered.\n");
    printf("GPA must be between 0.00 and 4.00.\n\n");
    return;
  }

  /* Everything is valid, so save it. */
  studentID = newID;
  strcpy(studentName, newName);
  gpa = newGpa;
  standing = calculateStanding(newGpa);
  studentAdded = true;
  gradesEntered = false; /* a new student has no grades yet */

  printf("\nStudent successfully added.\n\n");
}

/* Reads a name into name[] (must hold NAME_SIZE characters).
   Leading spaces are skipped, trailing spaces/newline are removed,
   and names longer than 49 characters are cut off.
   Returns true if the name starts with a letter. */
bool readStudentName(char name[]) {
  char line[LINE_SIZE];
  int start = 0;
  size_t len;

  if (readLine(line) == LINE_EOF) {
    return false;
  }

  while (line[start] == ' ' || line[start] == '\t') {
    start++;
  }

  if (!((line[start] >= 'A' && line[start] <= 'Z') ||
        (line[start] >= 'a' && line[start] <= 'z'))) {
    return false;
  }

  strncpy(name, line + start, NAME_SIZE - 1);
  name[NAME_SIZE - 1] = '\0';

  len = strlen(name);
  while (len > 0 && (name[len - 1] == '\n' || name[len - 1] == '\r' ||
                     name[len - 1] == ' ' || name[len - 1] == '\t')) {
    name[--len] = '\0';
  }

  return true;
}

/* Returns the academic standing for a GPA. */
enum AcademicStanding calculateStanding(double studentGpa) {
  if (studentGpa >= 3.50) {
    return HONORS;
  } else if (studentGpa >= 2.00) {
    return GOOD_STANDING;
  } else if (studentGpa >= 1.00) {
    return ACADEMIC_PROBATION;
  }
  return ACADEMIC_SUSPENSION;
}

/* Copies the words for a standing into text[] (must hold
   STANDING_SIZE characters). Used by both the display and the
   save functions so the wording is only written once. */
void getStandingText(enum AcademicStanding level, char text[]) {
  switch (level) {
  case HONORS:
    strcpy(text, "Honors");
    break;
  case GOOD_STANDING:
    strcpy(text, "Good Standing");
    break;
  case ACADEMIC_PROBATION:
    strcpy(text, "Academic Probation");
    break;
  case ACADEMIC_SUSPENSION:
    strcpy(text, "Academic Suspension");
    break;
  }
}

/* Asks for all five course grades. Each course gets two tries.
   Nothing is saved unless all five are valid. */
void enterGrades(void) {
  char line[LINE_SIZE];
  char junk;
  double newGrades[NUM_COURSES];
  bool gradeOk;
  bool gradesAreValid = true;
  int attempt;
  int i;
  enum LineStatus status;

  if (!studentAdded) {
    printf("\nPlease add a student before entering grades.\n\n");
    return;
  }

  printf("\n");

  for (i = 0; i < NUM_COURSES && gradesAreValid; i++) {
    gradeOk = false;
    attempt = 0;
    while (attempt < 2 && !gradeOk) {
      printf("Course %d: ", i + 1);
      status = readLine(line);
      if (status == LINE_EOF) {
        break; /* end of input */
      }
      gradeOk = (status == LINE_OK) &&
                (sscanf(line, "%lf %c", &newGrades[i], &junk) == 1) &&
                (newGrades[i] >= 0 && newGrades[i] <= 100);
      if (!gradeOk && attempt == 0) {
        printf("Invalid grade. Please enter a number between 0 and 100.\n");
      }
      attempt++;
    }
    if (!gradeOk) {
      gradesAreValid = false;
    }
  }

  if (!gradesAreValid) {
    printf("\nError: All course grades must be numbers between 0 and "
           "100.\n\n");
    return; /* old grades (if any) are left alone */
  }

  for (i = 0; i < NUM_COURSES; i++) {
    grades[i] = newGrades[i];
  }
  calculateGradeStatistics();
  gradesEntered = true;

  printf("\nGrades successfully recorded.\n\n");
}

/* Finds the average, highest, and lowest of the saved grades. */
void calculateGradeStatistics(void) {
  double sum = 0.0;
  int i;

  highestGrade = grades[0];
  lowestGrade = grades[0];

  for (i = 0; i < NUM_COURSES; i++) {
    sum += grades[i];
    if (grades[i] > highestGrade) {
      highestGrade = grades[i];
    }
    if (grades[i] < lowestGrade) {
      lowestGrade = grades[i];
    }
  }

  averageGrade = sum / NUM_COURSES;
}

/* Prints the student summary report. */
void displayStudentReport(void) {
  char standingText[STANDING_SIZE];
  int i;

  if (!studentAdded) {
    printf("\nNo student information has been entered yet.\n");
    printf("Please choose option 1 to add a student.\n\n");
    return;
  }

  getStandingText(standing, standingText);

  printf("\n----------------------------------------\n");
  printf("Student Summary\n");
  printf("----------------------------------------\n");
  printf("Student ID : %ld\n", studentID);
  printf("Student Name : %s\n", studentName);
  printf("Current GPA : %.2lf\n", gpa);
  printf("Academic Standing : %s\n", standingText);

  if (!gradesEntered) {
    printf("\nCourse grades have not been entered yet.\n");
    printf("Please choose option 3 to enter grades.\n\n");
    return;
  }

  printf("Course Grades\n");
  for (i = 0; i < NUM_COURSES; i++) {
    printf("%.2f\n", grades[i]);
  }
  printf("Average Grade : %.2lf\n", averageGrade);
  printf("Highest Grade : %.2f\n", highestGrade);
  printf("Lowest Grade : %.2f\n", lowestGrade);
  printf("\n");
}

/* Writes the student record to student_records.txt. */
void saveStudentRecord(void) {
  FILE *filePtr;
  char standingText[STANDING_SIZE];
  int i;

  if (!studentAdded) {
    printf("\nNo student information to save.\n");
    printf("Please choose option 1 to add a student.\n\n");
    return;
  }
  if (!gradesEntered) {
    printf("\nCannot save: course grades have not been entered yet.\n");
    printf("Please choose option 3 to enter grades.\n\n");
    return;
  }

  filePtr = fopen(FILE_NAME, "w");
  if (filePtr == NULL) {
    printf("\nERROR\n");
    printf("Unable to open %s for writing.\n", FILE_NAME);
    printf("Please verify that you have permission to write to this "
           "location.\n\n");
    return;
  }

  getStandingText(standing, standingText);

  fprintf(filePtr, "%ld\n", studentID);
  fprintf(filePtr, "%s\n", studentName);
  fprintf(filePtr, "%.2lf\n", gpa);
  fprintf(filePtr, "%s\n", standingText);
  for (i = 0; i < NUM_COURSES; i++) {
    fprintf(filePtr, "%.2f\n", grades[i]);
  }

  if (fclose(filePtr) != 0) {
    printf("\nERROR\n");
    printf("A problem occurred while writing %s.\n\n", FILE_NAME);
    return;
  }

  printf("\nStudent record successfully saved.\n");
  printf("%s created.\n\n", FILE_NAME);
}

/* Goodbye message for the Exit option. */
void displayExitMessage(void) {
  printf("\nThank you for using the\n");
  printf("Student Information Management System.\n");
  printf("Program terminated successfully.\n");
}

/* Message shown when the input ends unexpectedly (Ctrl+D / Ctrl+Z). */
void displayEndOfInputMessage(void) {
  printf("\n\nEnd of input reached.\n");
  printf("Program terminated.\n");
}

/* Error message for a bad menu choice. */
void displayInvalidSelection(void) {
  printf("\nERROR\n");
  printf("Invalid menu selection.\n");
  printf("Please choose an option between 1 and 5.\n\n");
}
