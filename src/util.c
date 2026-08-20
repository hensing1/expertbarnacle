#include "util.h"

#include <stdlib.h>
#include <string.h>

#include <unicase.h>
#include <uninorm.h>

float clamp(float f, float low, float high) {return max(low, min(f, high));}
float min(float a, float b) {return a < b ? a : b;}
float max(float a, float b) {return a > b ? a : b;}
float sgn(float f) {return f > 0 ? 1 : f == 0 ? 0 : -1;}

int compAlNumCaseInsensitive(const void* a, const void* b) {
    char* s1 = *(char**)a;
    char* s2 = *(char**)b;
    int res;
    u8_casecmp((uint8_t*)s1, strlen(s1), (uint8_t*)s2, strlen(s2), NULL, UNINORM_NFC, &res);
    return res; 
}

void sortAlNumCaseInsensitive(char** strings, size_t count) {
    qsort(strings, count, sizeof(char*), compAlNumCaseInsensitive);
}
