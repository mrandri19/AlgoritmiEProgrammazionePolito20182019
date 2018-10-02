#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int has_two_vowels(char *word) {
  int len = strlen(word);
  int vowels = 0;
  for (int i = 0; i < len; ++i) {

    char c = word[i];
    // evaluates to 1 (true) if c is a lowercase vowel
    int isLowercaseVowel =
        (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');

    // evaluates to 1 (true) if c is an uppercase vowel
    int isUppercaseVowel =
        (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');

    if (isLowercaseVowel || isUppercaseVowel) {
      vowels++;
    }
    if (vowels == 2) {
      return 1;
    }
  }
  return 0;
}

int conta(char *word, int substring_len) {
  int occurrences = 0;
  int word_len = strlen(word);

  printf("Word len: %d\n", word_len);
  for (int i = 0; i <= (word_len - substring_len); ++i) {
    char tmp_word[20] = {0};
    int k = 0;
    for (int j = i; j < (substring_len + i); ++j) {
      tmp_word[k++] = word[j];
    }
    if (has_two_vowels(tmp_word)) {
      occurrences++;
    }
  }

  return occurrences;
}

int main() {
  FILE *fp;
  if ((fp = fopen("input-2.txt", "r")) == NULL) {
    perror("Couldn't open file");
    return -1;
  }

  int n;
  printf("Insert substring length\n");
  scanf("%d", &n);

  int words_len;
  fscanf(fp, "%d", &words_len);

  for (int i = 0; i < words_len; ++i) {
    char word[20 + 1];
    fscanf(fp, "%s", word);

    printf("%d\n", conta(word, n));
  }

  fclose(fp);
  return 0;
}