//
// Created by Meitar Basson on 13/05/2023.
//

#include "LinkedList.h"

#define COMPARISON_EQUALS 1
#define COMPARISON_NOT_EQUALS 0


void freeList(LinkedList list) {
    while(list->m_first != NULL) {
        Node toFree = list->m_first;
        list->m_first = list->m_first->m_next;
        free(toFree);
    }
    free(list);
}

void insert(LinkedList q, Node location, Node newNode) {
    // Insert item at location
    newNode->m_next = location->m_next;
    location->m_next = newNode;
    q->m_size++;
}

void *dequeueFirstItem(LinkedList list) {
    if(list == NULL || list->m_first == list->m_last) {
        return NULL;
    }
    Node tmp = list->m_first;
    list->m_first = list->m_first->m_next;

    void *data = list->m_first->m_data;
    free(tmp);
    tmp = NULL;
    list->m_size--;
    return data;
}

bool listContains(LinkedList list, void *item, ComparisonFunction comparisonFunction) {
    if (list == NULL || item == NULL) {
        return false;
    }

    Node tmp = list->m_first->m_next;
    while (tmp != NULL) {
        if (comparisonFunction(tmp->m_data, item) == COMPARISON_EQUALS) {
            return true;
        }
        tmp = tmp->m_next;
    }
    return false;
}
