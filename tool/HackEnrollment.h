//
// Created by Meitar Basson on 13/05/2023.
//

#ifndef EX1_HACKENROLLMENT_H
#define EX1_HACKENROLLMENT_H

#include "../EnrollmentSystem.h"

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);
void hackEnrollment(EnrollmentSystem sys, FILE* out);

bool parseStudentsFile(EnrollmentSystem sys, FILE *students);
bool parseHackersFile(EnrollmentSystem sys, FILE *hackers);
bool parseCoursesFile(EnrollmentSystem sys, FILE *courses);
bool parseQueueFile(EnrollmentSystem sys, FILE *queues);

void printQueueIntoFile(EnrollmentSystem sys, FILE *out);

Student parseSingleLineIntoStudent(FILE *students);
Course parseSingleLineIntoCourse(FILE *course, bool isSensitive);
IsraeliQueueError parseSingleLineIntoQueue(EnrollmentSystem sys, FILE *queue);

#endif //EX1_HACKENROLLMENT_H
