/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: BuildSchedulesList.h
 * Description: This class is responsible for generating the schedules based on the classes the user has selected.
 *              It generates them one at a time, in order to speed up the process, and if it generates an invalid
 *              schedule, it stops, removes it, then starts generating a new schedule.
 */

#ifndef BUILDSCHEDULESLIST_H
#define BUILDSCHEDULESLIST_H

#include <QList>
#include "coursesofferedlist.h"

class BuildSchedulesList
{
public:
    BuildSchedulesList(QList<CoursesOfferedList> chosenCourseList);

    bool createScheduleList(QList<CoursesOfferedList> chosenCourseList);
    bool isScheduleValid(QList<courseInfo> schedule);

    void incrementCurrentIndex();

    QList<courseInfo> createScheduleFromIndexes(QList<CoursesOfferedList> chosenCourseList, QList<int> indexes); //creates a SINGLE schedule
    QList<int> incrementIndexes(QList<int> indexes, QList<CoursesOfferedList> chosenCourses);


    //variables:
    QList<QList<courseInfo> > allSchedulesList; //holds the list of all possible schedules
    QList<int> indexes; //holds the indexes of the chosenCourseList classes
                        //determines which class section of the list to add

    bool stillBuilding; //determines whether we are still generating schedules or if all done

    int currentIndex; //stores which index of completed list we go to next, starts at 0
};

#endif // BUILDSCHEDULESLIST_H
