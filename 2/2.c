#include <stdio.h>

#define MAX_JOURNEYS 100
#define MAX_FIELD_LEN 30 + 1

typedef struct {
  char id[MAX_FIELD_LEN];

  char date[MAX_FIELD_LEN];

  char start[MAX_FIELD_LEN];
  char start_time[MAX_FIELD_LEN];

  char end[MAX_FIELD_LEN];
  char end_time[MAX_FIELD_LEN];

  int delay;
} journey;

// TODO: finirlo al laib
int main(int argc, char const *argv[]) {
  printf("%s\n", "Hello, world!");

  FILE *fp = fopen("corse.txt", "r");
  if (fp == NULL) {
    perror("couldn't open file");
    return -1;
  }

  journey journeys[MAX_JOURNEYS];
  int journeys_len;
  fscanf(fp, "%d", &journeys_len);
  printf("%d\n", journeys_len);

  for (int i = 0; i < journeys_len; ++i) {
    fscanf(fp, "%s %s %s %s %s %s %d", journeys[i].id, journeys[i].start,
           journeys[i].end, journeys[i].date, journeys[i].start_time,
           journeys[i].end_time, &journeys[i].delay);
  }

  for (int i = 0; i < journeys_len; ++i) {
    printf("%s\n", journeys[i].id);
  }

  return 0;
}