#ifndef INCLUDE_SRC_FORMAT_H_
#define INCLUDE_SRC_FORMAT_H_

#include <stddef.h>
#include <time.h>

#include "lib/clay.h"

#include "io.h"

typedef enum {
    LANG_DE,
} Languages;

typedef enum {
    STR_RESOLUTION_TITLE,
    STR_RESOLUTION,
    STR_FILESIZE_TITLE,
    STR_FILESIZE,
    STR_TIME_MODIFIED_TITLE,
    STR_TIME_MODIFIED,
    NUM_APPL_STRINGS
} ApplicationStrings;

void initLocale();

Clay_String* makeApplicationStrings(Languages lang, ImageInfo img);

Clay_String mkClayString(const char* s);
Clay_String fmtClayString(const char* format, ...);
char* fmtDateTime(time_t time);
char* fmtFileSize(size_t numBytes);

#endif  // INCLUDE_SRC_FORMAT_H_
