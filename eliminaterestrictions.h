/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: EliminateRestrictions.h
 * Description: This class removes any restrictions a schedule has with chosen restrictions, and then returns any classes that
 *              are not conflicting. This only removes the sections in each class that conflict with the chosen
 *              restrictions, it DOES NOT remove any schedules that conflict.
 */

#ifndef ELIMINATERESTRICTIONS_H
#define ELIMINATERESTRICTIONS_H

#include "courseinfo.h"
#include "coursesofferedlist.h"

//this class will handle removing the class sections that violate any chosen restrictions
class EliminateRestrictions
{
public:
    EliminateRestrictions();

    QList<courseInfo> removeConflictsFromCourseRestrictions(CoursesOfferedList curCourse, QList<QString> classConstraints);
};

#endif // ELIMINATERESTRICTIONS_H
