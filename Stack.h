//
// Created by Meitar Basson on 13/05/2023.
//

#ifndef EX1_STACK_H
#define EX1_STACK_H

#include "Node.h"
#include "tool/StringOps.h"

typedef struct {
    Node last;
} stack;


bool push(stack *s, void *ptr);
void *pop(stack *s);;

#endif //EX1_STACK_H
