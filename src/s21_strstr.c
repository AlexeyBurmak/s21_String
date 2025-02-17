#include "s21_string.h"

char *s21_strstr(const char *string, const char *substr) {
  if (string == s21_NULL || substr == s21_NULL) return s21_NULL;

  char *result = s21_NULL;

  for (s21_size_t i = 0; result == s21_NULL && string[i] != '\0'; i++) {
    s21_size_t j = 0;
    while (string[i + j] == substr[j] && substr[j] != '\0') j++;

    if (substr[j] == '\0') result = (char *)&string[i];
  }

  return result;
}
