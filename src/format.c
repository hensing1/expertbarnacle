#include "format.h"

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initLocale() {
    setlocale(LC_ALL, "");
}

char* fmtFileSize(size_t numBytes) {
    char* units[] = {"Bytes", "kB", "MB", "GB", "TB", "PB", "EB"};  // exabytes, why not
    int mag = 0;
    float b = numBytes;
    while (b >= 1000 && mag < 7) {
        mag++;
        b /= 1000;
    }
    char* repr = malloc(10);
    if (fabsf(b - nearbyintf(b)) < 0.005) {
        snprintf(repr, 10, "%d %s", (int)nearbyintf(b), units[mag]);
    }
    else {
        snprintf(repr, 10, "%'.2f %s", b, units[mag]);
    }
    return repr;
}

char* fmtDateTime(time_t time) {
    struct tm *localTime = localtime(&time);
    char* str = malloc(64);
    strftime(str, 64, "%x %X", localTime);
    return str;
}
