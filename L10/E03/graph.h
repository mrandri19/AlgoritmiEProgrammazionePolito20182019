#ifndef GRAPH_T
#define GRAPH_T

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "edge.h"
#include "linked_list.h"
#include "symbol_table.h"

struct graph {
  // Adjacency list
  linked_list_t *adjacency_list;
  size_t vertexes_len;

  // Maps 2 indexes to a weight
  int **adjacency_matrix;

  // Maps names to indexes
  symbol_table_t vertexes;

  // Whether it's using an adjacency list or matrix
  bool using_adjacency_list;
};
typedef struct graph *graph_t;

graph_t graph_empty() {
  graph_t G = (graph_t)malloc(sizeof(struct graph));

  G->adjacency_list = NULL;
  G->vertexes_len = 0;
  G->adjacency_matrix = NULL;
  G->vertexes = symbol_table_empty();
  G->using_adjacency_list = false;

  return G;
}

graph_t graph_load(FILE *fp, bool with_adjacency_list) {
  graph_t G = graph_empty();

  char a_name[LEN], a_net_name[LEN], b_name[LEN], b_net_name[LEN];
  int weight;

  // Count vertexes
  while (fscanf(fp, "%s %s %s %s %d ", a_name, a_net_name, b_name, b_net_name,
                &weight) != EOF) {
    ssize_t a_index = symbol_table_find_by_name(G->vertexes, a_name);
    if (a_index == -1) {
      G->vertexes_len++;
      a_index = symbol_table_add(G->vertexes, a_name);
    }

    ssize_t b_index = symbol_table_find_by_name(G->vertexes, b_name);
    if (b_index == -1) {
      G->vertexes_len++;
      b_index = symbol_table_add(G->vertexes, b_name);
    }
  }

  if (with_adjacency_list) {
    G->using_adjacency_list = true;

    // Allocate adjaceny_list
    G->adjacency_list =
        (linked_list_t *)calloc(G->vertexes_len, sizeof(linked_list_t));

    // Fill list
    fseek(fp, 0, SEEK_SET);
    while (fscanf(fp, "%s %s %s %s %d ", a_name, a_net_name, b_name, b_net_name,
                  &weight) != EOF) {
      ssize_t a_index = symbol_table_find_by_name(G->vertexes, a_name);
      // We have just created the symbol table, all vertexes must be in it
      assert(a_index > -1);

      ssize_t b_index = symbol_table_find_by_name(G->vertexes, b_name);
      assert(b_index > -1);

      linked_list_insert(
          &(G->adjacency_list[a_index]),
          (edge_t){.id = (size_t)a_index, .id_net = 0, .weight = weight});
    }

    return G;
  } else {
    // Allocate matrix
    G->adjacency_matrix = (int **)calloc(G->vertexes_len, sizeof(size_t *));
    for (size_t i = 0; i < G->vertexes_len; i++) {
      G->adjacency_matrix[i] = (int *)calloc(G->vertexes_len, sizeof(size_t));
    }

    // Fill matrix
    fseek(fp, 0, SEEK_SET);
    while (fscanf(fp, "%s %s %s %s %d ", a_name, a_net_name, b_name, b_net_name,
                  &weight) != EOF) {
      ssize_t a_index = symbol_table_find_by_name(G->vertexes, a_name);
      // We have just created the symbol table, all vertexes must be in it
      assert(a_index > -1);

      ssize_t b_index = symbol_table_find_by_name(G->vertexes, b_name);
      assert(b_index > -1);

      // Undirected weighted graph
      G->adjacency_matrix[a_index][b_index] = weight;
      G->adjacency_matrix[b_index][a_index] = weight;
    }

    return G;
  }
}

void graph_free(graph_t G) {
  free(G->adjacency_list);

  if (G->using_adjacency_list) {
    free(G->adjacency_list);
  } else {
    for (size_t i = 0; i < symbol_table_size(G->vertexes); i++) {
      free(G->adjacency_matrix[i]);
    }
    free(G->adjacency_matrix);
  }

  symbol_table_free(G->vertexes);
}

// Global variable needed by `comparator` which is used in `qsort`
static symbol_table_t global_vertexes;
int comparator(const void *a_, const void *b_) {
  const int *a = (const int *)a_;
  const int *b = (const int *)b_;

  return strcmp(symbol_table_get(global_vertexes, *a),
                symbol_table_get(global_vertexes, *b));
}

int comparator_edge(const void *a_, const void *b_) {
  const edge_t *a = (const edge_t *)a_;
  const edge_t *b = (const edge_t *)b_;

  return strcmp(symbol_table_get(global_vertexes, a->id),
                symbol_table_get(global_vertexes, b->id));
}

void graph_print_vertexes(graph_t G) {
  if (G->using_adjacency_list) {
    size_t vertexes_len = symbol_table_size(G->vertexes);
    size_t *list = (size_t *)calloc(vertexes_len, sizeof(size_t));

    // Fill the unsorted list with vertexes
    for (size_t i = 0; i < vertexes_len; i++) {
      list[i] = i;
    }

    // Sort the list
    global_vertexes = G->vertexes;
    qsort(list, vertexes_len, sizeof(size_t), comparator);

    for (size_t i = 0; i < vertexes_len; ++i) {
      size_t index = list[i];
      printf("%s\n", symbol_table_get(G->vertexes, index));

      edge_t *edges = (edge_t *)calloc(vertexes_len, sizeof(edge_t));

      size_t k = 0;
      // Fill the edges list with the edges of a certain vertex

      linked_list_print(G->adjacency_list, index);

      qsort(edges, vertexes_len, sizeof(edge_t), comparator_edge);

      for (size_t l = 0; l < k; ++l) {
        printf("\t%s <- %d -> %s\n", symbol_table_get(G->vertexes, index),
               edges[l].weight, symbol_table_get(G->vertexes, edges[l].id));
      }

      assert(k <= vertexes_len);
      free(edges);
    }

    free(list);
  } else {
    size_t vertexes_len = symbol_table_size(G->vertexes);
    size_t *list = (size_t *)calloc(vertexes_len, sizeof(size_t));

    // Fill the unsorted list with vertexes
    for (size_t i = 0; i < vertexes_len; i++) {
      list[i] = i;
    }

    // Sort the list
    global_vertexes = G->vertexes;
    qsort(list, vertexes_len, sizeof(size_t), comparator);

    // Print it
    for (size_t i = 0; i < vertexes_len; i++) {
      size_t vertex = list[i];
      printf("%s\n", symbol_table_get(G->vertexes, vertex));

      size_t *edges = (size_t *)calloc(vertexes_len, sizeof(size_t));

      // Fill the unsorted list with edges
      for (size_t j = 0; j < vertexes_len; j++) {
        edges[j] = j;
      }

      // Sort the list
      qsort(edges, vertexes_len, sizeof(size_t), comparator);

      // Print it
      for (size_t j = 0; j < vertexes_len; j++) {
        int weight = G->adjacency_matrix[vertex][edges[j]];
        if (weight != 0) {
          printf("\t%s <- %d -> %s\n", symbol_table_get(G->vertexes, vertex),
                 weight, symbol_table_get(G->vertexes, edges[j]));
        }
      }

      free(edges);
    }
    printf("\n");

    free(list);
  }
}

bool graph_vertexes_form_complete_subgraph(graph_t G, char *first, char *second,
                                           char *third) {
  ssize_t first_id = symbol_table_find_by_name(G->vertexes, first);
  assert(first_id > -1);
  ssize_t second_id = symbol_table_find_by_name(G->vertexes, second);
  assert(second_id > -1);
  ssize_t third_id = symbol_table_find_by_name(G->vertexes, third);
  assert(third_id > -1);

  if (G->using_adjacency_list) {
    int weight_12 = 0, weight_23 = 0, weight_31 = 0;

    // For each element of linked_list[first_id] check if id == 2
    // if (G->adjacency_list[i].a == (size_t)first_id &&
    //     G->adjacency_list[i].b == (size_t)second_id) {
    //   weight_12 = G->adjacency_list[i].weight;
    // }
    // if (G->adjacency_list[i].a == (size_t)second_id &&
    //     G->adjacency_list[i].b == (size_t)first_id) {
    //   weight_12 = G->adjacency_list[i].weight;
    // }

    // if (G->adjacency_list[i].a == (size_t)second_id &&
    //     G->adjacency_list[i].b == (size_t)third_id) {
    //   weight_23 = G->adjacency_list[i].weight;
    // }
    // if (G->adjacency_list[i].a == (size_t)third_id &&
    //     G->adjacency_list[i].b == (size_t)second_id) {
    //   weight_23 = G->adjacency_list[i].weight;
    // }

    // if (G->adjacency_list[i].a == (size_t)third_id &&
    //     G->adjacency_list[i].b == (size_t)first_id) {
    //   weight_31 = G->adjacency_list[i].weight;
    // }
    // if (G->adjacency_list[i].a == (size_t)first_id &&
    //     G->adjacency_list[i].b == (size_t)third_id) {
    //   weight_31 = G->adjacency_list[i].weight;
    // }

    return (weight_12 * weight_23 * weight_31) > 0;
  } else {
    // if second and third are ajdacent to first
    // if second is adjacent to third
    // we can do only three checks since it's undirected
    int weight_12 = G->adjacency_matrix[first_id][second_id];
    assert(G->adjacency_matrix[first_id][second_id] ==
           G->adjacency_matrix[second_id][first_id]);

    int weight_23 = G->adjacency_matrix[second_id][third_id];
    assert(G->adjacency_matrix[second_id][third_id] ==
           G->adjacency_matrix[third_id][second_id]);

    int weight_31 = G->adjacency_matrix[third_id][first_id];
    assert(G->adjacency_matrix[third_id][first_id] ==
           G->adjacency_matrix[first_id][third_id]);

    return (weight_12 * weight_23 * weight_31) > 0;
  }
}

void graph_print_adjacency_matrix(graph_t G) {
  for (size_t i = 0; i < symbol_table_size(G->vertexes); i++) {
    for (size_t j = 0; j < symbol_table_size(G->vertexes); j++) {
      if (i == j) {
        printf("\033[36m\033[1m");
      }
      printf("%4d ", G->adjacency_matrix[i][j]);
      if (i == j) {
        printf("\033[0m ");
      }
    }
    printf("\n");
  }
}

void graph_switch_to_adjacency_list(graph_t G) {
  if (G->using_adjacency_list) {
    printf("Already using adjacency_list internally\n");
    exit(EXIT_FAILURE);
  }
  G->using_adjacency_list = true;
  G->vertexes_len = 0;

  // Generate adjacency list from matrix

  // For each vertex
  for (size_t i = 0; i < G->vertexes_len; ++i) {
    for (size_t j = 0; j < G->vertexes_len; ++j) {
      // TODO: finish
    }
  }
}

#endif
