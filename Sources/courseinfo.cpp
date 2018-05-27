/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: courseInfo.h
 * Description: This class is a holder class for all of the class section's info: it holds course subject, starting time, description,
 *              and much more. This is mainly a class to hold many different variables only.
 */

#include "Headers/courseinfo.h"

//constructor
courseInfo::courseInfo(){
    isLab = false;
    isUnderGradClass = false;
}

void courseInfo::setNumber(int num){
    number = num;
}

void courseInfo::setSection(int sec){
    section = sec;
}

void courseInfo::setCRN(int crn){
    CRN = crn;
}

void courseInfo::setcourseTitle(QString title){
    courseTitle = title;
}

void courseInfo::setInstructor(QString instr){
    instructor = instr;
}

void courseInfo::setClassDay(QString d){
    day = d;
}

void courseInfo::setClassStartTime(QString start){
    classStartTime = start;
}

void courseInfo::setClassEndTime(QString end){
    classEndTime = end;
}


void courseInfo::setDepName(QString dept){
    departmentName = dept;
}

void courseInfo::setCollegeName(QString coll){
    collegeName = coll;
}

void courseInfo::setIsLab(bool lab){
    isLab = lab;
}

void courseInfo::setClassLoc(QString loc){
    classLocation = loc;
}

void courseInfo::setEnrolledCur(int enrolled){
    enrolledCurrently = enrolled;
}

void courseInfo::setTotalSeats(int total){
    totalSeats = total;
}

void courseInfo::setIsUnderGradClass(bool undergrad){
    isUnderGradClass = undergrad;
}

void courseInfo::setDesc(QString descr){
    description = descr;
}

void courseInfo::setSubject(QString subj){
    subject = subj;
}


/* Function Name: operator ==
 * Description: This function allows comparison between classes, to ensure they are different or the same, testing
 *              only the title and number of the class.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Another course to test against
*/
bool courseInfo::operator == (const courseInfo &otherCourse){
    if (this->courseTitle == otherCourse.courseTitle && this->number == otherCourse.number)
        return true;
    return false;
}

/* Function Name: getStringForSchedule
 * Description: This function is used when showing the user the table of their schedule, meant to simplify the info
 *              the user actually needs.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
QString courseInfo::getStringForSchedule(){
    QString temp;
    temp = subject + " " + QString::number(number) + " Sec " + QString::number(section) + "\n" + courseTitle + "\n" + classLocation;
    return temp;
}

