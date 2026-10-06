#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 41 // Max 40 chars + 1 for null terminator

// Future-proofing: a full name is built from up to MAX_NAME_PARTS parts
// (first, middle, last, suffix). Only first and last are used today; to add
// middle names or suffixes later, add one more input and one more
// append_name_part() call in build_full_name().
#define MAX_NAME_PARTS 4
#define FULL_NAME_LEN (MAX_NAME_PARTS * (MAX_NAME_LEN - 1) + MAX_NAME_PARTS)

// Function prototypes
int get_member_input(int member_num, char *first_name, char *last_name);
int get_valid_name(const char *prompt, char *name_out, int allow_blank);
void clear_input_buffer(void);
void trim_whitespace(char *str);
int validate_name_chars(const char *str);
void append_name_part(char *full_name, const char *part);
void build_full_name(const char *first, const char *last, char *full_name);
int compare_members(const char *first1, const char *last1, const char *first2,
                    const char *last2);
void display_report(const char *full1, int len1, const char *full2, int len2,
                    int is_identical, int sort_order);

int main(void) {
  char first1[MAX_NAME_LEN], last1[MAX_NAME_LEN], full1[FULL_NAME_LEN];
  char first2[MAX_NAME_LEN], last2[MAX_NAME_LEN], full2[FULL_NAME_LEN];

  // Get input for both members; stop cleanly if input ends early
  if (!get_member_input(1, first1, last1) ||
      !get_member_input(2, first2, last2)) {
    printf("\nInput ended unexpectedly. No report produced.\n");
    return 1;
  }

  build_full_name(first1, last1, full1);
  build_full_name(first2, last2, full2);

  // Length counts every character, including spaces, hyphens and apostrophes
  int len1 = (int)strlen(full1);
  int len2 = (int)strlen(full2);

  // Case-sensitive: "alice" and "Alice" are different names
  int is_identical = (strcmp(full1, full2) == 0);

  // Case-sensitive order: last name first, first name breaks a tie
  int sort_order = compare_members(first1, last1, first2, last2);

  display_report(full1, len1, full2, len2, is_identical, sort_order);

  return 0;
}

/**
 * Discards remaining characters on the current input line.
 */
void clear_input_buffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    // Discard extra input
  }
}

/**
 * Trims leading and trailing whitespace in place.
 */
void trim_whitespace(char *str) {
  int start = 0;
  int end = (int)strlen(str) - 1;

  while (str[start] != '\0' && isspace((unsigned char)str[start])) {
    start++;
  }
  while (end >= start && isspace((unsigned char)str[end])) {
    end--;
  }

  int i;
  for (i = 0; start <= end; i++, start++) {
    str[i] = str[start];
  }
  str[i] = '\0';
}

/**
 * Allowed characters: letters, hyphens (-) and apostrophes ('). Returns 1 if
 * valid, 0 otherwise.
 */
int validate_name_chars(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    char c = str[i];
    if (!isalpha((unsigned char)c) && c != '-' && c != '\'') {
      return 0;
    }
  }
  return 1;
}

/**
 * Prompts until a valid name (max 40 characters) is entered. Leading and
 * trailing spaces are ignored. A blank entry is accepted only if allow_blank
 * is nonzero. Returns 1 on success, 0 if input ended (EOF) first.
 */
int get_valid_name(const char *prompt, char *name_out, int allow_blank) {
  while (1) {
    int too_long = 0;

    printf("%s", prompt);
    if (fgets(name_out, MAX_NAME_LEN, stdin) == NULL) {
      return 0; // EOF or read error
    }

    // No newline in the buffer: exactly 40 characters, or more than 40
    if (strchr(name_out, '\n') == NULL) {
      int c = getchar();
      if (c != '\n' && c != EOF) {
        clear_input_buffer();
        too_long = 1;
      }
    }

    if (too_long) {
      printf("Error: Name cannot exceed 40 characters. Please try again.\n");
      continue;
    }

    name_out[strcspn(name_out, "\r\n")] = '\0';
    trim_whitespace(name_out);

    if (strlen(name_out) == 0) {
      if (allow_blank) {
        return 1;
      }
      printf("Error: Name cannot be blank. Please try again.\n");
    } else if (!validate_name_chars(name_out)) {
      printf("Error: Name can only contain letters, hyphens (-), and "
             "apostrophes (').\n");
    } else {
      return 1;
    }
  }
}

/**
 * Prompts the librarian for one member. Last name is optional.
 * Returns 1 on success, 0 on EOF.
 */
int get_member_input(int member_num, char *first_name, char *last_name) {
  printf("Member %d\n", member_num);
  if (!get_valid_name("First Name: ", first_name, 0)) {
    return 0;
  }
  if (!get_valid_name("Last Name (optional): ", last_name, 1)) {
    return 0;
  }
  printf("\n");
  return 1;
}

/**
 * Appends one name part, adding a separating space only if the full name
 * already has text. Empty parts are skipped.
 */
void append_name_part(char *full_name, const char *part) {
  if (strlen(part) == 0) {
    return;
  }
  if (strlen(full_name) > 0) {
    strcat(full_name, " ");
  }
  strcat(full_name, part);
}

/**
 * Builds "First Last" (or just "First" when there is no last name).
 * Future: add middle name and suffix with extra append_name_part() calls.
 */
void build_full_name(const char *first, const char *last, char *full_name) {
  full_name[0] = '\0';
  append_name_part(full_name, first);
  append_name_part(full_name, last);
}

/**
 * Case-sensitive alphabetical comparison.
 * Primary key: last name. Tie-breaker: first name.
 * Returns <0 if member 1 comes first, 0 if equal, >0 if member 2 comes first.
 */
int compare_members(const char *first1, const char *last1, const char *first2,
                    const char *last2) {
  int result = strcmp(last1, last2);
  if (result != 0) {
    return result;
  }
  return strcmp(first1, first2);
}

/**
 * Prints the report.
 */
void display_report(const char *full1, int len1, const char *full2, int len2,
                    int is_identical, int sort_order) {
  printf("----------------------------------------\n");
  printf("Riverside Public Library\n");
  printf("Member Name Report\n");
  printf("----------------------------------------\n");
  printf("Member 1 : %s\n", full1);
  printf("Letters : %d\n", len1);
  printf("Member 2 : %s\n", full2);
  printf("Letters : %d\n", len2);

  if (is_identical) {
    printf("The names ARE identical.\n");
  } else {
    printf("The names are NOT identical.\n");
  }

  printf("Alphabetical Order\n");

  if (sort_order <= 0) {
    printf("1. %s\n", full1);
    printf("2. %s\n", full2);
  } else {
    printf("1. %s\n", full2);
    printf("2. %s\n", full1);
  }
}
