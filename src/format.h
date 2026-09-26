#ifndef INCLUDE_SRC_FORMAT_H_
#define INCLUDE_SRC_FORMAT_H_

#include <stddef.h>
#include <time.h>

#include "lib/arena.h"
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

Clay_String* makeImageStrings(Arena* imgArena, Languages lang, ImageMetadata img);
void freeApplicationStrings(Clay_String* s);

/** A function for wrapping dynamically allocated strings to be used in the UI */
Clay_String mkClayString(const char* s);

Clay_String fmtClayString(Arena* arena, const char* format, ...);
char* fmtDateTime(Arena* arena, time_t time);
char* fmtFileSize(Arena* arena, size_t numBytes);

#endif  // INCLUDE_SRC_FORMAT_H_
