#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unicase.h>
#include <raylib.h>

#include "src/util.h"

int cmp(const void* a, const void* b) {
    char* s1 = *(char**)a;
    char* s2 = *(char**)b;
    int res;
    u8_casecmp(s1, strlen(s1), s2, strlen(s2), NULL, NULL, &res);
    return res;
}

int main(void) {
    uint8_t* s1 = (uint8_t*)"aaa";
    uint8_t* s2 = (uint8_t*)"BBB";
    int res;
    
    FilePathList l;
    char* s[3] = {"zzz", (char*)s2, (char*)s1};
    l.paths = s;
    l.count = 3;

    // sortAlNumCaseInsensitive(l);
    qsort(s, 3, sizeof(char*), cmp);

    printf("%s\n", s[0]);
    printf("%s\n", s[1]);
    printf("%s\n", s[2]);
    // u8_casecmp(s1, strlen((char*)s1), s2, strlen((char*)s2), NULL, NULL, &res);
    // printf("%i\n", res);
}
