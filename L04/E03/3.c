#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SET_MATCHED()                                                          \
  do {                                                                         \
    if (!is_a_match) {                                                         \
      match_index = src_index;                                                 \
      is_a_match = true;                                                       \
    } else {                                                                   \
    }                                                                          \
  } while (0)

#define RESET_MATCHED()                                                                              \
  do {                                                                                               \
    /* If the matching failed we could be in 2 situations:*/                                         \
    /* If there was a match before restart from the character next to the*/ /* last match position*/ \
    if (is_a_match) {                                                                                \
      src_index = match_index + 1;                                                                   \
    } /* If there was no match before simply restart from the next character*/                       \
    else {                                                                                           \
      src_index++;                                                                                   \
    }                                                                                                \
    is_a_match = false;                                                                              \
    regexp_index = 0;                                                                                \
  } while (0)

#define RED(...)                                                               \
  {                                                                            \
    printf("\033[1;31m");                                                      \
    printf(__VA_ARGS__);                                                       \
    printf("\033[0m");                                                         \
  }

#define GREEN(...)                                                             \
  {                                                                            \
    printf("\033[1;32m");                                                      \
    printf(__VA_ARGS__);                                                       \
    printf("\033[0m");                                                         \
  }

// TODO: ASK: We assume that the regexp are correct, otherwise we'll at least
// need to lex them
char *cercaRegexp(char *src, char *regexp) {
  int regexp_index = 0;
  int src_index = 0;
  int match_index = 0;
  bool is_a_match = false;
  bool match_complete = false;

  while (true) {
    char current_regexp = regexp[regexp_index];
    char current_str = src[src_index];

    // If we matched all of the regex's chars...
    if (current_regexp == '\0') {
      match_complete = true;
    }
    if (current_regexp == '\0' || current_str == '\0') {
      break;
    }
    // printf("current_str: '%c'\t current_regexp: '%c'\n", current_str,
    // current_regexp);

    char lookahead = regexp[regexp_index + 1];

    switch (current_regexp) {
    // . matches a single char
    case '.':
      SET_MATCHED();

      regexp_index++;
      src_index++;
      break;
    case '[':
      // [^xyz] matches everything but 'x','y','z'
      if (lookahead == '^') {
        // Count the chars after lookahead + '^'
        int group_len = 0;

        while (regexp[regexp_index + 1 + group_len + 1] != ']') {
          group_len++;
        }

        bool found = false;
        for (int i = 0; i < group_len; ++i) {
          if (regexp[regexp_index + 1 + i + 1] == current_str) {
            found = true;
            break;
          }
        }
        if (!found) {
          SET_MATCHED();

          regexp_index += (1 + 1 + group_len + 1);
          src_index++;
        } else {
          RESET_MATCHED();
        }
      }
      // [xyz] matches only the chars 'x','y','z'
      else {
        // Count the chars after lookahead
        int group_len = 0;

        while (regexp[regexp_index + group_len + 1] != ']') {
          group_len++;
        }

        bool found = false;
        for (int i = 0; i < group_len; ++i) {
          if (regexp[regexp_index + i + 1] == current_str) {
            found = true;
            break;
          }
        }
        if (found) {
          SET_MATCHED();

          regexp_index += (1 + group_len + 1);
          src_index++;
        } else {
          RESET_MATCHED();
        }
      }
      break;
    case '\\':
      // \a matches a lowercase char
      if (lookahead == 'a') {
        if (islower(current_str)) {
          SET_MATCHED();

          regexp_index += 2;
          src_index++;
        } else {
          RESET_MATCHED();
        }
      }
      // \A matches an uppercase char
      else if (lookahead == 'A') {
        if (isupper(current_str)) {
          SET_MATCHED();

          regexp_index += 2;
          src_index++;
        } else {
          RESET_MATCHED();
        }
      } else {
        fprintf(stderr, "Unknown regexp char %c\n", current_regexp);
        exit(1);
      }
      break;
    default:
      // A normal alpabetic character which matches that char
      if (isalpha(current_regexp)) {
        // If we matched update the matched_index and flag with SET_MATCHED()
        // and go to next char of the regex and string
        if (current_regexp == current_str) {
          SET_MATCHED();

          regexp_index++;
          src_index++;
        } else {
          RESET_MATCHED();
        }
      } else {
        fprintf(stderr, "Unknown regexp char: '%c'\n", current_regexp);
        exit(1);
      }
      break;
    }
  }
  if (is_a_match && match_complete) {
    return &src[match_index];
  } else {
    return NULL;
  }
}

void test_should_pass(char *a, char *b) {
  char *match = cercaRegexp(a, b);
  if (match != NULL) {
  } else {
    printf("\nmatching '%s' with '%s'\n", a, b);
    RED("*************> No matches found\n");
  }
}
void test_should_fail(char *a, char *b) {
  char *match = cercaRegexp(a, b);
  if (match != NULL) {
    printf("\nmatching '%s' with '%s'\n", a, b);
    RED("=============> %s\n", match);
  } else {
  }
}

int main() {
  test_should_pass("pirandello", "pirandello");

  test_should_pass("voto", ".oto");

  test_should_fail("nate", ".oto");

  test_should_fail("nato", "c.to");
  test_should_pass("cito", "c..o");
  test_should_pass("cena", "c...");

  test_should_fail("daw", "c...");

  test_should_fail("dawdaw", "c...");

  test_should_pass("si", "..");
  test_should_fail("si", "...");
  test_should_pass("siccardi", "car.i");
  test_should_pass("siccardi", "ca..i");

  test_should_pass("s", "s");
  test_should_pass("s", ".");

  test_should_pass("t", "\\a");
  test_should_fail("tu", "\\at");
  test_should_pass("tu", "\\au");

  test_should_pass("T", "\\A");
  test_should_fail("Tu", "\\At");
  test_should_pass("Tu", "\\Au");

  test_should_pass("casa", "[^aeiou]...");
  test_should_fail("casa", "[^caeiou]...");
  test_should_pass("casatt", "[^caeiou]...");

  test_should_pass("moto", "[mn]oto");
  test_should_pass("noto", "[mn]oto");
  test_should_fail("poto", "[mn]oto");

  test_should_pass("AcenRid", "A[^f]\\anR.d");
  test_should_pass("Aezt3", "\\A[aeiou]zt[123]");

  return 0;
}