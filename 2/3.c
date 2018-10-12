#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 25 + 1
#define LINE_LEN 200 + 1
#define MAX_SEQUENCES 20
#define SEQUENCE_LEN 5 + 1
#define MAX_OCCURRENCES 10

typedef enum { false,
    true } bool;

// This struct will hold each occurence of a sequence in a word
typedef struct
{
    int position;
    char word[WORD_LEN];
} occurence;

int main()
{
    // Open the files
    FILE *seq_fp, *text_fp;
    seq_fp = fopen("sequenze.txt", "r");
    text_fp = fopen("testo.txt", "r");
    if (seq_fp == NULL || text_fp == NULL) {
        perror("cannot open files\n");
        return -1;
    }

    // Read the sequences len
    int sequences_len;
    fscanf(seq_fp, "%d", &sequences_len);

    // Read all of the sequences into an array
    char sequences[MAX_SEQUENCES][SEQUENCE_LEN];
    for (int i = 0; i < sequences_len; ++i) {
        fscanf(seq_fp, "%s", sequences[i]);
        for (int j = 0; j < strlen(sequences[i]); ++j) {
            sequences[i][j] = tolower(sequences[i][j]);
        }
    }

    // This array will hold the word occurrences for each sequence
    occurence occurences[MAX_SEQUENCES][MAX_OCCURRENCES];
    // This array will hold the length of the word occurences for each sequence
    int occurences_len_foreach_seq[MAX_SEQUENCES] = { 0 };

    // The words of the last line, initialized to 0
    int last_line_words = 0;

    // foreach row
    char text_line[LINE_LEN];
    while (fgets(text_line, LINE_LEN, text_fp) != NULL) {
        char word[WORD_LEN] = { 0 };

        int word_start = 0;
        int word_end = 0;
        bool in_word = false;

        // foreach sequence
        for (int k = 0; k < sequences_len; ++k) {
            // Track the current position, in each sequence, starting from
            // the the word count up to the last line
            int position = last_line_words;

            // restore the length of the occurences for each sequence every time
            // the line new. At the first iteration it will return 0 since the
            // array has been zeroed at declaration.
            int occurrence_len_for_this_seq = occurences_len_foreach_seq[k];

            // foreach word
            for (int i = 0; i < strlen(text_line); ++i) {
                // The word splitting routine
                if (isalnum(text_line[i])) {
                    if (in_word) {
                        word_end++;
                    } else {
                        in_word = true;
                        word_start = i;
                        word_end = i + 1;
                    }
                } else {
                    if (in_word) {
                        position++;

                        // Clear the temporary string `word`
                        for (int j = 0; j < WORD_LEN; j++) {
                            word[j] = 0;
                        }

                        // Copy into word a word from the line
                        strncpy(word, text_line + word_start, word_end - word_start);

                        // Use a temporary all-lowercase string for the substring check
                        char lowercase_word[WORD_LEN];
                        strcpy(lowercase_word, word);
                        for (int i = 0; i < strlen(word); i++) {
                            lowercase_word[i] = tolower(word[i]);
                        }

                        // If a match is found copy the match data into a struct
                        char* sub = strstr(lowercase_word, sequences[k]);
                        if (sub != NULL && occurrence_len_for_this_seq < MAX_OCCURRENCES) {
                            // Che oscenita' con un puntatore si evitava che la struct venisse copiata in una
                            // variabile temporanea e quindi avrei potuto evitare di ripetere 53 * 3 caratteri
                            occurences[k][occurrence_len_for_this_seq].position = position;
                            strcpy(occurences[k][occurrence_len_for_this_seq].word, word);
                            occurrence_len_for_this_seq++;
                        }

                        word_start = 0;
                        word_end = 0;
                        in_word = false;
                    } else {
                    }
                }
            }

            // Store the length of the occurences in the occurences length array
            occurences_len_foreach_seq[k] = occurrence_len_for_this_seq;

            // In the last sequence update the count of the last line's words
            if (k == (sequences_len - 1)) {
                last_line_words = position;
            }
        }
    }

    // foreach sequence
    for (int i = 0; i < sequences_len; i++) {
        printf("La sequenza \"%s\" e' contenuta in:\n", sequences[i]);
        // foreach sequence's occurence
        for (int j = 0; j < occurences_len_foreach_seq[i]; j++) {
            occurence occ = occurences[i][j];
            printf("- %d %s\n", occ.position, occ.word);
        }
    }

    return 0;
}