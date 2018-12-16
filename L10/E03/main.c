#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror("cannot open file");
    exit(EXIT_FAILURE);
  }

  graph_t G = graph_load(fp, false);

  // graph_print_vertexes(G);

  printf("%s\n",
         (graph_vertexes_form_complete_subgraph(G, "Shiva", "Ifrit", "Alderaan")
              ? "true"
              : "false"));

  printf("%s\n",
         (graph_vertexes_form_complete_subgraph(G, "Shiva", "Ifrit", "Axii")
              ? "true"
              : "false"));

  graph_switch_to_adjacency_list(G);

  printf("%s\n",
         (graph_vertexes_form_complete_subgraph(G, "Shiva", "Ifrit", "Alderaan")
              ? "true"
              : "false"));

  printf("%s\n",
         (graph_vertexes_form_complete_subgraph(G, "Shiva", "Ifrit", "Axii")
              ? "true"
              : "false"));

  graph_free(G);

  fclose(fp);
  return 0;
}
