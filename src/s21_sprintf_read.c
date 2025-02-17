#include <math.h>

#include "s21_string.h"

void s21_read_d(s21_specifier param, s21_union *data, va_list *ap) {
  if (param.size == 'l')
    data->nlongint = va_arg(*ap, long int);
  else if (param.size == 'h') {
    short int tempshort;
    int tempreg;
    tempreg = va_arg(*ap, int);
    tempshort = (short int)tempreg;
    data->nshortint = tempshort;
  } else
    data->nint = va_arg(*ap, int);
}

void s21_read_u(s21_specifier param, s21_union *data, va_list *ap) {
  if (param.size == 'l')
    data->nlongunsignedint = va_arg(*ap, unsigned long int);
  else if (param.size == 'h') {
    unsigned short int tempshort;
    unsigned int tempreg;
    tempreg = va_arg(*ap, unsigned int);
    tempshort = (unsigned short int)tempreg;
    data->nshortunsignedint = tempshort;
  } else
    data->nunsignedint = va_arg(*ap, unsigned int);
}

void s21_read_f(s21_specifier param, s21_union *data, va_list *ap) {
  if ((param.size == 'L')) {
    data->nlongdouble = va_arg(*ap, long double);
  } else if (param.size == 0) {
    float tempfloat;
    double tempdouble;
    tempdouble = va_arg(*ap, double);
    tempfloat = (float)tempdouble;
    data->nfloat = tempfloat;
  } else if (param.size == 'l') {
    data->ndouble = va_arg(*ap, double);
  }
}

void s21_read_c(s21_specifier param, s21_union *data, va_list *ap) {
  if (param.size == 'l') {
    data->nwchar = va_arg(*ap, wchar_t);
  } else
    data->nchar = va_arg(*ap, int);
}

void s21_read_s(s21_specifier param, s21_union *data, va_list *ap) {
  if (param.size == 'l') {
    char tmp[100000];
    wchar_t *tmpstringw = va_arg(*ap, wchar_t *);
    for (int i = 0; i < (int)s21_strlen((char *)tmpstringw); i++) {
      tmp[i] = tmpstringw[i];
      tmp[i + 1] = '\0';
    }
    s21_strncpy(data->string, tmp, 100000);
  } else {
    char *tmpstring = va_arg(*ap, char *);
    s21_strncpy(data->string, tmpstring, 100000);
  }
}

void s21_read_p(s21_union *data, va_list *ap) {
  data->nptr = va_arg(*ap, void *);
}

void s21_read_by_type(s21_specifier param, s21_union *data, va_list *ap) {
  switch (param.sp) {
    case 's':
      s21_read_s(param, data, ap);
      break;
    case 'c':
      s21_read_c(param, data, ap);
      break;
    case 'd':
      s21_read_d(param, data, ap);
      break;
    case 'u':
      s21_read_u(param, data, ap);
      break;
    case 'f':
      s21_read_f(param, data, ap);
      break;
    case 'x':
      s21_read_u(param, data, ap);
      break;
    case 'X':
      s21_read_u(param, data, ap);
      break;
    case 'o':
      s21_read_u(param, data, ap);
      break;
    case 'p':
      s21_read_p(data, ap);
      break;
    case 'e':
      s21_read_f(param, data, ap);
      break;
    case 'E':
      s21_read_f(param, data, ap);
      break;
    case 'i':
      s21_read_d(param, data, ap);
      break;
  }
}

void s21_buffer_s(s21_union *data, char *bufferstring) {
  for (int i = 0; i < (int)s21_strlen(data->string); i++)
    bufferstring[i] = data->string[i];
  bufferstring[s21_strlen(data->string)] = '\0';
}

void s21_buffer_c(s21_union *data, char *bufferstring) {
  bufferstring[0] = data->nchar;
  bufferstring[1] = '\0';
}

void s21_buffer_d(s21_specifier param, s21_union *data, char *bufferstring) {
  if (param.size == 'l')
    s21_int_to_char(bufferstring, data->nlongint, param);
  else if (param.size == 'h')
    s21_int_to_char(bufferstring, data->nshortint, param);
  else
    s21_int_to_char(bufferstring, data->nint, param);
}

void s21_buffer_f(s21_specifier param, s21_union *data, char *bufferstring) {
  if (param.size == 'l') {
    s21_double_to_char(bufferstring, data->ndouble, param);
  } else if (param.size == 'L') {
    s21_double_to_char(bufferstring, data->nlongdouble, param);
  } else {
    s21_double_to_char(bufferstring, data->nfloat, param);
  }
}

void s21_buffer_u(s21_specifier param, s21_union *data, char *bufferstring) {
  if (param.size == 'l') {
    s21_int_to_char(bufferstring, data->nlongunsignedint, param);
  } else if (param.size == 'h') {
    s21_int_to_char(bufferstring, data->nshortunsignedint, param);
  } else {
    s21_int_to_char(bufferstring, data->nunsignedint, param);
  }
}

void s21_buffer_xo(s21_specifier param, s21_union *data, char *bufferstring) {
  char basestring[16] = "0123456789ABCDEF";
  int base = 16;
  int gridshift = 0;
  if (param.sp == 'x')
    s21_memcpy(basestring, "0123456789abcdef", 16);
  else if (param.sp == 'o') {
    s21_memcpy(basestring, "01234567\0", 9);
    base = 8;
  }

  if (param.flags.grid && data->nunsignedint != 0) {
    bufferstring[0] = '0';
    gridshift++;
    if (param.sp == 'x' || param.sp == 'X') {
      bufferstring[1] = param.sp;
      bufferstring[2] = '\0';
      gridshift++;
    }
  }

  if (param.size == 'l')
    s21_format_to_base(data->nlongunsignedint, base, basestring,
                       bufferstring + gridshift);
  else if (param.size == 'h')
    s21_format_to_base(data->nshortunsignedint, base, basestring,
                       bufferstring + gridshift);
  else
    s21_format_to_base(data->nunsignedint, base, basestring,
                       bufferstring + gridshift);
}

void s21_buffer_p(s21_union *data, char *bufferstring) {
  if (data->nptr == s21_NULL) {
    s21_strncpy(bufferstring, "(nil)", 5);
    bufferstring[5] = '\0';
  } else {
    bufferstring[0] = '0';
    bufferstring[1] = 'x';
    bufferstring[2] = '\0';
    s21_format_to_base((unsigned long)data->nptr, 16, "0123456789abcdef",
                       bufferstring + 2);
  }
}

void s21_buffer_eE(s21_specifier param, s21_union *data, char *bufferstring) {
  int ccase = 0;
  if (param.sp == 'E' || param.sp == 'G') ccase = 1;
  if (param.size == 'l') {
    s21_to_scientific(bufferstring, data->ndouble, ccase, param);
  } else if (param.size == 'L') {
    s21_to_scientific(bufferstring, data->nlongdouble, ccase, param);
  } else {
    s21_to_scientific(bufferstring, data->nfloat, ccase, param);
  }
}

void s21_buffer_by_type(s21_specifier param, s21_union *data,
                        char *bufferstring) {
  switch (param.sp) {
    case 's':  // char*
      s21_buffer_s(data, bufferstring);
      break;
    case 'c':  // char
      s21_buffer_c(data, bufferstring);
      break;
    case 'f':  // float
      s21_buffer_f(param, data, bufferstring);
      break;
    case 'd':  // int
      s21_buffer_d(param, data, bufferstring);
      break;
    case 'u':  // unsigned int
      s21_buffer_u(param, data, bufferstring);
      break;
    case 'x':  // hex low
      s21_buffer_xo(param, data, bufferstring);
      break;
    case 'X':  // hex high
      s21_buffer_xo(param, data, bufferstring);
      break;
    case 'o':  // oct
      s21_buffer_xo(param, data, bufferstring);
      break;
    case 'p':  // oct
      s21_buffer_p(data, bufferstring);
      break;
    case 'e':  // science notation
      s21_buffer_eE(param, data, bufferstring);
      break;
    case 'E':  // Science Notation
      s21_buffer_eE(param, data, bufferstring);
      break;
    case 'i':  // i
      s21_buffer_d(param, data, bufferstring);
      break;
  }
}