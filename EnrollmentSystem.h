//
// Created by Meitar Basson on 13/05/2023.
//

#ifndef EX1_ENROLLMENTSYSTEM_H
#define EX1_ENROLLMENTSYSTEM_H

#include "Course.h"
#include "tool/FilesManager.h"

#define BAD_PARSING 0
#define GOOD_PARSING 1
#define NUM_OF_PARSING 4
#define HACKERS_SATISFIED -1

typedef struct enrollmentSystem {

    LinkedList m_students;
    LinkedList m_courses;
    LinkedList m_hackers;
    bool m_caseSenstive;

} * EnrollmentSystem;


void initEnrollmentSystem(EnrollmentSystem system);
void destroyEnrollmentSystem(EnrollmentSystem system);
Student findStudentInSystem(EnrollmentSystem sys, int studentId);
Course findCourseInSystem(EnrollmentSystem sys, int courseNum);

Student getHackerFromList(EnrollmentSystem system, Hacker hacker);

bool isHackerSatisfied(EnrollmentSystem sys, Hacker hacker);
int checkIfHackersAreSatisfiedReturnIdIfNot(EnrollmentSystem system);
bool checkIfHackerAskedForOneCourseAndNotSatisfied(EnrollmentSystem system);


int parseHackerId(FILE *hackers, int *status);
LinkedList parseHackerCourses(FILE *hackers, int *status);
LinkedList parseHackersStudentsLst(FILE *hackers, int *status);
Hacker parseHacker(FILE *hackers);


#endif //EX1_ENROLLMENTSYSTEM_H
