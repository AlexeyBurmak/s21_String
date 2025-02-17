#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H

#include <stdarg.h>

#define LEN_DECIMAL 200
#define LEN_DOUBLE 6000
#define FLAGS 5
#define DEFAULT_SP 0
#define NULL_STR "nan\0"

typedef struct {
  int minus;
  int plus;
  int space;
  int grid;
  int zero;
} sp_flag;

typedef struct {
  sp_flag flags;
  int width;
  int precision;
  char size;
  char sp;
  int isSimplePercent;
} s21_specifier;

typedef union {
  char string[100000];
  char nchar;
  wchar_t nwchar;
  wchar_t nstringw[100000];
  int nint;
  long int nlongint;
  short int nshortint;
  unsigned int nunsignedint;
  unsigned long int nlongunsignedint;
  unsigned short int nshortunsignedint;
  float nfloat;
  double ndouble;
  long double nlongdouble;
  void *nptr;
} s21_union;

void s21_format_to_base(unsigned long num, int base, const char *base_chars,
                        char *result);
int s21_double_to_char(char res[LEN_DOUBLE], long double num, s21_specifier sp);
int s21_int_to_char(char res[LEN_DECIMAL], long int num, s21_specifier sp);
int s21_char_to_int(const char *str);
int s21_to_scientific(char *result, long double value, int ccase,
                      s21_specifier sp);

int s21_char_to_int(const char *str);
double s21_char_to_double(const char *str);

int s21_parse_integer(const char **str);
void s21_parse_sign(const char **str, int *sign);
int s21_parse_specifier(const char *str_tmp, s21_specifier *sp, va_list *ap);
int s21_flags_parce(const char *str_tmp, s21_specifier *param);
int s21_flag_space(s21_specifier *param);
int s21_flag_simple_percent(s21_specifier *param);

void s21_read_by_type(s21_specifier param, s21_union *data, va_list *ap);
void s21_read_d(s21_specifier param, s21_union *data, va_list *ap);
void s21_buffer_by_type(s21_specifier param, s21_union *data,
                        char *bufferstring);
void s21_sci_inf(char *result, long double value, int ccase);
void s21_sci_nan(char *result, int ccase);
int s21_flag_plus(s21_specifier *param);

void s21_data_init(s21_union *data);
int s21_sprintf(char *str, const char *format, ...);
void s21_add_sign(char *str, char chr);
void s21_fraction_length(char *str, int p);
void s21_strnshift(char *str, int shift);
void s21_sp_init(s21_specifier *sp);
#endif