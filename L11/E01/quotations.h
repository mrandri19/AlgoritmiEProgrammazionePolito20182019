#ifndef BST_H
#define BST_H

#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "date.h"

typedef struct {
  date_t date;
  float value;
} quotation_t;

quotation_t quotation_new(date_t date, float value) {
  return (quotation_t){.date = date, .value = value};
}

void quotation_print(quotation_t q) {
  date_print(q.date);
  printf(" %f", q.value);
}

// ****************************************************************************

// BST indexed by the quotation's date
typedef struct BST {
  quotation_t quotation;

  struct BST *left;
  struct BST *right;
} * BST_t;

BST_t BST_new() { return NULL; }

void BST_free(BST_t bst) {
  if (bst == NULL)
    return;
  BST_free(bst->left);
  BST_free(bst->right);
  free(bst);
}

void BST_insert(BST_t *bst, quotation_t q) {
  if (*bst == NULL) {
    (*bst) = malloc(sizeof(struct BST));
    (*bst)->quotation = q;
    (*bst)->left = NULL;
    (*bst)->right = NULL;
  } else {
    if (date_lesser(q.date, (*bst)->quotation.date)) {
      BST_insert(&(*bst)->left, q);
    } else {
      BST_insert(&(*bst)->right, q);
    }
  }
}

void BST_print(BST_t bst, int depth) {
  if (bst == NULL) {
    printf("∅\n");
    return;
  }

  quotation_print(bst->quotation);
  printf("\n");

  if (bst->left != NULL) {

    for (int i = 0; i < depth; ++i) {
      printf("    ");
    }
    printf("└L──");
    BST_print(bst->left, depth + 1);
  }

  if (bst->right != NULL) {
    for (int i = 0; i < depth; ++i) {
      printf("    ");
    }
    printf("└R──");
    BST_print(bst->right, depth + 1);
  }
}

bool bst_get_quotation_from_date(BST_t bst, date_t d, quotation_t *q) {
  if (bst == NULL) {
    return false;
  }
  if (date_equal(bst->quotation.date, d)) {
    *q = bst->quotation;
    return true;
  }
  if (date_lesser(d, bst->quotation.date)) {
    return bst_get_quotation_from_date(bst->left, d, q);
  } else {
    return bst_get_quotation_from_date(bst->right, d, q);
  }
}

bool bst_get_min_quotation_from_date_interval(BST_t bst, date_t start,
                                              date_t finish, quotation_t *q) {
  if (bst == NULL) {
    return false;
  }

  if (date_is_inside_interval(bst->quotation.date, start, finish)) {
    if (bst->quotation.value < q->value) {
      *q = bst->quotation;
    }
  }

  bst_get_min_quotation_from_date_interval(bst->left, start, finish, q);
  bst_get_min_quotation_from_date_interval(bst->right, start, finish, q);

  return true;
}

bool bst_get_max_quotation_from_date_interval(BST_t bst, date_t start,
                                              date_t finish, quotation_t *q) {
  if (bst == NULL) {
    return false;
  }

  if (date_is_inside_interval(bst->quotation.date, start, finish)) {
    if (bst->quotation.value > q->value) {
      *q = bst->quotation;
    }
  }

  bst_get_max_quotation_from_date_interval(bst->left, start, finish, q);
  bst_get_max_quotation_from_date_interval(bst->right, start, finish, q);

  return true;
}

bool bst_get_max_quotation(BST_t bst, quotation_t *q) {
  if (bst == NULL) {
    return false;
  }

  if (bst->quotation.value > q->value) {
    *q = bst->quotation;
  }

  bst_get_max_quotation(bst->left, q);
  bst_get_max_quotation(bst->right, q);

  return true;
}

bool bst_get_min_quotation(BST_t bst, quotation_t *q) {
  if (bst == NULL) {
    return false;
  }

  if (bst->quotation.value < q->value) {
    *q = bst->quotation;
  }

  bst_get_min_quotation(bst->left, q);
  bst_get_min_quotation(bst->right, q);

  return true;
}

int bst_get_longest_path(BST_t bst) {
  if (bst == NULL) {
    return 0;
  }
  int longest_left = 1 + bst_get_longest_path(bst->left);
  int longest_right = 1 + bst_get_longest_path(bst->right);
  return max(longest_left, longest_right);
}

int bst_get_shortest_path(BST_t bst) {
  if (bst == NULL) {
    return 0;
  }
  int shortest_left = 1 + bst_get_shortest_path(bst->left);
  int shortest_right = 1 + bst_get_shortest_path(bst->right);
  return min(shortest_left, shortest_right);
}

int bst_count_nodes(BST_t bst) {
  if (bst == NULL) {
    return 0;
  }
  return (1 + bst_count_nodes(bst->left) + bst_count_nodes(bst->right));
}

static size_t node_index = 0;
void bst_get_traverse_inorder_fill_nodes(BST_t bst, quotation_t *nodes) {
  if (bst == NULL) {
    return;
  }
  bst_get_traverse_inorder_fill_nodes(bst->left, nodes);
  nodes[node_index++] = bst->quotation;
  bst_get_traverse_inorder_fill_nodes(bst->right, nodes);
}

// left and right __MUST__ be int and not size_t because when we are doing
// mid-1 we might get to a negative right value which will underflow if an
// unsigned datatype is used. If a signed datatype is used the left>right check
// will simply return NULL, ensuring the correctness
BST_t bst_from_sorted_array(quotation_t *nodes, int left, int right) {
  if (left > right) {
    return NULL;
  }

  int mid = (left + right) / 2;

  BST_t root = malloc(sizeof(struct BST));
  root->quotation = nodes[mid];
  root->left = bst_from_sorted_array(nodes, left, mid - 1);
  root->right = bst_from_sorted_array(nodes, mid + 1, right);

  return root;
}

void bst_rebalance(BST_t *bst) {
  int nodes_len = bst_count_nodes(*bst);
  quotation_t *nodes = calloc(nodes_len, sizeof(quotation_t));

  // fill nodes with the values of the array, since we are traversing in-order
  // the array will be sorted
  bst_get_traverse_inorder_fill_nodes(*bst, nodes);

  // Free the old tree since we will create a new one
  BST_free(*bst);

  // Rebuild a balanced binary tree from the sorted array
  BST_t new_tree = bst_from_sorted_array(nodes, 0, nodes_len - 1);

  *bst = new_tree;

  free(nodes);
}

#endif
