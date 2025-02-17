#include <math.h>

#include "s21_string.h"

void s21_parse_sign(const char **str, int *sign) {
  while (**str == ' ' || **str == '\t' || **str == '\n' || **str == '\r' ||
         **str == '\v' || **str == '\f') {
    (*str)++;
  }

  if (**str == '-') {
    *sign = -1;
    (*str)++;
  } else if (**str == '+') {
    *sign = 1;
    (*str)++;
  } else {
    *sign = 1;
  }
}

int s21_parse_integer(const char **str) {
  int sign = 1;
  s21_parse_sign(str, &sign);

  int res = 0;
  while (**str >= '0' && **str <= '9') {
    res = res * 10 + (**str - '0');
    (*str)++;
  }
  return res * sign;
}

double s21_char_to_double(const char *str) {
  double res = 0.0;
  int sign = 1;
  if (*str == '-') {
    sign = -1;
    str++;
  } else if (*str == '+') {
    str++;
  }
  res = s21_parse_integer(&str);
  if (*str == '.') {
    str++;
    double fract = 0.0;
    double decimal_step = 0.1;
    while (*str >= '0' && *str <= '9') {
      fract += (*str - '0') * decimal_step;
      decimal_step *= 0.1;
      str++;
    }
    res += fract;
  }
  if (*str == 'e' || *str == 'E') {
    str++;
    int exp_sign = 1;
    if (*str == '-') {
      exp_sign = -1;
      str++;
    } else if (*str == '+') {
      str++;
    }
    int exp = s21_parse_integer(&str);
    if (exp_sign > 0) {
      for (int i = 0; i < exp; i++) {
        res *= 10.0;
      }
    } else {
      for (int i = 0; i < exp; i++) {
        res /= 10.0;
      }
    }
  }
  return sign * res;
}

int s21_char_to_int(const char *str) {
  int res = 0;
  int sign = 1;

  s21_parse_sign(&str, &sign);
  res = s21_parse_integer(&str);

  return sign * res;
}

void s21_format_to_base(unsigned long num, int base, const char *base_chars,
                        char *result) {
  if (num == 0) {
    result[0] = '0';
    result[1] = '\0';
    return;
  }

  int i = 0;

  while (num > 0) {
    int remainder = num % base;
    result[i] = base_chars[remainder];
    i++;
    num = num / base;
  }

  result[i] = '\0';

  for (int j = 0; j < i / 2; j++) {
    char temp = result[j];
    result[j] = result[i - j - 1];
    result[i - j - 1] = temp;
  }
}

int s21_int_to_char(char res[LEN_DECIMAL], long int num, s21_specifier param) {
  long unsigned tmp_num;
  if (param.sp == 'u' && num < 0) {
    tmp_num = (long unsigned)num;
  } else {
    tmp_num = (long unsigned)(num < 0 ? -num : num);
  }

  long int i = 0;

  while (tmp_num != 0) {
    res[i] = tmp_num % 10 + '0';
    tmp_num = tmp_num / 10;
    i++;
  }
  if (num < 0 && param.sp != 'u') {
    res[i] = '-';
    i += 1;
  }
  long int len = i;
  res[i] = '\0';
  i -= 1;
  long int j = 0;
  while (j < i) {
    tmp_num = res[i];
    res[i] = res[j];
    res[j] = tmp_num;
    i -= 1;
    j += 1;
  }
  if (len == 0) {
    res[len] = '0';
    len += 1;
  }
  return len;
}

int s21_double_to_char(char res[LEN_DOUBLE], long double num,
                       s21_specifier param) {
  long double eps = 1e-9L;
  int len = 0;
  int additional_tail_percision = 0;
  int trueprecision;
  if (param.precision == -1)
    trueprecision = 6;
  else
    trueprecision = param.precision;
  if (isnan(num)) {
    len = 1;
    res[0] = '\0';
  } else if (fabsl(num) < eps) {
    num = 0.0;
  }
  if (!isnan(num)) {
    long int dec = num / 1;
    long double scale = powl(10.0, trueprecision + additional_tail_percision);
    long double flt = (num > 0) ? roundl((num + 1 - dec) * scale)
                                : roundl((-num + 1 + dec) * scale);
    double integral;
    long double fractional = modf(num, &integral);
    fractional *= 10;
    int dec2 = (int)fractional;
    char tail[LEN_DOUBLE] = {0};
    int tail_len = s21_int_to_char(tail, flt, param);
    s21_strnshift(tail, 1);
    if ((abs(dec2) >= 5) & ((trueprecision == 0) || (trueprecision == 1))) {
      if (dec > 0)
        dec++;
      else
        dec--;
      tail[0] = 0;
    }
    len += s21_int_to_char(res, dec, param);
    if (flt != 0) {
      res[len] = '.';
      res[++len] = '\0';
      s21_strncat(res, tail, tail_len - 1);
      len += tail_len;
    }
    res[len] = '\0';
  }
  return len;
}

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s = (const unsigned char *)src;

  if (d != s) {
    if (d < s) {
      for (s21_size_t i = 0; i < n; i++) {
        d[i] = s[i];
      }
    } else {
      for (s21_size_t i = n; i > 0; i--) {
        d[i - 1] = s[i - 1];
      }
    }
  }
  return dest;
}

int s21_degree(long double *value) {
  int degr = 0;
  long double v = *value;
  while (v >= 10.0) {
    v /= 10;
    degr++;
  }

  while (v < 0.1 && v != 0.0) {
    v *= 10;
    degr--;
  }

  *value = v;
  return degr;
}

void s21_sci_regular(s21_specifier param, char *result, long double value,
                     char *string, int *exp, int ccase) {
  int is_negative = 0;
  if (value < 0.0) {
    is_negative = 1;
    value = -value;
  }
  *exp = s21_degree(&value);
  s21_double_to_char(result, value, param);
  if (s21_strlen(result) > 3 &&
      (result[0] == '0' || (result[0] == '-' && result[1] == '0'))) {
    s21_memset(result, 0, s21_strlen(result));
    s21_double_to_char(result, value * 10, param);
    *exp -= 1;
  }
  if (is_negative) {
    s21_memmove(result + 1, result, s21_strlen(result) + 1);
    result[0] = '-';
  }
  string = string + s21_strlen(string);
  if (ccase) {
    *string++ = 'E';
  } else {
    *string++ = 'e';
  }
  if ((*exp >= 0) || (value == 0)) {
    *string++ = '+';
  } else {
    *string++ = '-';
    *exp *= -1;
  }
  char exp_num[200];
  s21_int_to_char(exp_num, *exp, param);
  if (abs(*exp) < 10) {
    exp_num[2] = '\0';
    exp_num[1] = exp_num[0];
    exp_num[0] = '0';
  }
  for (s21_size_t i = 0; i < s21_strlen(exp_num); i++) {
    if (value == 0)
      *string++ = '0';
    else
      *string++ = exp_num[i];
  }
  *string = '\0';
}

void s21_sci_inf(char *result, long double value, int ccase) {
  if (value >= 0) {
    if (ccase) {
      s21_strncpy(result, "INF", 3);
      result[3] = '\0';
    } else {
      s21_strncpy(result, "inf", 3);
      result[3] = '\0';
    }
  } else {
    if (ccase) {
      s21_strncpy(result, "-INF", 4);
      result[4] = '\0';
    } else {
      s21_strncpy(result, "-inf", 4);
      result[4] = '\0';
    }
  }
}

void s21_sci_nan(char *result, int ccase) {
  if (ccase) {
    s21_strncpy(result, "NAN", 3);
    result[3] = '\0';
  } else {
    s21_strncpy(result, "nan", 3);
    result[3] = '\0';
  }
}

int s21_to_scientific(char *result, long double value, int ccase,
                      s21_specifier param) {
  int exp = 0, flag = 0;
  char *string = result;
  if (result != s21_NULL && !isnan(value) && !isinf(value)) {
    s21_sci_regular(param, result, value, string, &exp, ccase);
  } else if (result != s21_NULL && !isnan(value)) {
    s21_sci_inf(result, value, ccase);
  } else {
    s21_sci_nan(result, ccase);
  }
  return flag;
}
