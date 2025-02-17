#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *result = s21_NULL;
  unsigned char *s = (unsigned char *)str;
  unsigned char uc = (unsigned char)c;

  for (s21_size_t i = 0; i < n && result == s21_NULL; i++) {
    if (s[i] == uc) {
      result = (void *)(s + i);
    }
  }
  return result;
}