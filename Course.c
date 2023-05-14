//
// Created by Meitar Basson on 13/05/2023.
//

#include "Course.h"

int compareById(Student student1, Student student2) {
    return (student1->m_id == student2->m_id) ? 1 : 0;
}

bool checkParamsValidity(Course course, const char *courseNumber, const char *courseSize) {
    if(!isNumericString(courseNumber) && !isNumericString(courseSize)) {
        return false;
    }
    course->m_courseNum = atoi(courseNumber);
    course->m_courseSize = atoi(courseSize);

    if(course->m_courseNum < 0 || course->m_courseSize < 0) {
        return false;
    }
    return true;
}

bool checkForCourseNullParamsAndReset(Course course, const char *courseNumber, const char *courseSize) {
    if (course == NULL || courseNumber == NULL || courseSize == NULL) {
        return false;
    }
    memset(course, 0, sizeof(struct course));
    return true;
}

bool initCourse(Course course, const char *courseNumber, const char *courseSize) {

    if(!checkForCourseNullParamsAndReset(course, courseNumber, courseSize))  {
        return false;
    }

    if(!checkParamsValidity(course, courseNumber, courseSize))  {
        return false;
    }

    FriendshipFunction f = NULL;
    course->m_students = IsraeliQueueCreate(&f, (ComparisonFunction) compareById, FRIENDS, RIVALS_MINIMUM);
    if (course->m_students == NULL) {
        return false;
    }

    return true;
}

void destroyCourse(Course pCourse) {
    if(pCourse == NULL) {
        return;
    }

    IsraeliQueueDestroy(pCourse->m_students);
    free(pCourse);
}

