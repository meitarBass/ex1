//
// Created by Meitar Basson on 14/05/2023.
//

#ifndef EX1_FRIENDLYFUNCTIONS_H
#define EX1_FRIENDLYFUNCTIONS_H

//#include "Student.h"
#include "stdlib.h"

#define FRIENDS 20
#define RIVALS -20
#define NO_RELATION 0


/**@param ha: void pointer to an hacker
 * @param st: void pointer to a student
 * @return the trinary value as was expected from the exercise pdf
 *
 * checkes if the hacker and student are friends/rivals/none
 * */
int areFriends(void *st, void *ha) {
    Student hacker = (Student)ha;
    Student student = (Student)st;

    Node tmpNode = hacker->m_hackersFriends->m_first;

    while(tmpNode != NULL) {
        int tmpId = *(int *)(tmpNode->m_data);
        if(tmpId == student->m_id) {
            return FRIENDS;
        }
        tmpNode = tmpNode->m_next;
    }

    tmpNode = hacker->m_hackersRivals->m_first;
    while(tmpNode != NULL) {
        int tmpId = *(int *)(tmpNode->m_data);
        if(tmpId == student->m_id) {
            return RIVALS;
        }
        tmpNode = tmpNode->m_next;
    }
    return NO_RELATION;
}

/**@param c: character to be made lower
 * @return: lower case letter
 *
 * Used in case we want our program not to be case sensitive
 * */
char makeLower(char c) {
    if(c >= 'A' && c <= 'Z') {
        return c + 'a' - 'A';
    }
    return c;
}


/**@param h: a course to be initialized
 * @param s: pointer to the line parsed from the file
 * @param isSensitive: Used to check if the comparison should be case sensitive
 * @return: the distance between the names
 *
 * Calculate the distance between hacker, student
 * */
int calcNameDist(const char *h, const char *s, bool isSensitive) {
    int total = 0;
    while(*h != '\0' || *s != '\0') {
        if(*h != '\0' && *s == '\0') {
            if (isSensitive) {
                total += (int)(*h);
            } else {
                total += (int)(makeLower(*h));
            }
        } else if (*s != '\0' && *h == '\0') {
            if (isSensitive) {
                total += (int)(*s);
            } else {
                total += (int)(makeLower(*s));
            }
        } else if (isSensitive) {
            total += abs((int)(*h) - (int)(*s));
        } else {
            total += abs((int)(makeLower(*h)) - (int)(makeLower(*s)));
        }

        h++;
        s++;
    }
    return total;
}


/**@param ha: hacker instance
 * @param st: student instace
 * @return returns the name if case sensitive
 *
 * Calculates the distance between the student names case sensitive
 * .*/
int nameDistanceCaseSensitive(void * st, void * ha) {
    Student hacker = (Student) ha, student = (Student) st;

    int total = 0;

    if(hacker->m_name == NULL || student->m_name == NULL){
        return 0;
    }

    const char *h = hacker->m_name, *s = student->m_name;
    total += calcNameDist(h, s, true);

    h = hacker->m_lastName;
    s = student->m_lastName;

    total += calcNameDist(h, s, true);
    return total;
}

/**@param ha: hacker instance
 * @param st: student instace
 * @return returns the name if case sensitive
 *
 * Calculates the distance between the student names case insensitive
 * .*/
int nameDistanceNotSensitive(void * ha, void * st) {
    Student hacker = (Student) ha, student = (Student) st;

    int total = 0;

    if(hacker->m_name == NULL || student->m_name == NULL){
        return 0;
    }

    const char *h = hacker->m_name, *s = student->m_name;
    total += calcNameDist(h, s, false);

    h = hacker->m_lastName;
    s = student->m_lastName;

    total += calcNameDist(h, s, false);
    return total;
}

/**@param ha: hacker instance
 * @param st: student instance
 * @return: the distance
 *
 * Calculates the distance between the hacker and the student ids.
 * .*/
int idDistance(void * st, void * ha) {
    Hacker hacker = (Hacker) ha;
    Student student = (Student) st;
    return abs(hacker->m_id - student->m_id);
}



#endif //EX1_FRIENDLYFUNCTIONS_H
