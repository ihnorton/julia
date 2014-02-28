#include <locale.h>
#include "libsupport.h"

double D_PNAN;
double D_NNAN;
double D_PINF;
double D_NINF;

DLLEXPORT int locale_is_utf8;
static int isInitialized = 0;

void libsupport_init(void)
{
    if (!isInitialized) {
        locale_is_utf8 = u8_is_locale_utf8(setlocale(LC_ALL, ""));
        setlocale(LC_NUMERIC, "C");

        ios_init_stdstreams();

        D_PNAN = strtod_c("+NaN",NULL);
        D_NNAN = -strtod_c("+NaN",NULL);
        D_PINF = strtod_c("+Inf",NULL);
        D_NINF = strtod_c("-Inf",NULL);
        isInitialized=1;
    }
}
