//
// Created by Meitar Basson on 13/05/2023.
//

#ifndef EX1_STUDENT_H
#define EX1_STUDENT_H

#include "stdio.h"
#include "LinkedList.h"
#include "tool/StringOps.h"

typedef struct student {
    int m_id;
    int m_credits;
    int m_gpa;

    const char *m_name;
    const char *m_lastName;
    const char *m_city;
    const char *m_department;

    bool m_isHacker;
    LinkedList m_hackersFriends;
    LinkedList m_hackersRivals;

} *Student;

typedef struct hacker {
    int m_id;
    LinkedList m_desiredCourses;
    LinkedList m_friends;
    LinkedList m_rivals;
} *Hacker;

bool initStudent(Student student, const char *id, const char *credits, const char *gpa,
                 const char *name, const char *lastName, const char *city, const char *department);
void initHacker(Hacker hacker, int id, LinkedList courses, LinkedList friends, LinkedList rivals);
void destroyStudent(Student pStudent);
void destroyHacker(Hacker pHacker);
void parseHackerIntoStudent(Hacker pHacker, Student pStudent);
int getHackerDesiredCount(Hacker hacker);


#endif //EX1_STUDENT_H
