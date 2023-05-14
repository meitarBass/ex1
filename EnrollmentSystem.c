//
// Created by Meitar Basson on 13/05/2023.
//

#include "EnrollmentSystem.h"

int IsraeliQueueGetPosition(IsraeliQueue q, void *item);


int checkIfHackersAreSatisfiedReturnIdIfNot(EnrollmentSystem system) {
    Node tmpNode = system->m_hackers->m_first;
    while (tmpNode != NULL) {
        Hacker tmpHacker = (Hacker) tmpNode->m_data;
        if (!isHackerSatisfied(system, tmpHacker)) {
            return tmpHacker->m_id;
        }
        tmpNode = tmpNode->m_next;
    }
    return HACKERS_SATISFIED;
}

bool checkIfHackerAskedForOneCourseAndNotSatisfied(EnrollmentSystem system) {
    int satisfaction = 0;
    Node tmpNode = system->m_hackers->m_first;
    while (tmpNode != NULL) {
        Hacker tmpHacker = (Hacker) tmpNode->m_data;
        Node myNode = (Node)tmpHacker->m_desiredCourses->m_first;
        while(myNode != NULL) {
            Course desiredCourse = (Course) myNode->m_data;
            Course course = findCourseInSystem(system, desiredCourse->m_courseNum);
            int pos = IsraeliQueueGetPosition(course->m_students, getHackerFromList(system, tmpHacker));
            if (pos < course->m_courseSize) {
                satisfaction++;
            }
            myNode = myNode->m_next;
        }
        if(getHackerDesiredCount(tmpHacker) == 1 && satisfaction < 1) {
            return false;
        }
        tmpNode = tmpNode->m_next;
    }
    return true;
}

void initEnrollmentSystem(EnrollmentSystem system) {
    if(system == NULL) {
        return;
    }

    system->m_students = safeAlloc(sizeof(struct linkedList));
    system->m_courses = safeAlloc(sizeof(struct linkedList));
    system->m_hackers = safeAlloc(sizeof(struct linkedList));

    system->m_caseSenstive = true;
}

void destroyEnrollmentSystem(EnrollmentSystem system) {
    if(system == NULL) {
        return;
    }

    Node tmpNode;

    // Destroy students
    tmpNode = system->m_students->m_first;
    while(tmpNode != NULL) {
        Node toFree = tmpNode;
        destroyStudent((Student) tmpNode->m_data);
        tmpNode->m_data = NULL;
        tmpNode = tmpNode->m_next;
        free(toFree);
    }


    // Destroy hackers
    tmpNode = system->m_hackers->m_first;
    while(tmpNode != NULL) {
        Node toFree = tmpNode;
        destroyHacker((Hacker) tmpNode->m_data);
        tmpNode = tmpNode->m_next;
        free(toFree);
    }

    // Destroy courses
    tmpNode = system->m_courses->m_first;
    while(tmpNode != NULL) {
        Node toFree = tmpNode;
        destroyCourse((Course)tmpNode->m_data);
        tmpNode = tmpNode->m_next;
        free(toFree);
    }

    free(system->m_students);
    free(system->m_courses);
    free(system->m_hackers);

    free(system);
}
bool isHackerSatisfied(EnrollmentSystem sys, Hacker hacker) {
    int satisfaction = 0;
    Node tmpNode = hacker->m_desiredCourses->m_first;
    while (tmpNode != NULL) {
        Course desiredCourse = (Course) tmpNode->m_data;
        Course course = findCourseInSystem(sys, desiredCourse->m_courseNum);
        int pos = IsraeliQueueGetPosition(course->m_students, getHackerFromList(sys, hacker));
        if (pos < course->m_courseSize) {
            satisfaction++;
        }
        tmpNode = tmpNode->m_next;
    }

    return getHackerDesiredCount(hacker) == satisfaction || satisfaction >= 2;
}


Student findStudentInSystem(EnrollmentSystem sys, int studentId) {
    Node tmpNode = sys->m_students->m_first;
    while(tmpNode != NULL) {
        Student student = (Student) tmpNode->m_data;
        if(student->m_id == studentId) {
            return student;
        }
        tmpNode = tmpNode->m_next;
    }
    return NULL;
}
Student getHackerFromList(EnrollmentSystem system, Hacker hacker) {
    int id = hacker->m_id;
    Node tmpNode = system->m_students->m_first;
    while(tmpNode != NULL) {
        Student student = (Student) tmpNode->m_data;
        if(student->m_id == id) {
            return student;
        }
        tmpNode = tmpNode->m_next;
    }
    return NULL;
}
Course findCourseInSystem(EnrollmentSystem sys, int courseNum) {
    Node tmpNode = sys->m_courses->m_first;
    while (tmpNode != NULL) {
        Course tmpCourse = (Course) tmpNode->m_data;
        if (tmpCourse->m_courseNum == courseNum) {
            return tmpCourse;
        }
        tmpNode = tmpNode->m_next;
    }
    return NULL;
}



int parseHackerId(FILE *hackers, int *status) {
    char *line = readLine(hackers);
    if (line == NULL || *line == '\0') {
        *status = BAD_PARSING;
        return -1;
    }

    if (!isNumericString(line) || !(my_strnlen(line, ' ') != 9 || (my_strnlen(line, ' ') != 10 &&
                                                                   *(line + my_strnlen(line, ' ') - 2) != '\n'))) {
        free((void *) line);
        *status = BAD_PARSING;
        return -1;
    }

    *status = GOOD_PARSING;
    int id = atoi(line);
    free((void *) line);
    return id;
}

LinkedList parseHackerCourses(FILE *hackers, int *status) {
    char *line = readLine(hackers);
    if (my_strnlen(line, ' ') == 0) {
        *status = GOOD_PARSING;
        if(!feof(hackers)){
            free(line);
        }
        LinkedList courseLst = safeAlloc(sizeof (struct linkedList));
        return courseLst;
    }
    *status = BAD_PARSING;

    if (line == NULL) {
        return NULL;
    }

    const char *tmp = line;
    const char *params = my_strdup(my_strsep(&tmp, ' '), ' ');


    LinkedList courseLst = safeAlloc(sizeof (struct linkedList));

    while (params != NULL) {
        Node newCourse = (Node) safeAlloc(sizeof(struct node));
        int *courseNum = (int *) safeAlloc(sizeof(int));
        if (newCourse == NULL || courseNum == NULL || !isNumericString(params)) {
            free(newCourse);
            free(courseNum);
            free((void *) params);
            free((void *) line);
            free((void *) courseLst);
            return NULL;
        }

        *courseNum = atoi(params);
        newCourse->m_data = courseNum;
        courseLst->m_first = pushBack(courseLst->m_first, newCourse);

        free((void *) params);
        params = my_strdup(my_strsep(&tmp, ' '), ' ');

    }
    *status = GOOD_PARSING;
    free((void *) line);
    return courseLst;
}
LinkedList parseHackersStudentsLst(FILE *hackers, int *status) {
    char *line = readLine(hackers);
    *status = BAD_PARSING;

    if (line == NULL) {
        return NULL;
    }

    if (my_strnlen(line, ' ') == 0) {
        *status = GOOD_PARSING;
        if(!feof(hackers)) {
            free(line);
        }
        return NULL;
    }

    const char * tmp = line;
    char *params = my_strdup(my_strsep(&tmp, ' '), ' ');
    LinkedList studentsLst = safeAlloc(sizeof (struct linkedList));

    while (params != NULL) {
        Node newStudent = (Node) safeAlloc(sizeof(struct node));
        int *studentId = (int *) safeAlloc(sizeof(int));

        if (studentsLst == NULL || newStudent == NULL || studentId == NULL || !isNumericString(params)) {
            free((void *) line);
            free((void *) newStudent);
            free((void *) studentId);
            free((void *) params);
            free((void *) studentsLst);
            return NULL;
        }

        *studentId = atoi(params);
        newStudent->m_data = studentId;
        studentsLst->m_first = pushBack(studentsLst->m_first, newStudent);

        free((void *) params);
        params = my_strdup(my_strsep(&tmp, ' '), ' ');
    }

    *status = GOOD_PARSING;
    free(line);
    return studentsLst;
}

Hacker parseHacker(FILE *hackers) {
    Hacker hacker = (Hacker) safeAlloc(sizeof(struct hacker));
    if (hacker == NULL) {
        return NULL;
    }

    int status[NUM_OF_PARSING] = {0};

    int id = parseHackerId(hackers, &status[0]);

    if (id == -1) {
        destroyHacker(hacker);
        return NULL;
    }

    LinkedList courses = parseHackerCourses(hackers, &status[1]);
    LinkedList friends = parseHackersStudentsLst(hackers, &status[2]);
    LinkedList rivals = parseHackersStudentsLst(hackers, &status[3]);
    initHacker(hacker, id, courses, friends, rivals);

    for (int i = 0; i < NUM_OF_PARSING; i++) {
        if (status[i] == BAD_PARSING) {
            destroyHacker(hacker);
            return NULL;
        }
    }
    return hacker;
}