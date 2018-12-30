#ifndef TITLE_H
#define TITLE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "date.h"
#include "quotations.h"

/** 1st class ADT for title and title list */
//  each title has a BST of daily quotations
// Quasi ADT for daily quotations and 1st class ADT for collection of
// daily quotations (implemented as BST)

typedef struct title {
  char name[20 + 1];
  BST_t quotations;
} * title_t;

typedef struct {
  date_t date;
  int hour;
  int minute;
  float value;
  int quantity;
} transaction_t;

title_t title_load(FILE *fp) {
  // Allocate the required structures
  title_t t = (title_t)malloc(sizeof(struct title));
  t->quotations = BST_new();

  // Read the transactions length
  size_t transactions_len;
  fscanf(fp, "%s %zu", t->name, &transactions_len);

  // Allocate a temporary array to calculate the daily quotations for the
  // transactions
  transaction_t *transactions = calloc(transactions_len, sizeof(transaction_t));

  // Read all transactions
  for (size_t i = 0; i < transactions_len; ++i) {
    date_t date = date_load(fp);
    transactions[i].date = date;
    fscanf(fp, "%d:%d %f %d ", &transactions[i].hour, &transactions[i].minute,
           &transactions[i].value, &transactions[i].quantity);
  }

  // Calculate the weighted average of value * quantity / quantity over a day to
  // get the quotation. Then insert the quotation into the BST

  // Marker array to avoid inserting twice the same date
  bool *already_processed = calloc(transactions_len, sizeof(bool));

  for (size_t i = 0; i < transactions_len; ++i) {
    already_processed[i] = false;
  }

  for (size_t i = 0; i < transactions_len; ++i) {
    date_t date = transactions[i].date;
    if (already_processed[i]) {
      continue;
    }

    float total_volume = 0;
    float total_quantity = 0;
    for (size_t j = 0; j < transactions_len; ++j) {
      if (date_equal(transactions[j].date, date)) {
        total_volume += (transactions[j].value * transactions[j].quantity);
        total_quantity += transactions[j].quantity;
        already_processed[j] = true;
      }
    }

    quotation_t q;
    q.date = date;
    q.value = (total_volume / total_quantity);

    BST_insert(&(t->quotations), q);
  }

  free(already_processed);
  free(transactions);

  // Return the newly created title
  return t;
}

void title_print(title_t t) {
  printf("%s\n", t->name);
  BST_print(t->quotations, 0);
}

void title_free(title_t t) {
  BST_free(t->quotations);
  free(t);
}

bool title_get_quotation_from_date(title_t t, date_t d, quotation_t *q) {
  return bst_get_quotation_from_date(t->quotations, d, q);
}

bool title_get_min_quotation_from_date_interval(title_t t, date_t start,
                                                date_t finish, quotation_t *q) {
  return bst_get_min_quotation_from_date_interval(t->quotations, start, finish,
                                                  q);
}

bool title_get_max_quotation_from_date_interval(title_t t, date_t start,
                                                date_t finish, quotation_t *q) {
  return bst_get_max_quotation_from_date_interval(t->quotations, start, finish,
                                                  q);
}

bool title_get_min_quotation(title_t t, quotation_t *q) {
  return bst_get_min_quotation(t->quotations, q);
}
bool title_get_max_quotation(title_t t, quotation_t *q) {
  return bst_get_max_quotation(t->quotations, q);
}

int title_compare(title_t t1, title_t t2) { return strcmp(t1->name, t2->name); }

int title_get_longest_path(title_t t) {
  return bst_get_longest_path(t->quotations);
}

int title_get_shortest_path(title_t t) {
  return bst_get_shortest_path(t->quotations);
}

void title_rebalance_quotation_tree(title_t t) {
  bst_rebalance(&(t->quotations));
}

// *****************************************************************************
// 1st class ADT for title and title collection (implemented as sorted linked
// list)

typedef struct linked_list {
  title_t head;
  struct linked_list *tail;
} * linked_list_t;

typedef struct title_list {
  linked_list_t list;
} * title_list_t;

title_list_t title_list_new() {
  title_list_t tl = (title_list_t)malloc(sizeof(struct title_list));
  tl->list = NULL;
  return tl;
}

void linked_list_free(linked_list_t l) {
  if (l == NULL)
    return;
  title_free(l->head);
  linked_list_free(l->tail);
  free(l);
}

void title_list_free(title_list_t tl) {
  linked_list_free(tl->list);
  free(tl);
}

// Insert into the linked list keeping it sorted
void linked_list_insert(linked_list_t *l, title_t t) {
  if (*l == NULL) {
    *l = (linked_list_t)malloc(sizeof(struct linked_list));
    (*l)->head = t;
    (*l)->tail = NULL;
  } else {
    if (title_compare((*l)->head, t) < 0) {
      linked_list_insert(&(*l)->tail, t);
    } else {
      linked_list_t new_l = (linked_list_t)malloc(sizeof(struct linked_list));
      new_l->head = t;
      new_l->tail = *l;

      *l = new_l;
    }
  }
}

void title_list_insert(title_list_t tl, title_t t) {
  linked_list_insert(&(tl->list), t);
}

void linked_list_print(linked_list_t l) {
  if (l == NULL) {
    return;
  }
  title_print(l->head);
  linked_list_print(l->tail);
}

void title_list_print(title_list_t tl) { linked_list_print(tl->list); }

bool linked_list_search(linked_list_t l, char *title_name, title_t *result) {
  if (l == NULL) {
    return false;
  }
  if (strcmp(l->head->name, title_name) == 0) {
    *result = l->head;
    return true;
  } else {
    return linked_list_search(l->tail, title_name, result);
  }
}

bool title_list_search(title_list_t tl, char *title_name, title_t *result) {
  return linked_list_search(tl->list, title_name, result);
}

#endif
