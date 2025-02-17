#include "s21_string.h"

int s21_strnchr(const char *str, int ch) {
  int res = 0;
  if (str) {
    for (int j = 0; str[j] != '\0'; j++) {
      if (str[j] == ch) res++;
    }
  }
  return res;
}

int s21_flag_minus(s21_specifier *param) {
  int end = 0;
  if (param->flags.minus != 1)
    param->flags.minus = 1;
  else
    end = -1;  // 2nd -
  return end;
}
int s21_flag_plus(s21_specifier *param) {
  int end = 0;
  if (param->flags.plus != 1 && param->flags.space != 1)
    param->flags.plus = 1;
  else
    end = -1;  // 2nd +
  return end;
}
int s21_flag_space(s21_specifier *param) {
  int end = 0;
  if (param->flags.plus != 1 && param->flags.space != 1 &&
      param->flags.minus != 1 && param->flags.grid != 1 &&
      param->flags.zero != 1 && param->isSimplePercent != 1)
    param->flags.space = 1;
  else
    end = -1;  // 2nd +
  return end;
}
int s21_flag_simple_percent(s21_specifier *param) {
  int end = 0;
  if (param->flags.plus != 1 && param->flags.space != 1)
    param->isSimplePercent = 1;
  else
    end = -1;  // 2nd +
  return end;
}

int s21_flags_parce(const char *str_tmp, s21_specifier *param) {
  int len = 0, end = 0;
  while (end == 0) {
    switch (str_tmp[len]) {
      case '-':
        end = s21_flag_minus(param);
        len++;
        break;
      case '+':
        end = s21_flag_plus(param);
        len++;
        break;
      case ' ':
        end = s21_flag_space(param);
        len++;
        break;
      case '#':
        if (param->flags.grid != 1)
          param->flags.grid = 1;
        else
          end = -1;  // 2nd #
        len++;
        break;
      case '0':
        if (param->flags.zero != 1)
          param->flags.zero = 1;
        else
          end = -1;  // 2nd 0
        len++;
        break;
      case '%':
        end = s21_flag_simple_percent(param);
        len++;
        break;
      default:
        end = 1;
        break;
    }
  }
  return end == -1 ? -1 : len;
}

int s21_width_parce(const char *str_tmp, const char *params,
                    s21_specifier *param, va_list *ap) {
  int len = 0;
  if (str_tmp[len] == '*') {
    s21_union data;
    s21_data_init(&data);
    s21_read_d(*param, &data, ap);
    if (data.nint > 0)
      param->width =
          data.nint;  // if the value is negative, sp->width must be as default
    len += 1;
  } else {
    char tmp[6000] = {0};
    while (s21_strnchr(params, str_tmp[len]) > 0) len++;
    s21_memcpy(tmp, str_tmp, len);
    tmp[len] = '\0';
    param->width = s21_char_to_int(tmp);
  }
  return len;
}

int s21_precision_parce(const char *str_tmp, char first_param,
                        const char *params, s21_specifier *param, va_list *ap) {
  int len = 0, end = 0;
  if (str_tmp[len] == first_param)
    len++;
  else
    end = 1;
  if (str_tmp[len] == '*' && end == 0) {
    s21_union data;
    s21_data_init(&data);
    s21_read_d(*param, &data, ap);
    if (data.nint > 0) param->precision = data.nint;
    len += 1;
  } else if (end == 0) {
    char tmp[6000] = {0};
    while (s21_strnchr(params, str_tmp[len]) > 0) len++;
    s21_memcpy(tmp, str_tmp + 1, len - 1);
    tmp[len - 1] = '\0';
    param->precision = s21_char_to_int(tmp);
  }
  return len;
}

int s21_size_parce(const char *str_tmp, const char *flags,
                   s21_specifier *param) {
  int len = 0;
  if (s21_strnchr(flags, str_tmp[0]) > 0) {
    param->size = str_tmp[0];
    len++;
  }
  return len;
}

int s21_sp_parce(const char *str_tmp, const char *flags, s21_specifier *param) {
  int len = 0;
  if (s21_strnchr(flags, str_tmp[0]) > 0) {
    param->sp = str_tmp[0];
    len++;
  }
  return len;
}

int s21_parse_specifier(const char *str_tmp, s21_specifier *param,
                        va_list *ap) {
  int len = 0;
  len += s21_flags_parce(str_tmp, param);
  if (param->isSimplePercent != 1 && len != -1) {
    len += s21_width_parce(str_tmp + len, "0123456789", param, ap);
    len += s21_precision_parce(str_tmp + len, '.', "0123456789", param, ap);
    len += s21_size_parce(str_tmp + len, "hlL", param);
    len += s21_sp_parce(str_tmp + len, "cdieEfgGosuxXpn", param);
  }
  if (param->isSimplePercent == 1) {
    len = 1;
  } else if (param->sp == 0 || (param->sp == 'f' && (param->size == 'h')) ||
             (param->sp == 'd' && (param->size == 'L')))
    len = -1;

  return len;
}
