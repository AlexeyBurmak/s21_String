#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  const char *s = (const char *)src;
  char *d = (char *)dest;

  for (s21_size_t i = 0; i < n; i++) {
    d[i] = s[i];
  }
  return dest;
}