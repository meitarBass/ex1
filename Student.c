//
// Created by Meitar Basson on 13/05/2023.
//

#include "Student.h"

bool checkForStudentNullParamsAndReset(Student student, const char *id, const char *credits, const char *gpa,
                                const char *name, const char *lastName, const char *city, const char *department) {
    if (student == NULL || id == NULL || credits == NULL || gpa == NULL || name == NULL || lastName == NULL ||
        city == NULL || department == NULL) {
        return false;
    }
    memset(student, 0, sizeof(struct student));
    return true;
}

bool checkIdValidity(const char *id) {
    bool isNumeric = isNumericString(id);
    bool isValidLength = my_strnlen(id, ' ') == 9 || (my_strnlen(id, ' ') == 10 && id[9] == '\n');
    return isNumeric && isValidLength;
}

bool checkParamsValidityAndInit(Student student, const char *id, const char *credits, const char *gpa) {
    if (checkIdValidity(id) && isNumericString(credits) && isNumericString(gpa)) {
        student->m_id = atoi(id);
        student->m_credits = atoi(credits);
        student->m_gpa = atoi(gpa);
        if (student->m_credits < 0 || student->m_gpa < 0 || student->m_gpa > 100) {
            return false;
        }
        return true;
    }
    return false;
}

bool initStudent(Student student, const char *id, const char *credits, const char *gpa,
                 const char *name, const char *lastName, const char *city, const char *department) {

    if (!checkForStudentNullParamsAndReset(student, id, credits, gpa, name, lastName, city, department)) {
        return false;
    }

    if (!checkParamsValidityAndInit(student, id, credits, gpa)) {
        return false;
    }

    student->m_name = name;
    student->m_lastName = lastName;
    student->m_city = city;
    student->m_department = department;

    student->m_isHacker = false;

    return true;
}

void initHacker(Hacker hacker, int id, LinkedList courses, LinkedList friends, LinkedList rivals) {
    hacker->m_id = id;
    hacker->m_desiredCourses = courses;
    hacker->m_friends = friends;
    hacker->m_rivals = rivals;
}
void destroyStudent(Student pStudent) {
    if (pStudent == NULL) {
        return;
    }

    free((void *) pStudent->m_name);
    free((void *) pStudent->m_lastName);
    free((void *) pStudent->m_city);
    free((void *) pStudent->m_department);

    free(pStudent);
    pStudent = NULL;
}

void destroyHacker(Hacker pHacker) {
    if(pHacker == NULL) {
        return;
    }

    // destroy courses
    Node tmpNode;

    if(pHacker->m_desiredCourses != NULL) {
        tmpNode = pHacker->m_desiredCourses->m_first;
        while(tmpNode != NULL) {
            free(tmpNode->m_data);
            Node toFree = tmpNode;
            tmpNode = tmpNode->m_next;
            free(toFree);
        }
    }


    // destroy friends
    if(pHacker->m_friends != NULL) {
        tmpNode = pHacker->m_friends->m_first;
        while(tmpNode != NULL) {
            free(tmpNode->m_data);
            Node toFree = tmpNode;
            tmpNode = tmpNode->m_next;
            free(toFree);
        }
    }


    // destroy rivals
    if(pHacker->m_rivals != NULL) {
        tmpNode = pHacker->m_rivals->m_first;
        while(tmpNode != NULL) {
            free(tmpNode->m_data);
            Node toFree = tmpNode;
            tmpNode = tmpNode->m_next;
            free(toFree);
        }
    }

    free(pHacker->m_desiredCourses);
    free(pHacker->m_friends);
    free(pHacker->m_rivals);
    free(pHacker);
}
void parseHackerIntoStudent(Hacker pHacker, Student pStudent) {
    pStudent->m_hackersFriends = pHacker->m_friends;
    pStudent->m_hackersRivals = pHacker->m_rivals;
}


int getHackerDesiredCount(Hacker hacker) {
    int count = 0;
    Node tmpHackerNode = hacker->m_desiredCourses->m_first;
    while(tmpHackerNode != NULL) {
        count++;
        tmpHackerNode = tmpHackerNode->m_next;
    }
    return count;
}