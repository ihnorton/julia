#include "libsupport.h"
#define _GNU_SOURCE
#include <stdlib.h>
#include <locale.h>

#if defined(_OS_WINDOWS_)
#define LOCALE_T _locale_t
#define STRTOD_L _strtod_l
#define STRTOF_L _strtof_l
#else
#include <xlocale.h>
#define LOCALE_T locale_t
#define STRTOD_L strtod_l
#define STRTOF_L strtof_l
#if defined(_OS_LINUX_)
extern double strtod_l(const char *nptr, char **endptr, locale_t loc);
extern float strtof_l(const char *nptr, char **endptr, locale_t loc);
#endif
#endif

// Cache locale object
static int c_locale_initialized = 0;
static LOCALE_T c_locale;
 
LOCALE_T get_c_locale()
{
  if(!c_locale_initialized)
  {
    c_locale_initialized = 1;
#if defined(_OS_WINDOWS_)
    c_locale = _create_locale(LC_ALL,"C");
#else
    c_locale = newlocale(LC_ALL_MASK, NULL, NULL);
#endif
  }
  return c_locale;
}
 
double strtod_c(const char *nptr, char **endptr)
{
  return STRTOD_L(nptr, endptr, get_c_locale());
}

float strtof_c(const char *nptr, char **endptr)
{
  return STRTOF_L(nptr, endptr, get_c_locale());
}

