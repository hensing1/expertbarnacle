#include "format.h"

#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void initLocale() {
    setlocale(LC_ALL, "");
}

Clay_String* makeImageStrings(Arena* imgArena, Languages lang, ImageMetadata img) {
    Clay_String* s = arena_alloc(imgArena, sizeof(Clay_String) * NUM_APPL_STRINGS);
    switch(lang) {
    case LANG_DE:
        s[STR_RESOLUTION_TITLE] = CLAY_STRING("Auflösung");
        s[STR_RESOLUTION] = fmtClayString(imgArena, "%d x %d", img.width, img.height);
        s[STR_FILESIZE_TITLE] = CLAY_STRING("Dateigröße");
        s[STR_FILESIZE] = mkClayString(fmtFileSize(imgArena, img.sizeBytes));
        s[STR_TIME_MODIFIED_TITLE] = CLAY_STRING("Datei geändert");
        s[STR_TIME_MODIFIED] = mkClayString(fmtDateTime(imgArena, img.timeModifiedUnix));
    }
    return s;
}

void freeApplicationStrings(Clay_String* s) {
    for (size_t i = 0; i < NUM_APPL_STRINGS; i++) {
        if (!s[i].isStaticallyAllocated) {free((void*)s[i].chars);}
    }
    free(s);
}

Clay_String mkClayString(const char *s) {
    return (Clay_String) {
        .chars = s,
        .isStaticallyAllocated = false,
        .length = strlen(s)
    };
}

Clay_String fmtClayString(Arena* arena, const char* format, ...) {
    va_list args;
    va_start(args, format);
    size_t n = vsnprintf(0, 0, format, args);
    va_end(args);
    char* buf = arena_alloc(arena, n+1);
    va_start(args, format);
    vsnprintf(buf, n+1, format, args);
    return (Clay_String) {
        .chars = buf,
        .isStaticallyAllocated = false,
        .length = n
    };
}

char* fmtFileSize(Arena* arena, size_t numBytes) {
    char* units[] = {"Bytes", "kB", "MB", "GB", "TB", "PB", "EB"};  // exabytes, why not
    int mag = 0;
    float b = numBytes;
    while (b >= 1000 && mag < 7) {
        mag++;
        b /= 1000;
    }
    char* repr = arena_alloc(arena, 10);
    if (fabsf(b - nearbyintf(b)) < 0.005) {
        snprintf(repr, 10, "%d %s", (int)nearbyintf(b), units[mag]);
    }
    else {
        snprintf(repr, 10, "%'.2f %s", b, units[mag]);
    }
    return repr;
}

char* fmtDateTime(Arena* arena, time_t time) {
    struct tm *localTime = localtime(&time);
    char* str = arena_alloc(arena, 64);
    strftime(str, 64, "%x %X", localTime);
    return str;
}
