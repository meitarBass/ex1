//
// Created by Meitar Basson on 14/05/2023.
//


#include "Node.h"

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
