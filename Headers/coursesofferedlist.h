/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: CoursesOfferedList.h
 * Description: This class is another holder class. It creates a list of the other holder class, so that we can easily access
 *              each subject's course's different section's info. Basically, we get a subject, then we have a list of
 *              classes. We store those classes here, but since each class has multiple sections, we need a list of them.
 */

#ifndef COURSESOFFEREDLIST_H
#define COURSESOFFEREDLIST_H

#include "courseinfo.h"

//simple holder class to allow multiple classes with multiple subjects
class CoursesOfferedList
{
public:
    CoursesOfferedList();
    CoursesOfferedList(QString subject);

    QList<courseInfo> coursesOffered; //list of courses in this subject
    QString subjectName;
};

//this is used to sort lists of course info to better display them to the user
struct sortByStartTime{
    bool operator() ( courseInfo left,  courseInfo right){
        return (left.classStartTime < right.classStartTime);
    }
};

//this is used to sort the drop down box of class sections
struct sortStuff{
    bool operator() ( courseInfo left,  courseInfo right){
        return (left.number < right.number);
    }
};

struct sortByDayAndStartTime{
    bool operator() (courseInfo left, courseInfo right){
        return ( (left.day < right.day) && (left.classStartTime < right.classStartTime) );
    }
};

struct sortByAscendingOrderDay{
    bool operator() (courseInfo left, courseInfo right){
        return (left.day < right.day) ;
    }
};


#endif // COURSESOFFEREDLIST_H
