//
// Created by Meitar Basson on 13/05/2023.
//

#include "tool/StringOps.h"
#include "IsraeliQueue.h"
#include "Student.h"
#include "Stack.h"
#include "math.h"

#define COMPARISON_EQUALS 1
#define COMPARISON_NOT_EQUALS 0

struct IsraeliQueue_t {
    int m_rivalry;
    int m_friendship;
    ComparisonFunction comparisonFunction;
    FriendshipFunction *friendshipFunction;
    LinkedList list;
};


Node findInsertionLocation(IsraeliQueue q, void *item);
IsraeliQueueError expandFriendshipArray(IsraeliQueue q, FriendshipFunction newFunc);
void *copyNullTerminatedArr(const void **nullTerminatedArr);
int IsraeliQueueGetPosition(IsraeliQueue q, void *item);
int calculateGeoFriendship(IsraeliQueue *q);
int calculateGeoRivalry(IsraeliQueue *q);
void addFunctions(IsraeliQueue *oldQueue, IsraeliQueue *newQueue);
void checkForMoreBlocks(IsraeliQueue q, Node tmp, void *item, stack s);
double getFriendshipAverage(IsraeliQueue q, void *item1, void *item2);
bool checkIfFriends(IsraeliQueue q, Node itemNode, void *item2);
bool checkIfRivals(IsraeliQueue q, void *item1, void *item2);



IsraeliQueue IsraeliQueueCreate(FriendshipFunction *friendshipFunction,
                                ComparisonFunction comparisonFunction, int friendship, int rivalry) {

    IsraeliQueue queue = (IsraeliQueue) safeAlloc(sizeof(struct IsraeliQueue_t));
    if (queue == NULL) {
        return NULL;
    }

    LinkedList list = (LinkedList) safeAlloc(sizeof(struct linkedList));
    if (list == NULL) {
        free(queue);
        return NULL;
    }

    queue->list = list;
    queue->friendshipFunction = copyNullTerminatedArr((const void **) friendshipFunction);
    if (friendshipFunction == NULL) {
        IsraeliQueueDestroy(queue);
        return NULL;
    }

    queue->comparisonFunction = comparisonFunction;
    queue->m_friendship = friendship;
    queue->m_rivalry = rivalry;
    return queue;
}
IsraeliQueue IsraeliQueueClone(IsraeliQueue other) {
    if (other == NULL) {
        return NULL;
    }

    IsraeliQueue queue = IsraeliQueueCreate(other->friendshipFunction,
                                            other->comparisonFunction, other->m_friendship,
                                            other->m_rivalry);
    if (queue == NULL) {
        return NULL;
    }

    for (Node tmp = other->list->m_first->m_next; tmp != NULL; tmp = tmp->m_next) {
        Node copy = (Node) safeAlloc(sizeof(struct node));
        if (copy == NULL) {
            IsraeliQueueDestroy(queue);
            return NULL;
        }
        insert(queue->list, queue->list->m_last, copy);
        queue->list->m_last = queue->list->m_last->m_next;
    }
    return queue;
}
void IsraeliQueueDestroy(IsraeliQueue q) {
    if (q == NULL) {
        return;
    }

    // Free the linked list
    freeList(q->list);
    free(q->friendshipFunction);

    q->list->m_first = NULL;
    q->list->m_last = NULL;
    q->friendshipFunction = NULL;
    free(q);
}
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void *item) {
    if (q == NULL || item == NULL) {
        return ISRAELIQUEUE_BAD_PARAM;
    }

    Node newNode = (Node) safeAlloc(sizeof(struct node));
    if (newNode == NULL) {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    Node location = findInsertionLocation(q, item);
    newNode->m_data = item;

    insert(q->list, location, newNode);
    if (location == q->list->m_last) {
        q->list->m_last = q->list->m_last->m_next;
    }

    return ISRAELIQUEUE_SUCCESS;
}
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction func) {
    if (q == NULL || func == NULL) {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    return expandFriendshipArray(q, func);
}
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int friendThreshold) {
    if (q == NULL) {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q->m_friendship = friendThreshold;
    return ISRAELIQUEUE_SUCCESS;
}
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int rivalThreshold) {
    if (q == NULL) {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q->m_rivalry = rivalThreshold;
    return ISRAELIQUEUE_SUCCESS;
}
int IsraeliQueueSize(IsraeliQueue q) {
    if (q == NULL || q->list == NULL) {
        return 0;
    }
    return q->list->m_size;
}
void *IsraeliQueueDequeue(IsraeliQueue q) {
    return dequeueFirstItem(q->list);
}
bool IsraeliQueueContains(IsraeliQueue q, void *item) {
    return listContains(q->list, item, q->comparisonFunction);
}
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q) {
    if (q == NULL) {
        return ISRAELIQUEUE_BAD_PARAM;
    }

    stack s;
    Node tmp = q->list->m_first;
    while (tmp->m_next != NULL) {
        if (!push(&s, tmp)) {
            while (pop(&s) != NULL) {
                // pop all
            }
            return ISRAELIQUEUE_ALLOC_FAILED;
        }
        tmp = tmp->m_next;
    }
    while ((tmp = pop(&s)) != NULL) {
        Node toImprove = tmp->m_next;
        tmp->m_next = toImprove->m_next;
        IsraeliQueueError err = IsraeliQueueEnqueue(q, toImprove->m_data);
        if (err != ISRAELIQUEUE_SUCCESS) {
            tmp->m_next = toImprove;
            while (pop(&s) != NULL) {
                // pop all
            }
            return err;
        }
    }

    return ISRAELIQUEUE_SUCCESS;
}
IsraeliQueue IsraeliQueueMerge(IsraeliQueue *q, ComparisonFunction comparisonFunction) {
    if (q == NULL) {
        return NULL;
    }

    IsraeliQueue newQueue = (IsraeliQueue) safeAlloc(sizeof(struct IsraeliQueue_t));
    if (newQueue == NULL) {
        return NULL;
    }

    //TODO: Add functions
    newQueue->comparisonFunction = comparisonFunction;
    newQueue->m_friendship = calculateGeoFriendship(q);
    newQueue->m_rivalry = calculateGeoRivalry(q);
    addFunctions(q, &newQueue);

    int numOfQueues = 0, numOfEmptyQueues = 0;
    while (*(q + numOfQueues) != NULL) {
        numOfQueues++;
    }

    while (numOfEmptyQueues != numOfQueues) {
        numOfEmptyQueues = 0;
        IsraeliQueue *tmp = q;
        while (tmp != NULL) {
            IsraeliQueueEnqueue(*tmp, IsraeliQueueDequeue(*tmp));
            if ((*tmp)->list->m_last == (*tmp)->list->m_first) {
                numOfQueues++;
            }
            tmp++;
        }
    }
    IsraeliQueueDestroy(*q);
    return newQueue;
}



Node findInsertionLocation(IsraeliQueue q, void *item) {
    stack s;
    memset(&s, 0, sizeof(stack));
    Node tmp = q->list->m_first->m_next;

    while (tmp != NULL) {
        if (!push(&s, tmp)) {
            while (pop(&s) != NULL) {
                // pop all
            }
            return NULL;
        }
        tmp = tmp->m_next;
    }

    Node lastValidInsertionLocation = q->list->m_last;
    Student lastValid = (Student)(tmp->m_data);
    Student student = (Student) (item);
    if (student->m_isHacker) {
        bool alreadyFoundLocation = false;
        while ((tmp = pop(&s)) != NULL) {
            Student tmpStudent = (Student)(tmp->m_data);
            if (tmpStudent->m_numOfFriends < FRIEND_QUOTA && checkIfFriends(q, tmp, item)) {
                if (alreadyFoundLocation) {
                    lastValid->m_numOfFriends --;
                }
                alreadyFoundLocation = true;
                lastValidInsertionLocation = tmp;
            } else if (tmpStudent->m_numOfRivals < RIVAL_QUOTA && checkIfRivals(q, tmp->m_data, item)) {
                tmpStudent->m_numOfRivals++;
                checkForMoreBlocks(q, tmp, item, s);
                break;
            }
        }
        lastValid->m_numOfFriends++;
    }
    return lastValidInsertionLocation;
}
IsraeliQueueError expandFriendshipArray(IsraeliQueue q, FriendshipFunction newFunc)  {
    int functionsAmount = 0;
    FriendshipFunction *func = q->friendshipFunction;

    while (*func != NULL) {
        functionsAmount++;
        func++;
    }

    FriendshipFunction *friendshipFunctions = (FriendshipFunction *) safeAlloc(
            sizeof(FriendshipFunction) * (functionsAmount + 2));

    if (friendshipFunctions == NULL) {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    memcpy(friendshipFunctions, q->friendshipFunction, sizeof(FriendshipFunction) * (functionsAmount));
    friendshipFunctions[functionsAmount] = newFunc;
    friendshipFunctions[functionsAmount + 1] = NULL;

    free(q->friendshipFunction);
    q->friendshipFunction = friendshipFunctions;

    return ISRAELIQUEUE_SUCCESS;
}
void *copyNullTerminatedArr(const void **nullTerminatedArr) {
    int amountOfFunctions = 0;
    while (*(nullTerminatedArr + amountOfFunctions) != NULL) {
        amountOfFunctions++;
    }

    void **copiedArray = (void **) safeAlloc(sizeof(void *) * (amountOfFunctions + 1));
    if (copiedArray == NULL) {
        return NULL;
    }

    memcpy(copiedArray, nullTerminatedArr, sizeof(void *) * (amountOfFunctions + 1));
    return copiedArray;
}
int IsraeliQueueGetPosition(IsraeliQueue q, void *item) {
    int position = 0;
    if (q == NULL || item == NULL) {
        return -1;
    }

    Node tmp = q->list->m_first->m_next;
    while (tmp != NULL) {
        if (q->comparisonFunction(tmp->m_data, item) == COMPARISON_EQUALS) {
            return position;
        }
        position++;
        tmp = tmp->m_next;
    }
    return -1;
}
int calculateGeoFriendship(IsraeliQueue *q) {
    int amount = 0, friendship = 0;
    while (q != NULL) {
        friendship += (*q)->m_friendship;
        amount++;
        q++;
    }

    return ceil(pow(friendship, 1. / amount));
}
int calculateGeoRivalry(IsraeliQueue *q) {
    int amount = 0, rivalry = 0;
    while (q != NULL) {
        rivalry += (*q)->m_rivalry;
        amount++;
        q++;
    }

    return ceil(pow(rivalry, 1. / amount));
}
void addFunctions(IsraeliQueue *oldQueue, IsraeliQueue *newQueue) {
    while (oldQueue != NULL) {
        FriendshipFunction func = *(*oldQueue)->friendshipFunction;
        while (func != NULL) {
            func = *(*oldQueue)->friendshipFunction;
            IsraeliQueueAddFriendshipMeasure(*newQueue, func);
        }
        oldQueue++;
    }
}
double getFriendshipAverage(IsraeliQueue q, void *item1, void *item2) {
    int numOfFunctions = 0;
    double sum = 0;
    FriendshipFunction *func = q->friendshipFunction;
    while (*func != NULL) {
        sum += (*func)(item1, item2);
        numOfFunctions++;
        func++;
    }
    if (numOfFunctions == 0) {
        return 0;
    }
    return sum / numOfFunctions;
}
void checkForMoreBlocks(IsraeliQueue q, Node tmp, void *item, stack s) {
    while ((tmp = pop(&s)) != NULL){
        Student student = (Student) tmp->m_data;
        if(student->m_numOfRivals < RIVAL_QUOTA && checkIfRivals(q, tmp->m_data, item)){
            student->m_numOfRivals++;
        }
    }
}
bool checkIfFriends(IsraeliQueue q, Node itemNode, void *item2) {
    if (q->comparisonFunction(itemNode->m_data, item2) == COMPARISON_EQUALS) {
        return false;
    }
    FriendshipFunction *func = q->friendshipFunction;

    while (*func != NULL) {
        if ((*func)(itemNode->m_data, item2) > q->m_friendship) {
            return true;
        }
        func++;
    }
    return false;
}
bool checkIfRivals(IsraeliQueue q, void *item1, void *item2) {
    if (q->comparisonFunction(item1, item2) == COMPARISON_EQUALS) {
        return false;
    }
    double avg = getFriendshipAverage(q, item1, item2);
    return avg < q->m_rivalry;
}