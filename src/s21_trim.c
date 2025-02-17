#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  if (!src || !trim_chars) {
    return s21_NULL;
  }

  int start = 0;
  int end = s21_strlen(src) - 1;

  while (start <= end && s21_strchr(trim_chars, src[start])) {
    start++;
  }

  while (end >= start && s21_strchr(trim_chars, src[end])) {
    end--;
  }

  char *new_str = (char *)calloc(end - start + 2, sizeof(char));

  if (new_str) {
    s21_strncpy(new_str, src + start, end - start + 1);
  } else
    new_str = s21_NULL;

  return new_str;
}