#include "util.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unicase.h>

float absf(float f) {
    float negZero = -0.f;
    int abs = *(int*)&f & ~*(int*)&negZero; // nothing to see here
    return *(float*)&abs;
}
float clamp(float f, float low, float high) {return max(low, min(f, high));}
int intmin(int a, int b) {return a < b ? a : b;}
int intmax(int a, int b) {return a > b ? a : b;}
float floatmin(float a, float b) {return a < b ? a : b;}
float floatmax(float a, float b) {return a > b ? a : b;}
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

inline Rectangle toRectangle(Clay_BoundingBox box) {
    return (Rectangle) { .x = box.x, .y = box.y, .width = box.width, .height = box.height };
}

inline Color toColor(Clay_Color color) {
    return (Color) { .r = (unsigned char)roundf(color.r), .g = (unsigned char)roundf(color.g), .b = (unsigned char)roundf(color.b), .a = (unsigned char)roundf(color.a) };
}

