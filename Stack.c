//
// Created by Meitar Basson on 13/05/2023.
//

#include "Stack.h"

bool push(stack *s, void *ptr) {
    Node n = (Node) safeAlloc(sizeof(struct node));
    if (n == NULL) {
        return false;
    }
    n->m_data = ptr;
    n->m_next = s->last;
    s->last = n;
    return true;
}

void *pop(stack *s) {
    if (s->last == NULL) {
        return NULL;
    }

    Node tmp = s->last;
    s->last = s->last->m_next;
    void *t = tmp->m_data;
    free(tmp);
    return t;
}