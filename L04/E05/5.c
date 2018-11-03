#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 30 + 1
#define MAX_LINES 1000

#define COMMAND_BUFFER 255 + 1

#define SECONDS_IN_YEAR 365 * 24 * 60 * 60
#define SECONDS_IN_MONTH 30 * 24 * 60 * 60
#define SECONDS_IN_DAY 24 * 60 * 60
#define SECONDS_IN_HOUR 60 * 60
#define SECONDS_IN_MINUTE 60

typedef struct {
    char code[MAX_LEN];
    char start[MAX_LEN];
    char destination[MAX_LEN];
    char date[MAX_LEN];
    char start_time[MAX_LEN];
    char arrival_time[MAX_LEN];
    int delay;
} line;

void print_lines(line lines[], int lines_len, FILE* fp)
{
    for (int i = 0; i < lines_len; ++i) {
        fprintf(fp, "%s %s %s %s %s %s %d\n", lines[i].code, lines[i].start,
            lines[i].destination, lines[i].date, lines[i].start_time,
            lines[i].arrival_time, lines[i].delay);
    }
}

void print_line(line* line)
{
    printf("%-15s %-15s %-15s %-15s %-15s %-15s %d\n", line->code, line->start, line->destination,
        line->date, line->start_time, line->arrival_time, line->delay);
}

int datetime_to_epoch(char* date, char* time)
{
    int year, month, day;
    int hour, minute, second;
    sscanf(date, "%d/%d/%d", &year, &month, &day);
    sscanf(time, "%d:%d:%d", &hour, &minute, &second);
    return ((year - 1970) * SECONDS_IN_YEAR + month * SECONDS_IN_MONTH + day * SECONDS_IN_DAY + hour * SECONDS_IN_HOUR + minute * SECONDS_IN_MINUTE + second);
}

bool line_date_less_than(line* a, line* b)
{
    int a_epoch = datetime_to_epoch(a->date, a->start_time);
    int b_epoch = datetime_to_epoch(b->date, b->start_time);
    return a_epoch < b_epoch;
}
bool line_date_equal_to(line* a, line* b)
{
    int a_epoch = datetime_to_epoch(a->date, a->start_time);
    int b_epoch = datetime_to_epoch(b->date, b->start_time);
    return a_epoch == b_epoch;
}

bool line_code_less_than(line* a, line* b)
{
    int res = strcmp(a->code, b->code);
    if (res == 0 || res > 0) {
        return false;
    } else {
        return true;
    }
}

bool line_code_equal_to(line* a, line* b)
{
    int res = strcmp(a->code, b->code);
    if (res == 0) {
        return true;
    } else {
        return false;
    }
}

bool line_start_less_than(line* a, line* b)
{
    int res = strcmp(a->start, b->start);
    if (res == 0 || res > 0) {
        return false;
    } else {
        return true;
    }
}

bool line_start_equal_to(line* a, line* b)
{
    int res = strcmp(a->start, b->start);
    if (res == 0) {
        return true;
    } else {
        return false;
    }
}

bool line_destination_less_than(line* a, line* b)
{
    int res = strcmp(a->destination, b->destination);
    if (res == 0 || res > 0) {
        return false;
    } else {
        return true;
    }
}

bool line_destination_equal_to(line* a, line* b)
{
    int res = strcmp(a->destination, b->destination);
    if (res == 0) {
        return true;
    } else {
        return false;
    }
}

void merge(line** A, line** B, int l, int q, int r, bool (*comparator_lt)(line*, line*), bool (*comparator_eq)(line*, line*))
{
    int i = l;
    int j = q + 1;
    for (int k = l; k <= r; ++k) {
        if (i > q)
            B[k] = A[j++];
        else if (j > r)
            B[k] = A[i++];
        else if (comparator_lt(A[i], A[j]) || comparator_eq(A[i], A[j]))
            B[k] = A[i++];
        else
            B[k] = A[j++];
    }
    for (int k = l; k <= r; k++) {
        A[k] = B[k];
    }
}

void mergesort_aux(line** A, line** B, int l, int r, bool (*comparator_lt)(line*, line*), bool (*comparator_eq)(line*, line*))
{
    int q = (l + r) / 2;
    if (r <= l)
        return;

    mergesort_aux(A, B, l, q, comparator_lt, comparator_eq);
    mergesort_aux(A, B, q + 1, r, comparator_lt, comparator_eq);
    merge(A, B, l, q, r, comparator_lt, comparator_eq);
}

void mergesort(line** lines, int l, int r, bool (*comparator_lt)(line*, line*), bool (*comparator_eq)(line*, line*))
{
    line** B = malloc((r - l + 1) * sizeof(line*));
    mergesort_aux(lines, B, l, r, comparator_lt, comparator_eq);
    free(B);
    return;
}

line* linear_search(line lines[], char* query, int lines_len)
{
    for (int i = 0; i < lines_len; i++) {
        char* res = strstr(lines[i].start, query);
        if (res == NULL)
            continue;
        if (res != lines[i].start)
            continue; // If the occurrence was not at the start
        return &lines[i];
    }

    return NULL;
}

line* binary_search(line** lines, char* query, int lines_len)
{
    int l = 0, r = lines_len - 1;
    while (l <= r) {
        int m = (l + r) / 2;

        char* res = strstr(lines[m]->start, query);

        if ((res == NULL) || (res != lines[m]->start)) {
            if (strcmp(query, lines[m]->start) > 0) {
                l = m + 1;
            } else {
                r = m - 1;
            }
        } else {
            return lines[m];
        }
    }
    return NULL;
}

typedef enum { PRINT_STDOUT,
    PRINT_FILE,
    SORT_DATETIME,
    SORT_CODE,
    SORT_START,
    SORT_DESTINATION,
    SEARCH_START,
    OTHER } commands;

commands parse_command(char* command_str)
{
    if (strcmp(command_str, "a") == 0) {
        return PRINT_STDOUT;
    } else if (strcmp(command_str, "b") == 0) {
        return PRINT_FILE;
    } else if (strcmp(command_str, "c") == 0) {
        return SORT_DATETIME;
    } else if (strcmp(command_str, "d") == 0) {
        return SORT_CODE;
    } else if (strcmp(command_str, "e") == 0) {
        return SORT_START;
    } else if (strcmp(command_str, "f") == 0) {
        return SORT_DESTINATION;
    } else if (strcmp(command_str, "g") == 0) {
        return SEARCH_START;
    } else {
        return OTHER;
    }
}

int main()
{
    FILE* fp = fopen("corse.txt", "r");
    if (fp == NULL) {
        perror("cannot open corse.txt");
        return 1;
    }
    int lines_len;
    line lines[MAX_LINES];

    // Read the file
    fscanf(fp, "%d", &lines_len);
    for (int i = 0; i < lines_len; ++i) {
        fscanf(fp, "%s %s %s %s %s %s %d\n", lines[i].code, lines[i].start,
            lines[i].destination, lines[i].date, lines[i].start_time,
            lines[i].arrival_time, &(lines[i].delay));
    }

    bool is_sorted_datetime = false;
    line** sorted_lines_datetime = calloc(sizeof(line*), lines_len);
    for (int i = 0; i < lines_len; ++i) {
        sorted_lines_datetime[i] = &lines[i];
    }

    bool is_sorted_code = false;
    line** sorted_lines_code = calloc(sizeof(line*), lines_len);
    for (int i = 0; i < lines_len; ++i) {
        sorted_lines_code[i] = &lines[i];
    }

    bool is_sorted_start = false;
    line** sorted_lines_start = calloc(sizeof(line*), lines_len);
    for (int i = 0; i < lines_len; ++i) {
        sorted_lines_start[i] = &lines[i];
    }

    bool is_sorted_destination = false;
    line** sorted_lines_destination = calloc(sizeof(line*), lines_len);
    for (int i = 0; i < lines_len; ++i) {
        sorted_lines_destination[i] = &lines[i];
    }

    bool main_loop_flag = true;
    while (main_loop_flag) {
        printf("Insert your command: ");
        char command[COMMAND_BUFFER];
        scanf("%s", command);
        printf("\n");

        switch (parse_command(command)) {
        case PRINT_STDOUT:
            print_lines(lines, lines_len, stderr);
            break;
        case PRINT_FILE: {
            FILE* out_fp = fopen("out.txt", "w");
            if (fp == NULL) {
                perror("cannot open out.txt");
                return 1;
            }
            print_lines(lines, lines_len, out_fp);

            fclose(out_fp);
            break;
        }
        case SORT_DATETIME: {
            mergesort(sorted_lines_datetime, 0, lines_len - 1, line_date_less_than, line_date_equal_to);
            is_sorted_datetime = true;
            break;
        }
        case SORT_CODE: {
            mergesort(sorted_lines_code, 0, lines_len - 1, line_code_less_than, line_code_equal_to);
            is_sorted_code = true;
            break;
        }
        case SORT_START: {
            mergesort(sorted_lines_start, 0, lines_len - 1, line_start_less_than, line_start_equal_to);
            is_sorted_start = true;
            break;
        }
        case SORT_DESTINATION: {
            mergesort(sorted_lines_destination, 0, lines_len - 1, line_destination_less_than, line_destination_equal_to);
            is_sorted_destination = true;
            break;
        }
        case SEARCH_START: {
            printf("Insert your query: ");
            char query[COMMAND_BUFFER];
            scanf("%s", query);
            line* res;
            if (is_sorted_start) {
                puts("Binary searching");
                res = binary_search(sorted_lines_start, query, lines_len);
            } else {
                puts("Linear searching");
                res = linear_search(lines, query, lines_len);
            }
            if (res == NULL) {
                printf("'%s' not found", query);
            } else {
                print_line(res);
            }
            break;
        }
        case OTHER: {
            printf("Exiting...");
            main_loop_flag = false;
            break;
        }
        }
        printf("\n");
    }

    free(sorted_lines_datetime);
    free(sorted_lines_code);
    free(sorted_lines_start);
    free(sorted_lines_destination);
    fclose(fp);

    return 0;
}
