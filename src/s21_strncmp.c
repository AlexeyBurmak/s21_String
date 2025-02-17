#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0;
  const unsigned char *s1 = (const unsigned char *)str1;
  const unsigned char *s2 = (const unsigned char *)str2;

  for (s21_size_t len = 0; len < n && result == 0; len++) {
    if (*s1 == '\0' || *s2 == '\0') {
      result = *s1 - *s2;
    } else if (*s1 != *s2) {
      result = *s1 - *s2;
    }
    s1++;
    s2++;
  }
  return result;
}