#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "quotations.h"
#include "title.h"

int main(int argc, char **argv) {
  FILE *fp = fopen("input.txt", "r");
  if (fp == NULL) {
    perror("cannot open input.txt\n");
    exit(EXIT_FAILURE);
  }

  size_t titles_len;
  fscanf(fp, "%zu ", &titles_len);

  title_list_t tl = title_list_new();

  for (size_t i = 0; i < titles_len; ++i) {
    title_t t = title_load(fp);
    title_list_insert(tl, t);
  }

  if (argc < 2) {
    printf("Not enough arguments\n");
    return 1;
  }

  switch (argv[1][0]) {
  // - search a title
  case 'a': {
    title_t t;
    if (!title_list_search(tl, argv[2], &t)) {
      printf("No title with that name found\n");
      break;
    } else {
      title_print(t);
    }
    break;
  }

  // - search a title's quotation for a certain date
  case 'b': {
    title_t t;
    if (!title_list_search(tl, argv[2], &t)) {
      printf("No title with that name found\n");
      break;
    }
    date_t d = date_load_from_string(argv[3]);

    quotation_t q;
    if (!title_get_quotation_from_date(t, d, &q)) {
      printf("No quotation found with that date\n");
    } else {
      quotation_print(q);
      printf("\n");
    }
    break;
  }

  // - search a title's min and max quotation in a given date interval
  case 'c': {
    title_t t;
    if (!title_list_search(tl, argv[2], &t)) {
      printf("No title with that name found\n");
      break;
    }
    date_t start = date_load_from_string(argv[3]);
    date_t finish = date_load_from_string(argv[4]);

    title_print(t);

    quotation_t q;
    q.value = 1000000000;
    if (!title_get_min_quotation_from_date_interval(t, start, finish, &q)) {
      printf("No quotation found with that date\n");
    } else {
      printf("Min quotation: ");
      quotation_print(q);
      printf("\n");
    }

    q.value = -1000000000;
    if (!title_get_max_quotation_from_date_interval(t, start, finish, &q)) {
      printf("No quotation found with that date\n");
    } else {
      printf("Max quotation: ");
      quotation_print(q);
      printf("\n");
    }
    break;
  }

  // - search a title's min and max quotation overall
  case 'd': {
    title_t t;
    if (!title_list_search(tl, argv[2], &t)) {
      printf("No title with that name found\n");
      break;
    }
    title_print(t);

    quotation_t q;
    q.value = 1000000000;
    if (!title_get_min_quotation(t, &q)) {
      printf("No quotation found with that date\n");
    } else {
      printf("Min quotation: ");
      quotation_print(q);
      printf("\n");
    }

    q.value = -1000000000;
    if (!title_get_max_quotation(t, &q)) {
      printf("No quotation found with that date\n");
    } else {
      printf("Max quotation: ");
      quotation_print(q);
      printf("\n");
    }
    break;
  }

  // - given a title, rebalance quotation tree if the ratio between longest and
  //   shortest path is greater that a thresold S
  // (Rebalancing via partitioning around median key)
  case 'e': {
    title_t t;
    if (!title_list_search(tl, argv[2], &t)) {
      printf("No title with that name found\n");
      break;
    }
    title_print(t);

    int thresold = atoi(argv[3]);
    printf("Thresold: %d\n", thresold);

    int longest_path = title_get_longest_path(t);
    printf("longest_path: %d\n", longest_path);

    int shortest_path = title_get_shortest_path(t);
    printf("shortest_path: %d\n", shortest_path);

    float ratio = (float)longest_path / shortest_path;

    if (ratio > thresold) {
      printf("must rebalance, ratio=%.2f > threshold=%d\n", ratio, thresold);
      title_rebalance_quotation_tree(t);

      printf("The rebalanced tree:\n");
      title_print(t);

      longest_path = title_get_longest_path(t);
      printf("longest_path: %d\n", longest_path);

      shortest_path = title_get_shortest_path(t);
      printf("shortest_path: %d\n", shortest_path);
    }

    break;
  }
  }

  title_list_free(tl);

  fclose(fp);
  return 0;
}
