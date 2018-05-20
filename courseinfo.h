/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: courseInfo.h
 * Description: This class is a holder class for all of the class section's info: it holds course subject, starting time, description,
 *              and much more. This is mainly a class to hold many different variables only.
 */

#ifndef COURSEINFO_H
#define COURSEINFO_H

#include "QString"
#include "QList"
#include "QLinkedList"

class courseInfo
{
public:
    courseInfo();

    int number; //like (CSCI) 4318
    int section;
    int CRN; //5 digits, used to sign up for classes at the end

    QString courseTitle;
    QString instructor;

    QString day;
    QString classStartTime;
    QString classEndTime;

    QString departmentName;
    QString collegeName; //kind of unnecessary, like ACC = Business Administration
    bool isLab;
    QString classLocation; //which building
    int enrolledCurrently;
    int totalSeats;
    bool isUnderGradClass;
    QString description; //catalog description
    QString subject;

    //here are setters:
    void setNumber(int );
    void setSection(int );
    void setCRN(int );

    void setcourseTitle(QString );
    void setInstructor(QString );

    void setClassDay(QString);
    void setClassStartTime(QString);
    void setClassEndTime(QString);

    void setDepName(QString );
    void setCollegeName(QString );

    void setIsLab(bool );

    void setClassLoc(QString );
    void setEnrolledCur(int );
    void setTotalSeats(int );
    void setIsUnderGradClass(bool );
    void setDesc(QString );
    void setSubject(QString subj);

    QString getStringForSchedule(); //used when displaying schedules to users - simple format of info obtained
    bool operator == (const courseInfo &otherCourse);
};


#endif // COURSEINFO_H
