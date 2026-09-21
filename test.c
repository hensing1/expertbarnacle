#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unicase.h>
#include <raylib.h>

#include "src/format.h"
// #include "src/util.h"

int main(void) {
    initLocale();
    char* f = fmtFileSize(112);
    printf("%s\n", f);
    free(f);
    return 0;
}
