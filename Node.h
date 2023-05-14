//
// Created by Meitar Basson on 13/05/2023.
//

#ifndef EX1_NODE_H
#define EX1_NODE_H

#define SUCCESS 1
#define FAILURE 0

#include "stdio.h"
#include "stdlib.h"

typedef struct node {
    void *m_data;
    struct node *m_next;

    int m_friends;
    int m_rivals;
} *Node;

Node pushBack(Node start, Node newNode);

#endif //EX1_NODE_H
