#include "s21_string.h"

void s21_strnshift(char *str, int shift) {
  int i = 0;
  if (str) {
    for (int j = shift; str[j] != '\0'; j++, i++) {
      str[i] = str[j];
    }
  }
  str[i] = '\0';
}

void s21_sp_init(s21_specifier *sp) {
  sp->flags.minus = 0;
  sp->flags.plus = 0;
  sp->flags.space = 0;
  sp->flags.grid = 0;
  sp->flags.zero = 0;
  sp->size = 0;
  sp->precision = -1;
  sp->width = DEFAULT_SP;
  sp->sp = '\0';
  sp->isSimplePercent = 0;
}

void s21_data_init(s21_union *data) {
  data->nchar = '\0';
  data->nint = 0;
  data->nunsignedint = 0;
  data->nlongint = 0;
  data->nlongunsignedint = 0;
  data->nshortint = 0;
  data->nshortunsignedint = 0;
  data->ndouble = 0.0;
  data->nlongdouble = 0.0;
  for (int i = 0; i < 100000; i++) {
    data->string[i] = '0';
  }
  data->string[99999] = '\0';
}

void s21_add_sign(char *str, char chr) {
  int length = (int)s21_strlen(str);
  for (int i = length; i >= 0; i--) {
    str[i + 1] = str[i];
  }
  str[length + 1] = '\0';
  str[0] = chr;
}

void s21_fraction_length(char *bufferstring, int p) {
  int dotindex = 0;
  int strlength = (int)s21_strlen(bufferstring);
  for (int i = 0; bufferstring[i] != '.'; i++) {
    dotindex++;
  }
  int fractionlen = strlength - dotindex - 1;

  if (p <= fractionlen) {
    bufferstring[dotindex + p + 1] = '\0';
  } else {
    for (int i = 0; i < p - fractionlen; i++) {
      bufferstring[strlength + i] = '0';
      bufferstring[strlength + i + 1] = '\0';
    }
  }
}

void s21_sign_to_left(char *bufferstring, char *outputstr) {
  if (bufferstring[0] == '-' || bufferstring[0] == '+' ||
      bufferstring[0] == ' ') {
    char sign = bufferstring[0];
    s21_size_t length = s21_strlen(outputstr);
    outputstr[0] = sign;
    for (s21_size_t i = 1; i < length; i++) {
      if (outputstr[i] == sign) {
        outputstr[i] = '0';
      }
    }
  }
}

void s21_format_precision(s21_specifier param, char *bufferstring) {
  if (param.precision == -1) {
    s21_fraction_length(bufferstring, 6);
  } else if (param.precision >= 0) {
    s21_fraction_length(bufferstring, param.precision);
  }
}

void s21_format_flag_plus(s21_specifier param, char *bufferstring) {
  if ((param.sp != 's') & (param.sp != 'c')) {
    if ((param.flags.plus) & (bufferstring[0] != '-')) {
      s21_add_sign(bufferstring, '+');
    }
    if ((param.flags.space) & (bufferstring[0] != '-')) {
      s21_add_sign(bufferstring, ' ');
    }
  }
}

void s21_format_width(s21_specifier param, char *bufferstring, int *length) {
  if (param.width == 0 || param.width < (int)s21_strlen(bufferstring)) {
    *length = (int)s21_strlen(bufferstring);
  } else {
    *length = param.width;
  }
}

void s21_format_remove_dot(char *outputstr) {
  int len = (int)s21_strlen(outputstr);
  char *posline = s21_strchr(outputstr, '.');
  int pos = len - (int)s21_strlen(posline);
  for (int i = pos; i < len; i++) {
    outputstr[i] = outputstr[i + 1];
  }
  outputstr[len - 1] = '\0';
}

void s21_format_fill(s21_specifier param, char *bufferstring, int length,
                     char *c, int i, int *readcounter) {
  int localwidth = (int)s21_strlen(bufferstring);
  int dotshift = 0;
  if ((param.precision == 0) & (param.sp == 'f') & (param.flags.grid == 0))
    dotshift = 1;

  if (param.flags.minus) {
    if (i < (localwidth - dotshift)) {
      *c = bufferstring[*readcounter];
      *readcounter += 1;
    } else if (param.flags.zero) {
      *c = '0';
    } else {
      *c = ' ';
    }
  } else {
    if (i >= (length - localwidth + dotshift)) {
      *c = bufferstring[*readcounter];
      *readcounter += 1;
    } else if (param.flags.zero) {
      *c = '0';
    } else {
      *c = ' ';
    }
  }
}

int s21_string_format(s21_specifier param, char *outputstr, s21_union data) {
  int length;
  int counter = 0;
  int readcounter = 0;
  char c;
  char bufferstring[256000] = {0};

  s21_buffer_by_type(param, &data, bufferstring);
  if (bufferstring[0] == '\0') {
    s21_sp_init(&param);
    counter = -1;
  }

  s21_format_flag_plus(param, bufferstring);
  if (param.sp == 'f') s21_format_precision(param, bufferstring);
  s21_format_width(param, bufferstring, &length);

  for (int i = 0; i < length; i++) {
    s21_format_fill(param, bufferstring, length, &c, i, &readcounter);
    outputstr[i] = c;
    counter++;
  }

  outputstr[length] = '\0';
  if (param.flags.zero & (param.sp != 'u'))
    s21_sign_to_left(bufferstring, outputstr);
  if ((param.precision == 0) & (param.flags.grid == 0) & (param.sp == 'e'))
    s21_format_remove_dot(outputstr);
  return counter;
}

int s21_simple_percent(s21_specifier param, s21_union *data, va_list *ap,
                       char *str, char *bufferstring, char *stringtoprint,
                       int res, int *countertotal) {
  int exitflag = 0;
  if (param.isSimplePercent != 1 && res == -1) {
    s21_strncpy(str, "\0", 1);
    countertotal = 0;
    exitflag = 1;
  } else if (param.isSimplePercent != 1) {
    s21_strnshift(bufferstring, res - 1);
    s21_read_by_type(param, data, ap);
    s21_string_format(param, stringtoprint, *data);
  }
  if (param.isSimplePercent == 1) {
    stringtoprint[0] = '%';
    stringtoprint[1] = '\0';
  } else if (stringtoprint[0] == '\0') {
    s21_strncpy(stringtoprint, NULL_STR, s21_strlen(NULL_STR));
  }
  countertotal = countertotal;
  return exitflag;
}

int s21_sprintf(char *str, const char *format, ...) {
  int countertotal = 0;
  int checkflag = 0;
  char bufferstring[100000] = {0};
  char stringtoprint[100000] = {0};
  va_list ap;
  va_start(ap, format);
  for (int i = 0; format[i] != '\0'; i++) {
    bufferstring[i] = format[i];
    bufferstring[i + 1] = '\0';
  }
  while (s21_strlen(bufferstring) > 0) {
    int res = 0;
    int i = 0;
    if (bufferstring[i] != '%') {
      str[countertotal] = bufferstring[i];
      str[countertotal + 1] = '\0';
      s21_strnshift(bufferstring, 1);
      countertotal++;
    } else {
      s21_specifier param;
      s21_union data;
      s21_sp_init(&param);
      s21_data_init(&data);
      s21_strnshift(bufferstring, 1);
      res = s21_parse_specifier(bufferstring, &param, &ap);
      checkflag = s21_simple_percent(param, &data, &ap, str, bufferstring,
                                     stringtoprint, res, &countertotal);
      if (!checkflag) {
        for (int j = 0; stringtoprint[j] != '\0'; j++) {
          str[countertotal] = stringtoprint[j];
          str[countertotal + 1] = '\0';
          countertotal++;
        }
        i += res;
        s21_strnshift(bufferstring, 1);
      }
    }
    i++;
  }  // End of loop
  va_end(ap);
  return countertotal;
}
