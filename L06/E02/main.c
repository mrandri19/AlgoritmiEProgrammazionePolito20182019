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

typedef enum {
    GREATER,
    EQUAL,
    LESSER
} birthday_compare_result;

// a 3/12/98, b 21/1/99 then a < b
birthday_compare_result birthday_compare(struct birthday a, struct birthday b)
{
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
}

typedef struct node {
    item head;
    struct node* tail;
} linked_list;

void print_linked_list(FILE* fp, linked_list* list)
{
    if (list == NULL) {
        printf("[]\n");
        return;
    }
    printf("item::");
    print_linked_list(fp, list->tail);
    return;
}

linked_list* insert_ordered_linked_list(item i, linked_list* list)
{
    if (list == NULL) {
        linked_list* new_list = calloc(1, sizeof(linked_list));
        new_list->head = i;
        new_list->tail = NULL;
        return new_list;
    } else {
        // TODO: finish
        if (birthday_compare(list->head.birthday, i.birthday) == LESSER) {
            // continue iterating
        } else {
            // insert here
        }
    }
}

void main_loop(linked_list* list)
{
    printf("> ");
    char command[COMMAND_BUF_LEN];

    // Handle exit with C-d
    if (scanf("%s", command) == EOF) {
        printf("Bye\n");
        return;
    }

    if (strcmp(command, "ik") == 0) {
        item i;
        scanf("%s %s %s %d/%d/%d %s %s %d",
            i.code,
            i.name,
            i.surname,
            &i.birthday.day,
            &i.birthday.month,
            &i.birthday.year,
            i.street,
            i.city,
            &i.cap);
        list = insert_ordered_linked_list(i, list);
    } else if (strcmp(command, "if") == 0) {
        printf("Insert from file\n");
    } else if (strcmp(command, "sc") == 0) {
        printf("Search by code\n");
    } else if (strcmp(command, "dc") == 0) {
        printf("Delete by code\n");
    } else if (strcmp(command, "dr") == 0) {
        printf("Delete by date range\n");
    } else if (strcmp(command, "pf") == 0) {
        print_linked_list(stdout, list);
    } else {
        printf("Unknown command\n");
    }

    main_loop(list);
}

int main()
{
    printf("Hello, world!\n");

    linked_list* list = NULL;

    main_loop(list);

    // Init empty linked list

    // Everything is sorted ascending by birthday

    // Options
    //   Insert ordered from keyboard
    //   Multiple insert ordered from file
    //   Search by code
    //   Delete by code
    //   Delete by date range
    //   Print to file

    return 0;
}
