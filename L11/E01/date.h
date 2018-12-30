#ifndef DATE_H
#define DATE_H
/** Quasi-ADT for date */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int year;
  int month;
  int day;
} date_t;

date_t date_load(FILE *fp) {
  date_t date;
  fscanf(fp, "%d/%d/%d ", &date.year, &date.month, &date.day);
  return date;
}

void date_print(date_t date) {
  printf("%d/%d/%d", date.year, date.month, date.day);
}

bool date_equal(date_t date1, date_t date2) {
  return ((date1.day == date2.day) && (date1.month == date2.month) &&
          (date1.year && date2.year));
}

bool date_lesser(date_t date1, date_t date2) {
  if (date1.year < date2.year) {
    return true;
  }
  if (date1.year > date2.year) {
    return false;
  }

  if (date1.month < date2.month) {
    return true;
  }
  if (date1.month > date2.month) {
    return false;
  }

  if (date1.day < date2.day) {
    return true;
  }
  if (date1.day > date2.day) {
    return false;
  }

  // They are equal
  return false;
}

bool date_is_inside_interval(date_t date, date_t start, date_t finish) {
  if ((date_lesser(start, date) && date_lesser(date, finish)) ||
      (date_equal(date, start) || date_equal(date, finish))) {
    return true;
  }
  return false;
}

date_t date_load_from_string(char *s) {
  date_t date;
  sscanf(s, "%d/%d/%d", &date.year, &date.month, &date.day);
  return date;
}

int date_comparator(const void *p1, const void *p2) {
  return !date_lesser(*((const date_t *)p1), *((const date_t *)p2));
}

#endif
