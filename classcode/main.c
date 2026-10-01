/* ============================================================
   Student Information Management System (SIMS)
   COP 3515 - Advanced Program Design
   Client Change Request: CCR-005 (builds on CCR-001 through CCR-004)
   ============================================================
   SCOPE: while/for loops, break, continue, arrays, enum, switch,
   if-statements, logical operators, file and character processing.
   No user-defined functions (still out of scope), so the whole
   menu system lives in main() with one while loop and one switch.

   DESIGN: validation errors never terminate the program. Each menu
   operation prints its error and returns to the menu.

   BUG-FIX NOTES (revision after bug test):
     1. INPUT HANDLING: every prompt now reads one whole line with
        fgets() and parses it with sscanf("%x %c"). A result of
        exactly 1 means "a value and nothing else on the line".
        This fixes: leftover characters from a rejected entry being
        read as the next menu choice, "12abc" / "3.5xyz" / "2.5"
        being accepted, extra tokens ("3.5 2") spilling into the
        menu, and Enter on an empty line blocking silently.
     2. EOF: end of input (Ctrl+D / Ctrl+Z, piped input running out)
        used to cause an infinite loop of "Invalid menu selection".
        It now exits cleanly.
     3. ATOMIC UPDATES: Add Student and Enter Grades now read into
        temporary variables and only commit on full success. Before,
        a failed re-add left a NEW ID paired with the OLD name/GPA,
        and a failed re-entry of grades left half-overwritten grades
        next to a stale average/highest/lowest.
     4. NaN GPA: "nan" slipped through the old (gpa < 0 || gpa > 4)
        range test. The test is now written as an "is within range"
        check, which rejects NaN.
     5. LONG NAMES: a name over 49 characters used to leave the
        remainder in the buffer and corrupt the GPA prompt. The extra
        is now discarded (name is truncated to 49 characters).
     6. GRADES: the five copy-pasted blocks are now one for loop
        (loops are permitted this sprint). Entry also stops at the
        first course that fails twice instead of continuing to
        prompt for the remaining courses.
     7. Save checks the result of fclose() as well as fopen().

   ASSUMPTIONS:
       - Menu banner matches the CCR example exactly.
       - Save Student Record only writes the file and prints the
         two-line success message.
       - Enter Grades / Save before Add Student returns a message.
       - Adding a student again replaces the one in memory and
         resets gradesEntered to false.
       - Non-numeric / trailing-junk menu input is treated the same
         as an out-of-range selection.
       - Student ID of 0 is accepted (unchanged from earlier
         behavior) even though the message says "positive".
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
  const char VERSION_NUMBER[] = "5.0";
  const char FILE_NAME[] = "student_records.txt";

  /* ---- Persistent student data (lives across menu operations) ---- */
  long studentID = 0;
  char studentName[50] = "";
  double gpa = 0.0;
  enum AcademicStanding standing = ACADEMIC_SUSPENSION;
  double grades[5] = {0, 0, 0, 0, 0};
  double averageGrade = 0.0;
  double highestGrade = 0.0;
  double lowestGrade = 0.0;
  bool studentAdded = false;
  bool gradesEntered = false;

  /* ---- Menu control ---- */
  int choice = 0;
  bool choiceIsValid;
  bool keepRunning = true;

  /* ---- Temporary values (committed only when everything is valid) ---- */
  long newID = 0;
  char newName[50];
  double newGpa = 0.0;
  enum AcademicStanding newStanding;
  double newGrades[5];

  /* ---- Line-reading / validation scratch ---- */
  char line[256];
  size_t len;
  int ch;
  int start;
  char junk;
  bool tooLong;
  bool idIsValid;
  bool nameIsValid;
  bool gpaIsValid;
  bool gradeOk;
  bool gradesAreValid;
  int i;
  int attempt;
  double sum;
  FILE *filePtr;

  while (keepRunning) {
    printf("----------------------------------------\n");
    printf("%s\n", COURSE_TITLE);
    printf("Version %s\n", VERSION_NUMBER);
    printf("----------------------------------------\n");
    printf("1. Add Student\n");
    printf("2. Display Student\n");
    printf("3. Enter Grades\n");
    printf("4. Save Student Record\n");
    printf("5. Exit\n");
    printf("Selection: ");

    if (fgets(line, sizeof line, stdin) == NULL) {
      /* End of input: exit cleanly instead of looping forever. */
      printf("\n\nEnd of input reached.\n");
      printf("Program terminated.\n");
      break;
    }

    tooLong = false;
    len = strlen(line);
    if (len == sizeof line - 1 && line[len - 1] != '\n') {
      tooLong = true;
      while ((ch = getchar()) != '\n' && ch != EOF) {
      }
    }

    choiceIsValid = !tooLong && (sscanf(line, "%d %c", &choice, &junk) == 1);

    if (!choiceIsValid) {
      printf("\nERROR\n");
      printf("Invalid menu selection.\n");
      printf("Please choose an option between 1 and 5.\n\n");
      continue;
    }

    switch (choice) {

    case 1: {
      /* ---- Add Student ---- */
      printf("\n");

      /* Student ID */
      printf("Student ID: ");
      idIsValid = false;
      if (fgets(line, sizeof line, stdin) != NULL) {
        tooLong = false;
        len = strlen(line);
        if (len == sizeof line - 1 && line[len - 1] != '\n') {
          tooLong = true;
          while ((ch = getchar()) != '\n' && ch != EOF) {
          }
        }
        idIsValid = !tooLong && (sscanf(line, "%ld %c", &newID, &junk) == 1) &&
                    newID >= 0;
      }
      if (!idIsValid) {
        printf("\nError: Student ID must be a positive whole number.\n\n");
        break;
      }

      /* Student Name */
      printf("Student Name: ");
      nameIsValid = false;
      if (fgets(line, sizeof line, stdin) != NULL) {
        len = strlen(line);
        if (len == sizeof line - 1 && line[len - 1] != '\n') {
          while ((ch = getchar()) != '\n' && ch != EOF) {
          }
        }
        start = 0;
        while (line[start] == ' ' || line[start] == '\t') {
          start++;
        }
        nameIsValid = (line[start] >= 'A' && line[start] <= 'Z') ||
                      (line[start] >= 'a' && line[start] <= 'z');
        if (nameIsValid) {
          strncpy(newName, line + start, sizeof newName - 1);
          newName[sizeof newName - 1] = '\0';
          len = strlen(newName);
          while (len > 0 &&
                 (newName[len - 1] == '\n' || newName[len - 1] == '\r' ||
                  newName[len - 1] == ' ' || newName[len - 1] == '\t')) {
            newName[--len] = '\0';
          }
        }
      }
      if (!nameIsValid) {
        printf("\nError: Student Name must begin with a letter.\n\n");
        break;
      }

      /* GPA */
      printf("Current GPA: ");
      gpaIsValid = false;
      if (fgets(line, sizeof line, stdin) != NULL) {
        tooLong = false;
        len = strlen(line);
        if (len == sizeof line - 1 && line[len - 1] != '\n') {
          tooLong = true;
          while ((ch = getchar()) != '\n' && ch != EOF) {
          }
        }
        /* Written as an "in range" test so NaN is rejected. */
        gpaIsValid = !tooLong &&
                     (sscanf(line, "%lf %c", &newGpa, &junk) == 1) &&
                     (newGpa >= 0.00 && newGpa <= 4.00);
      }
      if (!gpaIsValid) {
        printf("\nERROR\n");
        printf("Invalid GPA entered.\n");
        printf("GPA must be between 0.00 and 4.00.\n\n");
        break;
      }

      if (newGpa >= 3.50) {
        newStanding = HONORS;
      } else if (newGpa >= 2.00) {
        newStanding = GOOD_STANDING;
      } else if (newGpa >= 1.00) {
        newStanding = ACADEMIC_PROBATION;
      } else {
        newStanding = ACADEMIC_SUSPENSION;
      }

      /* Everything valid: commit. */
      studentID = newID;
      strcpy(studentName, newName);
      gpa = newGpa;
      standing = newStanding;
      studentAdded = true;
      gradesEntered = false; /* new/replaced student has no grades yet */

      printf("\nStudent successfully added.\n\n");
      break;
    }

    case 2: {
      /* ---- Display Student ---- */
      if (!studentAdded) {
        printf("\nNo student information has been entered yet.\n");
        printf("Please choose option 1 to add a student.\n\n");
        break;
      }

      printf("\n----------------------------------------\n");
      printf("Student Summary\n");
      printf("----------------------------------------\n");
      printf("Student ID : %ld\n", studentID);
      printf("Student Name : %s\n", studentName);
      printf("Current GPA : %.2lf\n", gpa);
      printf("Academic Standing : ");
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

      if (!gradesEntered) {
        printf("\nCourse grades have not been entered yet.\n");
        printf("Please choose option 3 to enter grades.\n\n");
        break;
      }

      printf("Course Grades\n");
      for (i = 0; i < 5; i++) {
        printf("%.2f\n", grades[i]);
      }
      printf("Average Grade : %.2lf\n", averageGrade);
      printf("Highest Grade : %.2f\n", highestGrade);
      printf("Lowest Grade : %.2f\n", lowestGrade);
      printf("\n");
      break;
    }

    case 3: {
      /* ---- Enter Grades ---- */
      if (!studentAdded) {
        printf("\nPlease add a student before entering grades.\n\n");
        break;
      }

      printf("\n");
      gradesAreValid = true;

      for (i = 0; i < 5 && gradesAreValid; i++) {
        gradeOk = false;
        attempt = 0;
        while (attempt < 2 && !gradeOk) {
          printf("Course %d: ", i + 1);
          if (fgets(line, sizeof line, stdin) == NULL) {
            break; /* end of input: leave gradeOk false */
          }
          tooLong = false;
          len = strlen(line);
          if (len == sizeof line - 1 && line[len - 1] != '\n') {
            tooLong = true;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
          }
          gradeOk = !tooLong &&
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
        break; /* previous grades (if any) are left untouched */
      }

      sum = 0.0;
      highestGrade = newGrades[0];
      lowestGrade = newGrades[0];
      for (i = 0; i < 5; i++) {
        grades[i] = newGrades[i];
        sum += newGrades[i];
        if (newGrades[i] > highestGrade) {
          highestGrade = newGrades[i];
        }
        if (newGrades[i] < lowestGrade) {
          lowestGrade = newGrades[i];
        }
      }
      averageGrade = sum / 5.0;

      gradesEntered = true;
      printf("\nGrades successfully recorded.\n\n");
      break;
    }

    case 4: {
      /* ---- Save Student Record ---- */
      if (!studentAdded) {
        printf("\nNo student information to save.\n");
        printf("Please choose option 1 to add a student.\n\n");
        break;
      }
      if (!gradesEntered) {
        printf("\nCannot save: course grades have not been entered yet.\n");
        printf("Please choose option 3 to enter grades.\n\n");
        break;
      }

      filePtr = fopen(FILE_NAME, "w");
      if (filePtr == NULL) {
        printf("\nERROR\n");
        printf("Unable to open %s for writing.\n", FILE_NAME);
        printf("Please verify that you have permission to write to this "
               "location.\n\n");
        break;
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

      for (i = 0; i < 5; i++) {
        fprintf(filePtr, "%.2f\n", grades[i]);
      }

      if (fclose(filePtr) != 0) {
        printf("\nERROR\n");
        printf("A problem occurred while writing %s.\n\n", FILE_NAME);
        break;
      }

      printf("\nStudent record successfully saved.\n");
      printf("%s created.\n\n", FILE_NAME);
      break;
    }

    case 5: {
      printf("\nThank you for using the\n");
      printf("Student Information Management System.\n");
      printf("Program terminated successfully.\n");
      keepRunning = false;
      break;
    }

    default: {
      printf("\nERROR\n");
      printf("Invalid menu selection.\n");
      printf("Please choose an option between 1 and 5.\n\n");
      break;
    }
    }
  }

  return 0;
}
