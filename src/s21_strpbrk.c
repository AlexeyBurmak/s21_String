#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *res = s21_NULL;

  while (*str1 != '\0' && res == s21_NULL) {
    const char *temp = str2;

    while (*temp != '\0') {
      if (*str1 == *temp) {
        res = (char *)str1;
        temp = "\0";
      } else {
        temp++;
      }
    }

    str1++;
  }

  return res;
}