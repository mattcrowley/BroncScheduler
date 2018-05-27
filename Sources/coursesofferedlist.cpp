/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: CoursesOfferedList.h
 * Description: This class is another holder class. It creates a list of the other holder class, so that we can easily access
 *              each subject's course's different section's info. Basically, we get a subject, then we have a list of
 *              classes. We store those classes here, but since each class has multiple sections, we need a list of them.
 */

#include "Headers/coursesofferedlist.h"

CoursesOfferedList::CoursesOfferedList()
{
}
CoursesOfferedList::CoursesOfferedList(QString subject){
    this->subjectName = subject;
}
