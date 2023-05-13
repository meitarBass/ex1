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

void initEnrollmentSystem(EnrollmentSystem system) {
    if(system == NULL) {
        return;
    }

    system->m_students = NULL;
    system->m_courses = NULL;
    system->m_hackers = NULL;
    system->m_caseSenstive = true;
}
void destroyEnrollmentSystem(EnrollmentSystem system) {
    if(system == NULL) {
        return;
    }

    // Destroy students
    Node tmpNode;
    tmpNode = system->m_students->m_first;

    while(tmpNode != NULL) {
        Node toFree = tmpNode;
        destroyStudent((Student) tmpNode->m_data);
        tmpNode = tmpNode->m_next;
        free(toFree);
    }

    // Destroy courses
    tmpNode = system->m_courses->m_first;
    while(tmpNode != NULL) {
        Node toFree = tmpNode;
        destroyCourse((Course) tmpNode->m_data);
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
Node parseHackerCourses(FILE *hackers, int *status) {
    char *line = readLine(hackers);
    if (my_strnlen(line, ' ') == 0) {
        *status = GOOD_PARSING;
        free(line);
        return NULL;
    }
    *status = BAD_PARSING;

    if (line == NULL) {
        return NULL;
    }
    const char *tmp = line;
    const char *params = my_strdup(my_strsep(&tmp, ' '), ' ');

    Node courseLst = NULL;

    while (params != NULL) {
        Node newCourse = (Node) safeAlloc(sizeof(struct node));
        int *courseNum = (int *) safeAlloc(sizeof(int));
        if (newCourse == NULL || courseNum == NULL || !isNumericString(params)) {
            free(newCourse);
            free(courseNum);
            free((void *) params);
            free((void *) line);
            return NULL;
        }

        *courseNum = atoi(params);
        newCourse->m_data = courseNum;
        courseLst = pushBack(courseLst, newCourse);

        free((void *) params);
        params = my_strdup(my_strsep(&tmp, ' '), ' ');

    }
    *status = GOOD_PARSING;
    free((void *) line);
    return courseLst;
}
Node parseHackersStudentsLst(FILE *hackers, int *status) {
    char *line = readLine(hackers);
    *status = BAD_PARSING;

    if (line == NULL) {
        return NULL;
    }

    if (my_strnlen(line, ' ') == 0) {
        *status = GOOD_PARSING;
        free(line);
        return NULL;
    }

    const char *tmp = line;
    char *params = my_strdup(my_strsep(&tmp, ' '), ' ');
    Node studentsLst = NULL;

    while (params != NULL) {
        Node newStudent = (Node) safeAlloc(sizeof(struct node));
        int *studentId = (int *) safeAlloc(sizeof(int));

        if (newStudent == NULL || studentId == NULL || !isNumericString(params)) {
            free((void *) line);
            free((void *) newStudent);
            free((void *) studentId);
            free((void *) params);
            return NULL;
        }

        *studentId = atoi(params);
        newStudent->m_data = studentId;
        studentsLst = pushBack(studentsLst, newStudent);

        free((void *) params);
        params = my_strdup(my_strsep(&tmp, ' '), ' ');
    }

    *status = GOOD_PARSING;
    free((void *) line);
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

    Node courses = parseHackerCourses(hackers, &status[1]);
    Node friends = parseHackersStudentsLst(hackers, &status[2]);
    Node rivals = parseHackersStudentsLst(hackers, &status[3]);
    initHacker(hacker, id, courses, friends, rivals);

    for (int i = 0; i < NUM_OF_PARSING; i++) {
        if (status[i] == BAD_PARSING) {
            destroyHacker(hacker);
            return NULL;
        }
    }
    return hacker;
}