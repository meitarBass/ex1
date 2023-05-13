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
} *Node;

Node pushBack(Node start, Node newNode) {
    if (start == NULL) {
        return newNode;
    }
    Node tmp = start;
    while (tmp->m_next != NULL) {
        tmp = tmp->m_next;
    }
    tmp->m_next = newNode;
    return start;
}

#endif //EX1_NODE_H
