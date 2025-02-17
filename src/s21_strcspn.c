#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  const char *p1 = str1;
  s21_size_t res = s21_strlen(str1);
  int should_continue = 1;
  while (*p1 && should_continue) {
    const char *p2 = str2;
    while (*p2 && should_continue) {
      if (*p1 == *p2) {
        res = p1 - str1;
        should_continue = 0;
      }
      p2++;
    }
    p1++;
  }
  return res;
}