/* =====================================================================
   Precision Payroll Services - Line Numbering File Copy Utility
   CCR-004 (Beginner / Lesson 1-5 version)

   Copies every non-blank line from a user-specified text file into
   a fixed-name output file, adding a line number to the front of
   each line. Blank lines are skipped. Prints a summary at the end.

   Only uses: file I/O, character processing, casting, basic I/O,
   selection statements, loops, and arrays. No user-defined
   functions, structs, or pointers beyond basic array use.
   ===================================================================== */

#include <stdio.h>

#define OUTPUT_FILE_NAME "numbered_output.txt"
#define MAX_LINE_LENGTH                                                        \
  1000 /* Lines longer than this will be truncated.                            \
           Payroll rows are short, so this is more                             \
           than enough room in practice. */

int main(void) {
  char inputFileName[100];
  char line[MAX_LINE_LENGTH];

  FILE *inputFile;
  FILE *outputFile;
  FILE *testFile;

  int c;             /* current character read from the input file */
  int charsInLine;   /* number of characters currently stored in "line" */
  int i;             /* loop counter */
  int lineNumber;    /* next line number to assign */
  int errorHappened; /* becomes 1 if a read/write problem occurs mid-copy */

  long totalLines; /* count of lines actually copied */
  long totalChars; /* count of characters actually copied (no newlines) */

  /* ---------- Step 1: Get the input file name ---------- */
  printf("Enter the input file name: ");
  scanf("%99s", inputFileName);

  /* ---------- Step 2: Open the input file ---------- */
  inputFile = fopen(inputFileName, "r");

  if (inputFile == NULL) {
    printf("----------------------------------------\n");
    printf("Precision Payroll Services\n");
    printf("File Processing Summary\n");
    printf("----------------------------------------\n");
    printf("Error\n");
    printf("Unable to open file \"%s\".\n", inputFileName);
    printf("Please verify that the file exists and try again.\n");
    return 0;
  }

  /* ---------- Step 3: Do not overwrite an existing output file ---------- */
  testFile = fopen(OUTPUT_FILE_NAME, "r");
  if (testFile != NULL) {
    fclose(testFile);
    fclose(inputFile);
    printf("----------------------------------------\n");
    printf("Precision Payroll Services\n");
    printf("File Processing Summary\n");
    printf("----------------------------------------\n");
    printf("Error\n");
    printf("Output file \"%s\" already exists.\n", OUTPUT_FILE_NAME);
    printf("Please remove or rename it and try again.\n");
    return 0;
  }

  outputFile = fopen(OUTPUT_FILE_NAME, "w");

  /* ---------- Step 4: Copy the file, one character at a time ---------- */
  totalLines = 0;
  totalChars = 0;
  lineNumber = 0;
  charsInLine = 0;
  errorHappened = 0;

  while ((c = fgetc(inputFile)) != EOF && !errorHappened) {

    if ((char)c == '\n') {
      /* End of a line: only write it out if it had content
         (this is how blank lines get skipped). */
      if (charsInLine > 0) {
        lineNumber++;

        if (fprintf(outputFile, "%d.", lineNumber) < 0) {
          errorHappened = 1;
        }

        for (i = 0; i < charsInLine && !errorHappened; i++) {
          if (fputc(line[i], outputFile) == EOF) {
            errorHappened = 1;
          } else {
            totalChars++;
          }
        }

        if (!errorHappened) {
          fputc('\n', outputFile);
          totalLines++;
        }
      }
      charsInLine = 0;
    } else {
      /* Store the character in the line array (if there is room). */
      if (charsInLine < MAX_LINE_LENGTH - 1) {
        line[charsInLine] = (char)c;
        charsInLine++;
      }
    }
  }

  /* Edge case: the last line has content but the file does not end
     with a newline character. Write that final line out too. */
  if (!errorHappened && charsInLine > 0) {
    lineNumber++;

    if (fprintf(outputFile, "%d.", lineNumber) < 0) {
      errorHappened = 1;
    }

    for (i = 0; i < charsInLine && !errorHappened; i++) {
      if (fputc(line[i], outputFile) == EOF) {
        errorHappened = 1;
      } else {
        totalChars++;
      }
    }

    if (!errorHappened) {
      fputc('\n', outputFile);
      totalLines++;
    }
  }

  fclose(inputFile);
  fclose(outputFile);

  /* ---------- Step 5: Display the summary ---------- */
  printf("----------------------------------------\n");
  printf("Precision Payroll Services\n");
  printf("File Processing Summary\n");
  printf("----------------------------------------\n");

  if (errorHappened) {
    printf("Error\n");
    printf("A problem occurred while copying the file.\n");
    printf("Processing was stopped before the file could be completed.\n");
    printf("\n");
    printf("Partial results before the error:\n");
  }

  printf("Input File         : %s\n", inputFileName);
  printf("Output File         : %s\n", OUTPUT_FILE_NAME);
  printf("Lines Copied        : %ld\n", totalLines);
  printf("Characters Copied   : %ld\n", totalChars);

  if (!errorHappened) {
    printf("File copied successfully.\n");
  }

  return 0;
}
