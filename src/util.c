#include "util.h"

float clamp(float f, float low, float high) {return max(low, min(f, high));}
float min(float a, float b) {return a < b ? a : b;}
float max(float a, float b) {return a > b ? a : b;}
float sgn(float f) {return f > 0 ? 1 : f == 0 ? 0 : -1;}
