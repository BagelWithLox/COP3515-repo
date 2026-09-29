/* ============================================================
   Student Information Management System (SIMS)
   COP 3515 - Advanced Program Design
   Client Change Request: CCR-004 (builds on CCR-001 through CCR-003)
   ============================================================
   SCOPE / FEATURE AUDIT (checked against CCR-004):
     Newly permitted this sprint: file processing (fopen/fclose),
     formatted file I/O (fprintf/fscanf), character processing
     (strcmp, used below to compare the recovered name and
     standing text), and type casting. Arrays, enum, switch,
     if-statements, and logical operators carry forward from
     CCR-002/CCR-003.
     Still out of scope: multiple students, user-defined
     functions, menus, searching/editing saved records, binary
     files, encryption, databases, dynamic memory allocation,
     multiple data files. Loops are still not listed as permitted
     (the CCR's own "Looking Ahead" section says loops are a
     future addition), so the codebase stays loop-free.
     FILE * is used only because file processing is explicitly
     permitted this week and is impossible without it; no other
     pointer variables are introduced (e.g. the recovered academic
     standing is stored as a char array, not a char *, same as
     studentName always has been).

     DESIGN NOTES:
       - Save-then-reload happens automatically in the same run,
         matching the CCR's Example 1 exactly: the program writes
         student_records.txt, then immediately reads it back and
         displays it as a "Recovered Student Record" separate
         from the normal CCR-001/002/003 report.
       - Business requirement #7 (let an employee verify the
         recovered data matches the original) is implemented as
         an actual in-program comparison, not just two printouts
         for a human to eyeball. GPA and grades are compared using
         type casting ((long)(value * 100 + 0.5)) instead of raw
         double equality, to avoid floating-point rounding issues
         - this is the newly permitted "type casting" concept.
         Name and academic standing are compared with strcmp(),
         using this week's newly permitted character processing.
       - Academic standing is written to the file as plain text
         (e.g. "Honors") and read back as text, not recalculated
         from the recovered GPA. Recalculating would make the
         standing always match by definition and defeat the point
         of a real verification step (see Question 8 below).

     ASSUMPTIONS (no further customer answers supplied this
     sprint, so these are my own reasonable defaults per the
     submission's "Assumptions" requirement):
       - File name is fixed as "student_records.txt", per the
         CCR's own error-message example.
       - The file is overwritten (not appended) each run, since
         the CCR states the system only manages one student at a
         time - appending would just duplicate that one record.
       - The program always reads the file back immediately after
         saving, matching Example 1.
       - No blank lines are written between fields, since fscanf
         does not need them and it keeps the file simple to parse.
       - If the file cannot be opened at all, the program shows
         the CCR's exact Example 3 error and exits. If the file
         opens but its contents are incomplete or malformed, a
         separate "corrupted file" message is shown instead, to
         directly answer Question 9 about invalid/incomplete data.
   ============================================================ */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum AcademicStanding {
  HONORS,
  GOOD_STANDING,
  ACADEMIC_PROBATION,
  ACADEMIC_SUSPENSION
};

int main(void) {
  /* ---- Constants describing this program ---- */
  const char COURSE_TITLE[] = "Student Information Management System";
  const char PROGRAMMER_NAME[] = "Samuel Shivarig";
  const char VERSION_NUMBER[] = "4.0";
  const char FILE_NAME[] = "student_records.txt";

  /* ---- Variables to hold the student's data (CCR-001) ---- */
  long studentID;
  char studentName[50];
  double gpa;
  bool idIsValid;
  bool nameIsValid;
  bool gpaIsValid;
  char nextChar; /* holds the character right after the ID digits */

  /* ---- Variable to hold academic standing (CCR-003) ---- */
  enum AcademicStanding standing;

  /* ---- Variables to hold the course grades (CCR-002) ---- */
  double grades[5];
  bool gradesAreValid;
  bool gradeOk;
  double averageGrade;
  double highestGrade;
  double lowestGrade;

  /* ---- Variables for saving/loading the record (CCR-004) ---- */
  FILE *filePtr;
  long loadedID;
  char loadedName[50];
  double loadedGPA;
  char loadedStanding[30];
  double loadedGrades[5];
  bool loadOk;
  bool idMatches;
  bool nameMatches;
  bool gpaMatches;
  bool standingMatches;
  bool gradesMatch;
  bool allMatch;

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

  /* ---- Collect GPA (must be numeric and between 0.00 and 4.00) ----
     CCR-003 tightens this from CCR-001's "not negative" check to
     the full valid range, and defines the exact error message
     (see Example 5 in the CCR). */
  printf("Current GPA: ");
  gpaIsValid = (scanf("%lf", &gpa) == 1);

  if (gpaIsValid && (gpa < 0.00 || gpa > 4.00)) {
    gpaIsValid = false;
  }

  if (!gpaIsValid) {
    printf("\nERROR\n");
    printf("Invalid GPA entered.\n");
    printf("GPA must be between 0.00 and 4.00.\n");
    printf("Program terminated.\n");
    return 1;
  }

  /* ---- Determine academic standing (CCR-003) ----
     Cascading from the top down avoids any gap between the
     table's listed bands (see DESIGN NOTE at the top of this
     file). Since gpa is already confirmed to be in [0.00, 4.00],
     the final else covers Academic Suspension correctly. */
  if (gpa >= 3.50) {
    standing = HONORS;
  } else if (gpa >= 2.00) {
    standing = GOOD_STANDING;
  } else if (gpa >= 1.00) {
    standing = ACADEMIC_PROBATION;
  } else {
    standing = ACADEMIC_SUSPENSION;
  }

  /* ---- Collect the five course grades (CCR-002) ----
     Grades are whole or fractional numbers on a 0-100 scale (see
     "Answers to Questions for the Customer"). Each grade gets one
     re-prompt if the first entry is invalid, then is rejected
     outright. */
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

  /* ---- Display formatted summary (CCR-001 + CCR-002 + CCR-003) ---- */
  printf("\n");
  printf("Student Summary\n");
  printf("%-18s: %ld\n", "Student ID", studentID);
  printf("%-18s: %s\n", "Student Name", studentName);
  printf("%-18s: %.2lf\n", "Current GPA", gpa);

  printf("%-18s: ", "Academic Standing");
  switch (standing) {
  case HONORS:
    printf("Honors\n");
    break;
  case GOOD_STANDING:
    printf("Good Standing\n");
    break;
  case ACADEMIC_PROBATION:
    printf("Academic Probation\n");
    break;
  case ACADEMIC_SUSPENSION:
    printf("Academic Suspension\n");
    break;
  }

  printf("----------------------------------------\n");
  printf("Course Grades\n");
  printf("%-18s: %.2f\n", "Course 1", grades[0]);
  printf("%-18s: %.2f\n", "Course 2", grades[1]);
  printf("%-18s: %.2f\n", "Course 3", grades[2]);
  printf("%-18s: %.2f\n", "Course 4", grades[3]);
  printf("%-18s: %.2f\n", "Course 5", grades[4]);
  printf("----------------------------------------\n");
  printf("%-18s: %.2lf\n", "Average Grade", averageGrade);
  printf("%-18s: %.2f\n", "Highest Grade", highestGrade);
  printf("%-18s: %.2f\n", "Lowest Grade", lowestGrade);

  /* ---- Save the student record to a text file (CCR-004) ---- */
  filePtr = fopen(FILE_NAME, "w");
  if (filePtr == NULL) {
    printf("\nERROR\n");
    printf("Unable to open %s for writing.\n", FILE_NAME);
    printf(
        "Please verify that you have permission to write to this location.\n");
    return 1;
  }

  fprintf(filePtr, "%ld\n", studentID);
  fprintf(filePtr, "%s\n", studentName);
  fprintf(filePtr, "%.2lf\n", gpa);

  switch (standing) {
  case HONORS:
    fprintf(filePtr, "Honors\n");
    break;
  case GOOD_STANDING:
    fprintf(filePtr, "Good Standing\n");
    break;
  case ACADEMIC_PROBATION:
    fprintf(filePtr, "Academic Probation\n");
    break;
  case ACADEMIC_SUSPENSION:
    fprintf(filePtr, "Academic Suspension\n");
    break;
  }

  fprintf(filePtr, "%.2f\n", grades[0]);
  fprintf(filePtr, "%.2f\n", grades[1]);
  fprintf(filePtr, "%.2f\n", grades[2]);
  fprintf(filePtr, "%.2f\n", grades[3]);
  fprintf(filePtr, "%.2f\n", grades[4]);

  fclose(filePtr);

  printf("\nStudent information successfully saved.\n");

  /* ---- Read the student record back from the file (CCR-004) ----
     The file is always read back immediately after saving, to
     match the CCR's Example 1. If the file cannot be opened at
     all, this shows the CCR's exact Example 3 error message. */
  printf("Reading student information...\n");

  filePtr = fopen(FILE_NAME, "r");
  if (filePtr == NULL) {
    printf("\nERROR\n");
    printf("Unable to open %s\n", FILE_NAME);
    printf("Please verify that the file exists and that you have permission to "
           "access it.\n");
    return 1;
  }

  loadOk = (fscanf(filePtr, "%ld", &loadedID) == 1);
  if (fscanf(filePtr, " %49[^\n]", loadedName) != 1)
    loadOk = false;
  if (fscanf(filePtr, "%lf", &loadedGPA) != 1)
    loadOk = false;
  if (fscanf(filePtr, " %29[^\n]", loadedStanding) != 1)
    loadOk = false;
  if (fscanf(filePtr, "%lf", &loadedGrades[0]) != 1)
    loadOk = false;
  if (fscanf(filePtr, "%lf", &loadedGrades[1]) != 1)
    loadOk = false;
  if (fscanf(filePtr, "%lf", &loadedGrades[2]) != 1)
    loadOk = false;
  if (fscanf(filePtr, "%lf", &loadedGrades[3]) != 1)
    loadOk = false;
  if (fscanf(filePtr, "%lf", &loadedGrades[4]) != 1)
    loadOk = false;

  fclose(filePtr);

  if (!loadOk) {
    printf("\nERROR\n");
    printf("The data file exists but could not be read correctly.\n");
    printf("The file may be corrupted or incomplete.\n");
    return 1;
  }

  printf("File successfully loaded.\n");

  /* ---- Display the recovered record (CCR-004) ----
     Formatted to match the CCR's Example 1 layout for this
     section specifically, which is simpler than the live report
     above (no aligned label columns, grades listed as bare
     numbers). */
  printf("\nRecovered Student Record\n");
  printf("----------------------------------------\n");
  printf("%s\n", COURSE_TITLE);
  printf("Version %s\n", VERSION_NUMBER);
  printf("----------------------------------------\n");
  printf("Student ID : %ld\n", loadedID);
  printf("Student Name : %s\n", loadedName);
  printf("Current GPA : %.2lf\n", loadedGPA);
  printf("Academic Standing : %s\n", loadedStanding);
  printf("Course Grades\n");
  printf("%.2f\n", loadedGrades[0]);
  printf("%.2f\n", loadedGrades[1]);
  printf("%.2f\n", loadedGrades[2]);
  printf("%.2f\n", loadedGrades[3]);
  printf("%.2f\n", loadedGrades[4]);
  printf("Average Grade : %.2lf\n",
         (loadedGrades[0] + loadedGrades[1] + loadedGrades[2] +
          loadedGrades[3] + loadedGrades[4]) /
             5.0);
  printf("Highest Grade : %.2f\n", highestGrade);
  printf("Lowest Grade : %.2f\n", lowestGrade);

  /* ---- Verify the recovered data matches the original (CCR-004) ----
     GPA and grades use type casting to compare as whole hundredths
     instead of raw doubles, avoiding floating-point rounding
     issues. Name and standing use strcmp() for exact text match. */
  idMatches = (studentID == loadedID);
  nameMatches = (strcmp(studentName, loadedName) == 0);
  gpaMatches = ((long)(gpa * 100 + 0.5) == (long)(loadedGPA * 100 + 0.5));

  switch (standing) {
  case HONORS:
    standingMatches = (strcmp(loadedStanding, "Honors") == 0);
    break;
  case GOOD_STANDING:
    standingMatches = (strcmp(loadedStanding, "Good Standing") == 0);
    break;
  case ACADEMIC_PROBATION:
    standingMatches = (strcmp(loadedStanding, "Academic Probation") == 0);
    break;
  case ACADEMIC_SUSPENSION:
    standingMatches = (strcmp(loadedStanding, "Academic Suspension") == 0);
    break;
  default:
    standingMatches = false;
    break;
  }

  gradesMatch = true;
  if ((long)(grades[0] * 100 + 0.5) != (long)(loadedGrades[0] * 100 + 0.5))
    gradesMatch = false;
  if ((long)(grades[1] * 100 + 0.5) != (long)(loadedGrades[1] * 100 + 0.5))
    gradesMatch = false;
  if ((long)(grades[2] * 100 + 0.5) != (long)(loadedGrades[2] * 100 + 0.5))
    gradesMatch = false;
  if ((long)(grades[3] * 100 + 0.5) != (long)(loadedGrades[3] * 100 + 0.5))
    gradesMatch = false;
  if ((long)(grades[4] * 100 + 0.5) != (long)(loadedGrades[4] * 100 + 0.5))
    gradesMatch = false;

  allMatch =
      +idMatches && nameMatches && gpaMatches && standingMatches && gradesMatch;

  printf("\n----------------------------------------\n");
  printf("Verification\n");
  printf("----------------------------------------\n");
  if (allMatch) {
    printf("The recovered data matches the original entry exactly.\n");
  } else {
    printf("WARNING: The recovered data does NOT match the original entry.\n");
  }

  return 0;
}
