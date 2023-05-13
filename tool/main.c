//
// Created by Meitar Basson on 13/05/2023.
//

#include "HackEnrollment.h"
#include "../FriendlyFunctions.h"

void addFunctionsToSystem(EnrollmentSystem system, bool isSensitive) {
    Node courseNode = (Node) system->m_courses;
    while(courseNode != NULL) {
        Course course = (Course)courseNode->m_data;
        if(isSensitive) {
            IsraeliQueueAddFriendshipMeasure(course->m_students, nameDistanceCaseSensitive);
        } else {
            IsraeliQueueAddFriendshipMeasure(course->m_students,nameDistanceNotSensitive);
        }
        IsraeliQueueAddFriendshipMeasure(course->m_students, idDistance);
        IsraeliQueueAddFriendshipMeasure(course->m_students, areFriends);
        courseNode = courseNode->m_next;
    }
}

int main(int argc, char **argv) {
    EnrollmentSystem system = NULL;
    if(argc == 6) {
        system = createEnrollment((FILE *) argv[1], (FILE *) argv[2], (FILE *) argv[3]);
        if(system != NULL) {
            addFunctionsToSystem(system, true);
//            if(readEnrollment(system, (FILE *) argv[4]) != NULL) {
//                hackEnrollment(system, (FILE *) argv[5]);
//            }
        }
    } else if(argc == 7) {
        system = createEnrollment((FILE *) argv[2], (FILE *) argv[3], (FILE *) argv[4]);
        if(system != NULL) {
            addFunctionsToSystem(system, false);
//            if(readEnrollment(system, (FILE *) argv[5]) != NULL) {
//                hackEnrollment(system, (FILE *) argv[6]);
//            }
        }
    }

    if(system != NULL) {
        destroyEnrollmentSystem(system);
    }
    return 0;
}