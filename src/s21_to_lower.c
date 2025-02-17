#include "s21_string.h"

void *s21_to_lower(const char *str) {
  if (!str) {
    return s21_NULL;
  }

  char *new_str = (char *)calloc((s21_strlen(str) + 1), sizeof(char));
  if (new_str) {
    for (int i = 0; str[i] != '\0'; i++) {
      if (str[i] >= 'A' && str[i] <= 'Z') {
        new_str[i] = str[i] - 'A' + 'a';
      } else {
        new_str[i] = str[i];
      }
    }
  } else {
    new_str = s21_NULL;
  }

  return new_str;
}