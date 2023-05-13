//
// Created by Meitar Basson on 13/05/2023.
//

#ifndef EX1_COURSE_H
#define EX1_COURSE_H

#include "Student.h"
#include "IsraeliQueue.h"
#include "tool/StringOps.h"

typedef struct course {

    int m_courseNum;
    int m_courseSize;

    IsraeliQueue m_students;
    bool m_isSensitive;

} * Course;

bool initCourse(Course course, const char *courseNumber, const char *courseSize);
void destroyCourse(Course pCourse);



#endif //EX1_COURSE_H
