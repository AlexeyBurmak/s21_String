#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  char *res = s21_NULL;

  while (*str != '\0') {
    if (*str == (char)c) {
      res = (char *)str;
    }
    str++;
  }

  if ((char)c == '\0') {
    res = (char *)str;
  }

  return res;
}