//
// Created by Meitar Basson on 13/05/2023.
//

#include "FilesManager.h"

void safeFclose(FILE *ptr) {
    if (ptr != NULL) {
        fclose(ptr);
    }
}

char *readLine(FILE *file) {
    char buf[4096] = {0};
    if (NULL == fgets(buf, sizeof(buf), file)) {
        int a = feof(file);
        if (a == 1) {
            return "\0";
        }
        return NULL;
    }
    int len = (int) my_strnlen(buf, '\0');
    char *str = safeAlloc(len + 1);
    if (str == NULL) {
        return NULL;
    }
    str = strncpy(str, buf, len);
    for (int i = 0; i < len; ++i) {
        if (str[i] == '\n') {
            str[i] = '\0';
        }
    }
    return str;
}