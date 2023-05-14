//
// Created by Meitar Basson on 13/05/2023.
//

#include "StringOps.h"

void *safeAlloc(int size) {
    void * ptr = malloc(size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}


int my_strnlen(const char *s1, char delim) {
    int len = 0;
    for (const char *tmp = s1; *tmp != delim && *tmp != '\0'; ++tmp) {
        if (len == 4096) {
            return -1;
        }
        len++;
    }
    return len;
}

char *my_strdup(const char *s1, char delim) {
    if (s1 == NULL) {
        return NULL;
    }

    int len = my_strnlen(s1, delim);
    if (len == -1) {
        return NULL;
    }
    char * out = safeAlloc(len + 1);
    for (int i = 0; i < len; ++i) {
        out[i] = s1[i];
    }
    out[len] = '\0';
    return out;
}

const char *my_strsep(const char ** s, char delim) {
    if (*s == NULL) {
        return NULL;
    }

    int len = my_strnlen(*s, delim);
    if (len == -1) {
        return NULL;
    }
    const char * orig = *s;
    if (len == 0 || orig[len] == '\0') {
        *s = NULL;
    } else {
        *s = orig + len + 1;
    }
    return orig;
}

bool isNumericString(const char *str) {
    for (int i = 0; i < my_strnlen(str, ' '); i++) {
        if(str[i] == '\n') {
            return true;
        }
        else if (str[i] - '0' > 9 || str[i] - '0' < 0) {
            return false;
        }
    }
    return true;
}