//
// Created by Meitar Basson on 13/05/2023.
//

#ifndef EX1_FILESMANAGER_H
#define EX1_FILESMANAGER_H

#include "stdio.h"
#include "StringOps.h"

void safeFclose(FILE *ptr);
char *readLine(FILE *file);

#endif //EX1_FILESMANAGER_H
