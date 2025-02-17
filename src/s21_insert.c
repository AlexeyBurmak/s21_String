#include "s21_string.h"

char *s21_insert(const char *src, const char *str, int start_index) {
  if (!src || !str || start_index < 0 || start_index > (int)s21_strlen(src)) {
    return s21_NULL;
  }

  int src_len = s21_strlen(src);
  int str_len = s21_strlen(str);

  char *new_str = (char *)calloc((src_len + str_len + 1), sizeof(char));

  if (new_str) {
    s21_strncpy(new_str, src, start_index);
    s21_strncpy(new_str + start_index, str, str_len);
    s21_strncpy(new_str + start_index + str_len, src + start_index,
                src_len - start_index);
  } else {
    new_str = s21_NULL;
  }

  return new_str;
}