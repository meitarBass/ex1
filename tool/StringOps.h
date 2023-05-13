//
// Created by Meitar Basson on 13/05/2023.
//

#ifndef EX1_STRINGOPS_H
#define EX1_STRINGOPS_H

#include "string.h"
#include "stdlib.h"
#include "stdbool.h"

const char *my_strsep(const char ** s, char delim);
char *my_strdup(const char *s1, char delim);
int my_strnlen(const char *s1, char delim);
void *safeAlloc(int size);

bool isNumericString(const char *str);

#endif //EX1_STRINGOPS_H
