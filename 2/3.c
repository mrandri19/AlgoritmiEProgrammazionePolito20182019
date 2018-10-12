#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *seq_fp, *text_fp;
  seq_fp = fopen("sequenze.txt", "r");
  text_fp = fopen("testo.txt", "r");
  if (seq_fp == NULL || text_fp == NULL) {
    perror("cannot open files\n");
    return -1;
  }

  int sequences_len;
  fscanf(seq_fp, "%d", &sequences_len);

  char sequences[20][5 + 1];

  for (int i = 0; i < sequences_len; ++i) {
    fscanf(seq_fp, "%s", sequences[i]);
    for (int j = 0; j < strlen(sequences[i]); ++j) {
      sequences[i][j] = tolower(sequences[i][j]);
    }
  }

  char text_line[200 + 1];
  while (fgets(text_line, 200 + 1, text_fp) != NULL) {
    // for (int i = 0; i < sequences_len; ++i) {}

    // TODO: finish
    // TODO: implement own strtok
    char parola[25 + 1];

    printf("- ");
    for (int i = 0; i < strlen(text_line); ++i) {
      if (isalnum(text_line[i])) {
        printf("%c", text_line[i]);
      }
    }
    puts("");

    // per ogni sequenza
    //    controlla ogni parola (max 25char) (parola == isalnum), separata da
    //    spazio (spazio
    //    == isspace o ispunct)
    //    salvati al max 10 occorrenze e la relativa posizione
  }

  return 0;
}