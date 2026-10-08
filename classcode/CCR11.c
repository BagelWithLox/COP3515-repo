/*
 * CCR11.c  -  CCR-011  National Technology Conference  (version 3)
 *
 * Usage:  CCR11.exe Alice "Brian Smith" Emily
 *
 * Reads presenter names from the command line, then prints:
 *   - every presenter (numbered, aligned)
 *   - the total number of presenters
 *   - the longest name(s) and their character count
 *
 * How names are typed:
 *   - Without quotes, each word is one presenter with a first name only.
 *         CCR11.exe Alice Brian        ->  Alice, Brian
 *   - With quotes, the words inside are one presenter with a first and
 *     last name, shown exactly as typed.
 *         CCR11.exe "Alice Johnson"    ->  Alice Johnson
 *
 * Rules agreed with the customer:
 *   - A name is a first name, or a first and last name (at most 2 words).
 *   - Blanks before/after an argument are trimmed. The text in between is
 *     shown exactly as typed.
 *   - Spaces are NOT counted when finding the longest name.
 *   - Names that contain a title (Dr, Dr., Professor, Mr, ...) are not
 *     accepted.
 *   - Duplicate names are cut (case-insensitive, first spelling is kept).
 *   - At most 10 presenters. An 11th different presenter is an error.
 *   - No usable names -> error message, program ends.
 *   - Ties for longest name: all tied names are shown.
 *   - A name (including its space) may be at most 40 characters.
 *
 * Not in this version (planned for a later version): displaying extra
 * presenter information. Presenters are kept in one table (presenters[])
 * so extra columns can be added beside it later.
 */

#include <stdio.h>
#include <string.h>

#define LINE "----------------------------------------"
#define MAX_PRESENTERS 10
#define MAX_NAME 40
#define MAX_WORDS 2

/* Titles that are not accepted in a presenter name (lower case) */
const char *TITLES[] = { "dr", "doctor", "prof", "professor",
                         "mr", "mrs", "ms", "miss", "mx", "sir", "madam" };
#define TITLE_COUNT 11

/* Returns 1 if c is a blank character (space, tab, CR or LF). */
int isBlank(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

/* Returns the lower-case version of a letter; other characters unchanged. */
char toLower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return (char)(c - 'A' + 'a');
    return c;
}

/* Finds the trimmed part of s. Sets *start to the first non-blank
 * character and *len to the length with trailing blanks removed. */
void trim(const char *s, const char **start, int *len)
{
    int n;

    while (isBlank(*s))
        s++;

    n = (int)strlen(s);
    while (n > 0 && isBlank(s[n - 1]))
        n--;

    *start = s;
    *len = n;
}

/* Returns 1 if the first alen characters of a equal b, ignoring case. */
int sameText(const char *a, int alen, const char *b)
{
    int i;

    if ((int)strlen(b) != alen)
        return 0;
    for (i = 0; i < alen; i++)
    {
        if (toLower(a[i]) != toLower(b[i]))
            return 0;
    }
    return 1;
}

/* Returns the number of words (groups of non-blank characters) in s. */
int countWords(const char *s, int len)
{
    int i;
    int words = 0;
    int inWord = 0;

    for (i = 0; i < len; i++)
    {
        if (isBlank(s[i]))
            inWord = 0;
        else if (!inWord)
        {
            inWord = 1;
            words++;
        }
    }
    return words;
}

/* Returns the number of characters in s that are not blanks.
 * This is the length used to find the longest name. */
int countChars(const char *s)
{
    int n = 0;

    while (*s != '\0')
    {
        if (!isBlank(*s))
            n++;
        s++;
    }
    return n;
}

/* Returns 1 if the word is a title, such as "Dr", "Dr." or "Dr.Smith".
 * Only the text before the first period is compared with the title list,
 * so real names like "Drew" are still accepted. */
int isTitle(const char *s, int len)
{
    int p = 0;
    int t;

    while (p < len && s[p] != '.')
        p++;

    for (t = 0; t < TITLE_COUNT; t++)
    {
        if (sameText(s, p, TITLES[t]))
            return 1;
    }
    return 0;
}

/* Returns 1 if any word in the name is a title. */
int hasTitle(const char *s, int len)
{
    int i = 0;
    int start;

    while (i < len)
    {
        while (i < len && isBlank(s[i]))
            i++;
        start = i;
        while (i < len && !isBlank(s[i]))
            i++;
        if (i > start && isTitle(s + start, i - start))
            return 1;
    }
    return 0;
}

/* Returns 1 if the name is already in the presenter table. */
int isDuplicate(char list[][MAX_NAME + 1], int count, const char *s, int len)
{
    int k;

    for (k = 0; k < count; k++)
    {
        if (sameText(s, len, list[k]))
            return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char presenters[MAX_PRESENTERS][MAX_NAME + 1];
    const char *name;
    int len;
    int i;
    int count = 0;
    int rejected = 0;
    int maxLen = 0;
    int width = 1;
    int printedLongest = 0;

    if (argc < 2)
    {
        printf("Error: no presenter names were supplied.\n");
        printf("Usage: CCR11.exe Name1 \"First Last\" ... (maximum %d names)\n",
               MAX_PRESENTERS);
        return 1;
    }

    /* Check every argument and build the presenter table */
    for (i = 1; i < argc; i++)
    {
        trim(argv[i], &name, &len);

        if (len == 0)               /* empty name: skipped */
            continue;

        if (countWords(name, len) > MAX_WORDS)
        {
            printf("Not accepted: \"%s\" - use a first name, or a first and "
                   "last name only.\n", argv[i]);
            rejected++;
            continue;
        }
        if (hasTitle(name, len))
        {
            printf("Not accepted: \"%s\" - titles are not allowed.\n",
                   argv[i]);
            rejected++;
            continue;
        }
        if (len > MAX_NAME)
        {
            printf("Not accepted: \"%s\" - longer than %d characters.\n",
                   argv[i], MAX_NAME);
            rejected++;
            continue;
        }
        if (isDuplicate(presenters, count, name, len))
            continue;               /* duplicate: cut */

        if (count == MAX_PRESENTERS)
        {
            printf("Error: too many presenters. The maximum is %d.\n",
                   MAX_PRESENTERS);
            return 1;
        }

        strncpy(presenters[count], name, (size_t)len);
        presenters[count][len] = '\0';
        count++;
    }

    if (count == 0)
    {
        printf("Error: no valid presenter names were supplied.\n");
        printf("Usage: CCR11.exe Name1 \"First Last\" ... (maximum %d names)\n",
               MAX_PRESENTERS);
        return 1;
    }

    if (rejected > 0)
        printf("\n");

    /* Longest name length (spaces are not counted) */
    for (i = 0; i < count; i++)
    {
        len = countChars(presenters[i]);
        if (len > maxLen)
            maxLen = len;
    }

    /* Width of the largest list number, so 9. and 10. line up */
    if (count >= 10)
        width = 2;

    /* Header */
    printf("%s\n", LINE);
    printf("National Technology Conference\n");
    printf("Presenter Summary\n");
    printf("%s\n", LINE);
    printf("Presenters\n");

    /* List every presenter, as typed */
    for (i = 0; i < count; i++)
        printf("%*d. %s\n", width, i + 1, presenters[i]);

    /* Summary */
    printf("%s\n", LINE);
    printf("Total Presenters : %d\n", count);

    /* Longest name(s): show every presenter tied for the maximum */
    for (i = 0; i < count; i++)
    {
        if (countChars(presenters[i]) != maxLen)
            continue;
        if (!printedLongest)
            printf("Longest Name : %s\n", presenters[i]);
        else
            printf("               %s\n", presenters[i]);
        printedLongest = 1;
    }
    printf("Characters : %d\n", maxLen);

    return 0;
}
