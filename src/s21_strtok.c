#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *next = s21_NULL;
  char *result = s21_NULL;

  if (str != s21_NULL) {
    next = str;
  }

  if (next != s21_NULL) {
    while (*next && s21_strchr(delim, *next)) {
      next++;
    }

    if (*next != '\0') {
      result = next;

      while (*next && !s21_strchr(delim, *next)) {
        next++;
      }

      if (*next != '\0') {
        *next = '\0';
        next++;
      } else {
        next = s21_NULL;
      }
    }
  }

  return result;
}
