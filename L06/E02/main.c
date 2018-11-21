#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FIELD_BUF_LEN 50 + 1
#define COMMAND_BUF_LEN 1024

typedef struct {
  char code[MAX_FIELD_BUF_LEN];
  char name[MAX_FIELD_BUF_LEN];
  char surname[MAX_FIELD_BUF_LEN];

  struct birthday {
    int day;
    int month;
    int year;
  } birthday;

  char street[MAX_FIELD_BUF_LEN];
  char city[MAX_FIELD_BUF_LEN];
  int cap;
} item;

void print_item(item i) {
  printf("%s %s %s %d/%d/%d %s %s %d\n", i.code, i.name, i.surname,
         i.birthday.day, i.birthday.month, i.birthday.year, i.street, i.city,
         i.cap);
}

typedef enum { GREATER, EQUAL, LESSER } birthday_compare_result;

// a 3/12/98, b 21/1/99 then a < b
birthday_compare_result birthday_compare(struct birthday a, struct birthday b) {
  if (a.year < b.year) {
    return LESSER;
  }
  if (a.year > b.year) {
    return GREATER;
  }
  if (a.year == b.year) {
    if (a.month < b.month) {
      return LESSER;
    }
    if (a.month > b.month) {
      return GREATER;
    }
    if (a.month == b.month) {
      if (a.day < b.day) {
        return LESSER;
      }
      if (a.day > b.month) {
        return GREATER;
      }
      if (a.day == b.day) {
        return EQUAL;
      }
    }
  }
  printf("Impossible\n");
  exit(1);
}

typedef struct node {
  item head;
  struct node* tail;
} linked_list;

void print_linked_list(FILE* fp, linked_list* list) {
  if (list == NULL) {
    return;
  }
  print_item(list->head);
  print_linked_list(fp, list->tail);
  return;
}

linked_list* insert_ordered_linked_list(item i, linked_list* list) {
  // case 1: inserting into empty
  if (list == NULL) {
    linked_list* new_list = calloc(1, sizeof(linked_list));
    new_list->head = i;
    new_list->tail = NULL;
    return new_list;
  } else {
    // case 2 insert into existing list
    // case 2a: insert into list where the list's head is **less** than the item
    // to insert
    if (birthday_compare(list->head.birthday, i.birthday) == LESSER) {
      list->tail = insert_ordered_linked_list(i, list->tail);
      return list;
    }
    // case 2b: insert into list where the list's head is **more** than the item
    // to insert
    else {
      linked_list* new_list = calloc(1, sizeof(linked_list));
      new_list->head = i;
      new_list->tail = list;
      return new_list;
    }
  }
}

item* search_code_linked_list(char* code, linked_list* list) {
  if (list == NULL) {
    return NULL;
  }
  if (strcmp(list->head.code, code) == 0) {
    return &(list->head);
  } else {
    return search_code_linked_list(code, list->tail);
  }
}

item* find_item_withing_date_range_linked_list(struct birthday start,
                                               struct birthday end,
                                               linked_list* list) {
  if (list == NULL) {
    return NULL;
  }
  if (birthday_compare(list->head.birthday, start) == GREATER &&
      birthday_compare(list->head.birthday, end) == LESSER) {
    return &(list->head);
  } else {
    return find_item_withing_date_range_linked_list(start, end, list->tail);
  }
}

linked_list* delete_item_code_linked_list(char* code, linked_list* list,
                                          item* item_returned, int* success) {
  if (list == NULL) {
    *success = 0;
    return NULL;
  }
  if (strcmp(list->head.code, code) == 0) {
    *success = 1;
    if (item_returned != NULL) {
      *item_returned = list->head;
    }
    return list->tail;
  } else {
    list->tail =
        delete_item_code_linked_list(code, list->tail, item_returned, success);
    return list;
  }
}

void main_loop(linked_list* list) {
  printf("> ");
  char command[COMMAND_BUF_LEN];

  // Handle exit with C-d
  if (scanf("%s", command) == EOF) {
    printf("Bye\n");
    return;
  }

  if (strcmp(command, "ik") == 0) {
    item i;
    scanf("%s %s %s %d/%d/%d %s %s %d", i.code, i.name, i.surname,
          &i.birthday.day, &i.birthday.month, &i.birthday.year, i.street,
          i.city, &i.cap);
    list = insert_ordered_linked_list(i, list);
  } else if (strcmp(command, "if") == 0) {
    char filename[1024];
    printf("file name: ");
    scanf("%s", filename);
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
      perror("Cannot open file");
    } else {
      item i;
      while (fscanf(fp, "%s %s %s %d/%d/%d %s %s %d", i.code, i.name, i.surname,
                    &i.birthday.day, &i.birthday.month, &i.birthday.year,
                    i.street, i.city, &i.cap) != EOF) {
        list = insert_ordered_linked_list(i, list);
      }
    }
  } else if (strcmp(command, "sc") == 0) {
    printf("insert the code to search: ");

    char code[MAX_FIELD_BUF_LEN];
    scanf("%s", code);

    item* res = search_code_linked_list(code, list);
    if (res == NULL) {
      printf("Cannot find item with such code\n");
    } else {
      printf("found an item with such code\n");
      print_item(*res);
    }
  } else if (strcmp(command, "dc") == 0) {
    printf("insert the code to delete: ");

    char code[MAX_FIELD_BUF_LEN];
    scanf("%s", code);

    item* res = malloc(sizeof(item));
    int success = 0;
    list = delete_item_code_linked_list(code, list, res, &success);
    if (!success) {
      printf("Cannot find item with such code\n");
    } else {
      printf("deleted\n");
      print_item(*res);
    }
  } else if (strcmp(command, "dr") == 0) {
    struct birthday start, end;
    printf("insert date range start: ");
    scanf("%d/%d/%d", &start.day, &start.month, &start.year);
    printf("insert date range end: ");
    scanf("%d/%d/%d", &end.day, &end.month, &end.year);

    item *item, *item_deleted;
    while ((item = find_item_withing_date_range_linked_list(start, end,
                                                            list)) != NULL) {
      item_deleted = malloc(sizeof(item));
      int success = 0;
      list = delete_item_code_linked_list(item->code, list, item_deleted,
                                          &success);
      if (success) {
        printf("deleted an item\n");
        print_item(*item_deleted);
      } else {
        printf("failed to delete an item");
      }
      free(item_deleted);
    }

  } else if (strcmp(command, "pf") == 0) {
    char filename[1024];
    printf("file name: ");
    scanf("%s", filename);
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
      perror("Cannot open file");
    } else {
      print_linked_list(fp, list);
    }
  } else {
    printf("Unknown command\n");
  }

  main_loop(list);
}

int main() {
  linked_list* list = NULL;

  main_loop(list);

  //   Delete by date range
  //   Print to file

  return 0;
}
