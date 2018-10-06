#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN (200 + 1)

typedef struct {
  char from[LINE_LEN];
  int to;
} substitution;

// find_in_line
// Finds the `needle` string in the `haystack` string, starting from the
// `start_from` char. Return 1 if found and sets position otherwise returns 0.
// PS: I could have used `strstr`...
int find_in_line(char *haystack, char *needle, int *position, int start_from) {
  for (int i = start_from; i < strlen(haystack) - strlen(needle); i++) {
    for (int j = 0; j < strlen(needle); ++j) {
      if (haystack[i + j] != needle[j]) {
        break;
      }
      if (j == (strlen(needle) - 1)) {
        *position = i;
        return 1;
      }
    }
  }
  return 0;
}

void replace(char *line, char *from, int to) {
  int position;
  int start_from = 0;

  while (1) {
    int found = find_in_line(line, from, &position, start_from);
    if (!found) {
      break;
    }

    char new_line[LINE_LEN] = {0};

    // copy into new_line, line: starting from 0 to the first occurrene
    strncpy(new_line, line, position);

    // strlen($ + ceil(log10(INT_MAX)) + $) = 12
    char to_str[12] = {0};
    sprintf(to_str, "$%d$", to);

    // append into new_line, to_str
    strcat(new_line, to_str);

    // append into new_line, line, starting from the end of the current match
    strcat(new_line, line+position+strlen(from));

    // copy the new_line into the line to continue looping for the substitutions
    strcpy(line, new_line);

    // Start searching from the the end of the last match
    start_from = position + strlen(from);
  }

}

int main() {
  FILE *dic_fp = fopen("dizionario.txt", "r");
  FILE *source_fp = fopen("sorgente.txt", "r");

  if (dic_fp == NULL || source_fp == NULL) {
    perror("Couldn't open both files\n");
    return -1;
  }

  int substitution_len;
  fscanf(dic_fp, "%d ", &substitution_len);

  substitution substitutions[30];

  for (int i = 0; i < substitution_len; ++i) {
    fscanf(dic_fp, "$%d$ %s ", &substitutions[i].to, substitutions[i].from);
  }

  char line[LINE_LEN] = {0};
  while (fgets(line, LINE_LEN, source_fp) != NULL) {
    for (int i = 0; i < substitution_len; ++i) {
      replace(line, substitutions[i].from, substitutions[i].to);
    }
    printf("%s", line);
  }

  fclose(source_fp);
  fclose(dic_fp);

  return 0;
}