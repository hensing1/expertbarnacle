#ifndef INCLUDE_SRC_FORMAT_H_
#define INCLUDE_SRC_FORMAT_H_

#include <stddef.h>
#include <time.h>

void initLocale();
char* fmtDateTime(time_t time);
char* fmtFileSize(size_t numBytes);

#endif  // INCLUDE_SRC_FORMAT_H_
