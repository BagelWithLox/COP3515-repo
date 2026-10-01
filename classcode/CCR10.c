#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 41  // Max 40 chars + 1 for null terminator
#define FULL_NAME_LEN 85 // Combined length for First + Space + Last + '\0'

// Function prototypes
void get_member_input(int member_num, char *first_name, char *last_name);
void get_valid_name(const char *prompt, char *name_out, int allow_blank);
void trim_whitespace(char *str);
int validate_name_chars(const char *str);
void clear_input_buffer(void);
void combine_names(const char *first, const char *last, char *full_name);
int compare_members_alphabetical(const char *first1, const char *last1,
                                 const char *first2, const char *last2);
void display_report(const char *full1, int len1, const char *full2, int len2,
                    int is_identical, int sort_order);

int main(void) {
  // Arrays of characters (C strings)
  char first1[MAX_NAME_LEN], last1[MAX_NAME_LEN], full1[FULL_NAME_LEN];
  char first2[MAX_NAME_LEN], last2[MAX_NAME_LEN], full2[FULL_NAME_LEN];

  // Get inputs for Member 1 and Member 2
  get_member_input(1, first1, last1);
  get_member_input(2, first2, last2);

  // Combine names
  combine_names(first1, last1, full1);
  combine_names(first2, last2, full2);

  // Character counts (including spaces, hyphens, and apostrophes)
  int len1 = (int)strlen(full1);
  int len2 = (int)strlen(full2);

  // Case-sensitive comparison for exact match
  int is_identical = (strcmp(full1, full2) == 0);

  // Case-sensitive comparison for alphabetical sorting (Last name tie-broken by
  // First name)
  int sort_order = compare_members_alphabetical(first1, last1, first2, last2);

  // Output formatted report
  display_report(full1, len1, full2, len2, is_identical, sort_order);

  return 0;
}

/**
 * Flushes extra characters from stdin up to the newline.
 */
void clear_input_buffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    // Discard extra input
  }
}

/**
 * Trims leading and trailing whitespace characters in-place.
 */
void trim_whitespace(char *str) {
  int start = 0;
  int end = (int)strlen(str) - 1;

  // Find index of first non-space character
  while (str[start] != '\0' && isspace((unsigned char)str[start])) {
    start++;
  }

  // Find index of last non-space character
  while (end >= start && isspace((unsigned char)str[end])) {
    end--;
  }

  // Shift trimmed characters to the start of string
  int i;
  for (i = 0; start <= end; i++, start++) {
    str[i] = str[start];
  }
  str[i] = '\0';
}

/**
 * Validates allowed characters: letters, spaces, hyphens (-), and apostrophes
 * ('). Rejects digits (0-9) and special symbols.
 */
int validate_name_chars(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    char c = str[i];
    if (!isalpha((unsigned char)c) && !isspace((unsigned char)c) && c != '-' &&
        c != '\'') {
      return 0;
    }
  }
  return 1;
}

/**
 * Prompts user for a name string (max 40 chars) and enforces validation rules.
 */
void get_valid_name(const char *prompt, char *name_out, int allow_blank) {
  int valid = 0;

  while (!valid) {
    printf("%s", prompt);
    if (fgets(name_out, MAX_NAME_LEN, stdin) != NULL) {
      // If newline is missing, user entered more than 40 characters
      if (strchr(name_out, '\n') == NULL) {
        clear_input_buffer();
      } else {
        name_out[strcspn(name_out, "\r\n")] = '\0';
      }

      // Strip leading and trailing whitespace
      trim_whitespace(name_out);

      if (strlen(name_out) == 0) {
        if (allow_blank) {
          valid = 1; // Last name is optional
        } else {
          printf("Error: First name cannot be blank. Please try again.\n");
        }
      } else if (!validate_name_chars(name_out)) {
        printf("Error: Name can only contain letters, spaces, hyphens (-), and "
               "apostrophes (').\n");
      } else {
        valid = 1;
      }
    }
  }
}

/**
 * Prompts librarian for member inputs.
 */
void get_member_input(int member_num, char *first_name, char *last_name) {
  printf("Member %d\n", member_num);
  get_valid_name("First Name: ", first_name, 0);          // Mandatory
  get_valid_name("Last Name (optional): ", last_name, 1); // Optional
  printf("\n");
}

/**
 * Combines trimmed First and Last names into a single Full Name.
 */
void combine_names(const char *first, const char *last, char *full_name) {
  strcpy(full_name, first);
  if (strlen(last) > 0) {
    strcat(full_name, " ");
    strcat(full_name, last);
  }
}

/**
 * Performs case-sensitive alphabetical comparison:
 * Primary: Last Name
 * Secondary: First Name (tie-breaker)
 */
int compare_members_alphabetical(const char *first1, const char *last1,
                                 const char *first2, const char *last2) {
  int last_cmp = strcmp(last1, last2);
  if (last_cmp != 0) {
    return last_cmp;
  }
  return strcmp(first1, first2);
}

/**
 * Displays the member report matching exact acceptance criteria formatting.
 */
void display_report(const char *full1, int len1, const char *full2, int len2,
                    int is_identical, int sort_order) {
  printf("----------------------------------------\n");
  printf("Riverside Public Library\n");
  printf("Member Name Report\n");
  printf("----------------------------------------\n");
  printf("Member 1 : %s\n", full1);
  printf("Letters  : %d\n", len1);
  printf("Member 2 : %s\n", full2);
  printf("Letters  : %d\n", len2);

  if (is_identical) {
    printf("The names ARE identical.\n");
  } else {
    printf("The names are NOT identical.\n");
  }

  printf("Alphabetical Order\n\n");

  if (sort_order <= 0) {
    printf("1. %s\n", full1);
    printf("2. %s\n", full2);
  } else {
    printf("1. %s\n", full2);
    printf("2. %s\n", full1);
  }
}
