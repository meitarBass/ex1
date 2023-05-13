//
// Created by Meitar Basson on 13/05/2023.
//

#ifndef EX1_LINKEDLIST_H
#define EX1_LINKEDLIST_H

#include "Node.h"
#include "stdbool.h"

typedef int (*ComparisonFunction)(void*,void*);

typedef struct linkedList {
    Node m_first;
    Node m_last;
    int m_size;
} *LinkedList;


void freeList(LinkedList list);
void insert(LinkedList list, Node location, Node newNode);
void *dequeueFirstItem(LinkedList list);
bool listContains(LinkedList list, void *item, ComparisonFunction comparisonFunction);


#endif //EX1_LINKEDLIST_H
