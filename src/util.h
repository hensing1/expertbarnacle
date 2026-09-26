#ifndef INCLUDE_SRC_UTIL_H_
#define INCLUDE_SRC_UTIL_H_

#include <stddef.h>

#define FREE_PTR(ptr) do{ if(ptr) {free(ptr); ptr = NULL;} } while(0)
#define min(a, b) (a) <= (b) ? (a) : (b)
    // _Generic((a), \
    //     int: _Generic((b), int: intmin, float: floatmin), \
    //     float: floatmin \
    // )(a, b)
#define max(a, b) (a) >= (b) ? (a) : (b)
    // _Generic((a), \
    //     int: _Generic((b), int: intmax, float: floatmax), \
    //     float: floatmax \
    // )(a, b)

float absf(float f);
float clamp(float f, float low, float high);
int intmin(int a, int b);
int intmax(int a, int b);
float floatmin(float a, float b);
float floatmax(float a, float b);
float sgn(float f);

void sortAlNumCaseInsensitive(char** strings, size_t count);

#endif  // INCLUDE_SRC_UTIL_H_
