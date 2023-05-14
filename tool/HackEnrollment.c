//
// Created by Meitar Basson on 13/05/2023.
//

#include "HackEnrollment.h"

void initQueue(IsraeliQueue q);

EnrollmentSystem createEnrollment(FILE *students, FILE *courses, FILE *hackers) {

    FILE *studentsStream = fopen((const char *) students, "r");
    FILE *coursesStream = fopen((const char *) courses, "r");
    FILE *hackersStream = fopen((const char *) hackers, "r");

    EnrollmentSystem system = (EnrollmentSystem) safeAlloc(sizeof(struct enrollmentSystem));

    memset(system, 0, sizeof(struct enrollmentSystem));
    if (studentsStream == NULL || coursesStream == NULL || hackersStream == NULL || system == NULL) {
        free(system->m_students);
        free(system->m_courses);
        free(system->m_hackers);
        free(system); // REMEMBER TO NOT CALL DESTROY HERE
        safeFclose(studentsStream);
        safeFclose(coursesStream);
        safeFclose(hackersStream);
        return NULL;
    }
    initEnrollmentSystem(system);

    if (!parseStudentsFile(system, studentsStream) || !parseCoursesFile(system, coursesStream) ||
        !parseHackersFile(system, hackersStream)) {
        destroyEnrollmentSystem(system);
        return NULL;
    }

    safeFclose(studentsStream);
    safeFclose(coursesStream);
    safeFclose(hackersStream);

    return system;
}

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE *queues) {
    FILE *queueStream = fopen((const char *) queues, "r");
    if (queueStream == NULL || sys == NULL) {
        safeFclose(queueStream);
        return NULL;
    }

    bool parseSucceeded = parseQueueFile(sys, queueStream);
    safeFclose(queueStream);

    if (parseSucceeded) {
        return sys;
    }

    return NULL;
}

void hackEnrollment(EnrollmentSystem sys, FILE *out) {
    FILE *outStream = fopen((const char *) out, "w");
    if (outStream == NULL || sys == NULL) {
        return;
    }
    printQueueIntoFile(sys, outStream);
    fflush(outStream);
    fclose(outStream);
}


bool parseStudentsFile(EnrollmentSystem sys, FILE *students) {
    if (students == NULL) {
        return false;
    }

    while (feof(students) == 0) {
        Node newNode = (Node) safeAlloc(sizeof(struct node));
        if (newNode == NULL) {
            return false;
        }
        newNode->m_data = parseSingleLineIntoStudent(students);
        if (newNode->m_data == NULL) {
            free((void *) newNode);
            return feof(students);
        }
        sys->m_students->m_first = pushBack(sys->m_students->m_first, newNode);
    }
    return true;
}

bool parseHackersFile(EnrollmentSystem sys, FILE *hackers) {
    if (hackers == NULL) {
        return false;
    }
    while (feof(hackers) == 0) {
        Node newNode = (Node) safeAlloc(sizeof(struct node));
        if (newNode == NULL) {
            return false;
        }
        newNode->m_data = parseHacker(hackers);
        if (newNode->m_data == NULL) {
            free((void *) newNode);
            return feof(hackers);
        }
        sys->m_hackers->m_first = pushBack(sys->m_hackers->m_first, newNode);
    }
    return true;
}

bool parseCoursesFile(EnrollmentSystem sys, FILE *courses) {
    if (courses == NULL) {
        return NULL;
    }
    while (feof(courses) == 0) {
        Node newNode = (Node) safeAlloc(sizeof(struct node));
        if (newNode == NULL) {
            return false;
        }
        newNode->m_data = parseSingleLineIntoCourse(courses, sys->m_caseSenstive);
        if (newNode->m_data == NULL) {
            free((void *) newNode);
            return feof(courses);
        }
        sys->m_courses->m_first = pushBack(sys->m_courses->m_first, newNode);
    }
    return true;
}

bool parseQueueFile(EnrollmentSystem sys, FILE *queues) {
    if (queues == NULL) {
        return false;
    }


    while (feof(queues) == 0) {
        if (parseSingleLineIntoQueue(sys, queues) != ISRAELIQUEUE_SUCCESS) {
            return false;
        }
    }

    Node tmp = sys->m_courses->m_first;
    while (tmp != NULL) {
        Node hackers = sys->m_hackers->m_first;
        Course course = tmp->m_data;
        while (hackers != NULL) {
            Student hackerAsStudent = findStudentInSystem(sys, ((Hacker) hackers->m_data)->m_id);
            if (hackerAsStudent == NULL) {
                return ISRAELI_QUEUE_ERROR;
            }
            LinkedList courseList = ((Hacker) hackers->m_data)->m_desiredCourses;
            if (courseList->m_first == NULL) {
                hackers = hackers->m_next;
                continue;
            }
            int courseNum = course->m_courseNum;
            Node tmpNode = courseList->m_first;
            while (tmpNode != NULL) {
                if (((Course) tmpNode->m_data)->m_courseNum == courseNum) {
                    hackerAsStudent->m_isHacker = true;
                    parseHackerIntoStudent((Hacker) hackers->m_data, hackerAsStudent);
                    IsraeliQueueError err = IsraeliQueueEnqueue(course->m_students, hackerAsStudent);
                    if (err != ISRAELIQUEUE_SUCCESS) {
                        return err;
                    }
                }
                tmpNode = tmpNode->m_next;
            }
            hackers = hackers->m_next;
        }
        tmp = tmp->m_next;
    }

    return true;
}

void printQueueIntoFile(EnrollmentSystem sys, FILE *out) {
    // TODO: Case of 1 course not happy
    bool singlesAreSatisfied = checkIfHackerAskedForOneCourseAndNotSatisfied(sys);
    int studentsId = checkIfHackersAreSatisfiedReturnIdIfNot(sys);
    if(!singlesAreSatisfied) {
        return;
    } else if (studentsId != HACKERS_SATISFIED) {
        fprintf(out, "Cannot satisfy constraints for %d\n", studentsId);
    } else {
        Node tmpNode = sys->m_courses->m_first;
        while (tmpNode != NULL) {
            Course tmpCourse = (Course) tmpNode->m_data;
            IsraeliQueue tmpQueue = tmpCourse->m_students;
            int count = 0;
            if (IsraeliQueueSize(tmpCourse->m_students) > 0) {
                fprintf(out, "%d", tmpCourse->m_courseNum);
                while (tmpQueue != NULL && count < tmpCourse->m_courseSize) {
                    Student student = (Student) IsraeliQueueDequeue(tmpQueue);
                    if (student == NULL) {
                        break;
                    }
                    fprintf(out, " %d", student->m_id);
                    count++;
                }
                fprintf(out, "\n");
            }
            tmpNode = tmpNode->m_next;
        }
    }
}

Student parseSingleLineIntoStudent(FILE *students) {
    char *line = readLine(students);
    if (line == NULL || my_strnlen(line, '\0') == 0) {
        return NULL;
    }
    const char *params[7] = {0};
    const char *tmp = line;

    for (int i = 0; i < 7; i++) {
        params[i] = my_strdup(my_strsep(&tmp, ' '), ' ');
        if (params[i] == NULL) {
            for (int j = 0; j < i; ++j) {
                free((void *) params[j]);
            }
            free((void *) line);
            return NULL;
        }
    }

    const char *studentId = params[0];
    const char *totalCredits = params[1];
    const char *gpa = params[2];
    const char *name = params[3];
    const char *lastName = params[4];
    const char *city = params[5];
    const char *department = params[6];
    Student newStudent = (Student) safeAlloc(sizeof(struct student));
    if (!initStudent(newStudent, studentId, totalCredits, gpa, name, lastName, city, department)) {
        destroyStudent(newStudent);
        free((void *) studentId);
        free((void *) totalCredits);
        free((void *) gpa);
        free(line);
        return NULL;
    }

    free(line);
    free((void *) studentId);
    free((void *) totalCredits);
    free((void *) gpa);
    return newStudent;
}

Course parseSingleLineIntoCourse(FILE *course, bool isSensitive) {
    char *line = readLine(course);
    if (line == NULL) {
        return NULL;
    }
    if (my_strnlen(line, '\0') == 0) {
        if(!feof(course)) {
            free(line);
        }
        return NULL;
    }

    char *params[2] = {0};
    const char *tmp = line;
    params[0] = my_strdup(my_strsep(&tmp, ' '), ' ');
    params[1] = my_strdup(my_strsep(&tmp, ' '), ' ');
    free((void *) line);
    for (int i = 0; i < 2; i++) {
        if (params[i] == NULL) {
            free((void *) params[0]);
            free((void *) params[1]);
            return NULL;
        }
    }
    const char *courseNumber = params[0];
    const char *courseSize = params[1];

    Course newCourse = (Course) safeAlloc(sizeof(struct course));

    if (!initCourse(newCourse, courseNumber, courseSize)) {
        free((void *) courseNumber);
        free((void *) courseSize);
        destroyCourse(newCourse);
        return NULL;
    }

    free((void *) courseNumber);
    free((void *) courseSize);
    return newCourse;
}

IsraeliQueueError parseSingleLineIntoQueue(EnrollmentSystem sys, FILE *queue) {
    char *line = readLine(queue);
    const char *tmp = line;

    if (line == NULL) {
        free((void *) line);
        return feof(queue) == 1 ? ISRAELIQUEUE_SUCCESS : ISRAELIQUEUE_ALLOC_FAILED;
    }

    const char *courseNum = my_strdup(my_strsep(&tmp, ' '), ' ');
    if (courseNum == NULL || !isNumericString(courseNum)) {
        free((void *) line);
        free((void *) courseNum);
        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    int actualCourseNum = atoi(courseNum);
    Course course = findCourseInSystem(sys, actualCourseNum);
    free((void *) courseNum);

    while (true) {
        const char *params = my_strdup(my_strsep(&tmp, ' '), ' ');
        if (params == NULL) {
            break;
        } else {
            if (!isNumericString(params) || (my_strnlen(params, ' ') != 9 && my_strnlen(params, ' ') != 10 &&
                                             *(params + my_strnlen(params, ' ') - 2) != '\n')) {
                free((void *) line);
                free((void *) params);
                return ISRAELIQUEUE_BAD_PARAM;
            }

            int studentId = atoi(params);
            Student student = findStudentInSystem(sys, studentId);
            student->m_isHacker = false;

            IsraeliQueueError err = IsraeliQueueEnqueue(course->m_students, student);

            if (ISRAELIQUEUE_SUCCESS != err) {
                free((void *) line);
                return err;
            }
        }
        free((void *) params);
    }

    if((void *) line != NULL && *line != '\0'){
        free((void *) line);
        line = NULL;
    }
    return ISRAELIQUEUE_SUCCESS;
}

