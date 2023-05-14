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


/**@param system: system to be initialized
 *
 * initialize a new sysem
 * .*/
void initEnrollmentSystem(EnrollmentSystem system);


/**@param system: system to be deallocated
 *
 *
 * destroys the system
 * .*/
void destroyEnrollmentSystem(EnrollmentSystem system);


/**@param sys: our system
 * @param studentId: student to be found
 *
 * returns a student object out of the system according to the student id
 * .*/
Student findStudentInSystem(EnrollmentSystem sys, int studentId);


/**@param sys: our system
 * @param courseNum: course to be found
 *
 * returns a course object out of the system according to the course number
 * .*/
Course findCourseInSystem(EnrollmentSystem sys, int courseNum);


/**@param system: our system
 * @param hacker: hacker to be found
 *
 * returns a student object out of the system according to the hacker
 * .*/
Student getHackerFromList(EnrollmentSystem system, Hacker hacker);

/**@param sys: our system
 * @param hacker: hacker to be checked if happy
 * @return: true if a single hacker is happy
 *
 * checks if a hacker is happy with the results of the assignment
 * .*/
bool isHackerSatisfied(EnrollmentSystem sys, Hacker hacker);


/**@param sys: our system
 * @return: id of unsatisfied hacker or param if all happy
 *
 * checks if all of the hackers are happy with the results
 * .*/
int checkIfHackersAreSatisfiedReturnIdIfNot(EnrollmentSystem system);


/**@param sys: our system
 * @return: if all of the hackers who wanted a single course are happy
 *
 * make sure no hacker who wanted a single course is sad
 * .*/
bool checkIfHackerAskedForOneCourseAndNotSatisfied(EnrollmentSystem system);


/**@param hackers: hacker file to get the id from
 * @param status: checks for success or failure
 * return hacker's id
 *
 *
 * gets hacker's id from the file
 * .*/
int parseHackerId(FILE *hackers, int *status);


/**@param hackers: hacker file
 * @param status: checks for success or failure
 * @return list of hacker's courses
 *
 * creates a single user from file
 * .*/
LinkedList parseHackerCourses(FILE *hackers, int *status);


/**@param hackers: hacker file
 * @param status: checks for success or failure
 * @return hacker's students list friends / rivals
 *
 * Used to get hackers friends/rivals list.
 * .*/
LinkedList parseHackersStudentsLst(FILE *hackers, int *status);


/**@param hackers: hacker's file
 * @return new hacker
 *
 * parse a new hacker
 * .*/
Hacker parseHacker(FILE *hackers);


#endif //EX1_ENROLLMENTSYSTEM_H
