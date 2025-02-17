#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  char *res = s21_NULL;

  for (; *str != '\0' && res == s21_NULL; str++) {
    if (*str == (char)c) {
      res = (char *)str;
    }
  }

  if ((char)c == '\0') {
    res = (char *)str;
  }

  return res;
}