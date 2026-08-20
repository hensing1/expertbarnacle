#ifndef INCLUDE_SRC_UTIL_H_
#define INCLUDE_SRC_UTIL_H_

#include <stddef.h>

#include <raylib.h>

float clamp(float f, float low, float high);
float min(float a, float b);
float max(float a, float b);
float sgn(float f);

void sortAlNumCaseInsensitive(char** strings, size_t count);

#endif  // INCLUDE_SRC_UTIL_H_
