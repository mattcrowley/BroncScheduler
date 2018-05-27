/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: EliminateRestrictions.h
 * Description: This class removes any restrictions a schedule has with chosen restrictions, and then returns any classes that
 *              are not conflicting. This only removes the sections in each class that conflict with the chosen
 *              restrictions, it DOES NOT remove any schedules that conflict.
 */

#include "Headers/eliminaterestrictions.h"
#include "qstringlist.h"
#include "qdebug.h"

EliminateRestrictions::EliminateRestrictions()
{
}

/* Function Name: removeConflictsFromCourseRestrictions
 * Description: This function will take in the time constraints the user chose, and then eliminate course sections that conflict with those times.
 *              This does not eliminate schedules, just sections.
 *
 *              Returns a list of courses not eliminated.
 * Preconditions: The user hit "Build Schedules" button.
 * Postconditions: All sections remaining do not conflict with chosen time constraints.
 * Inputs: The list of courses selected and all sections, and the constraints chosen by the user.
*/
QList<courseInfo> EliminateRestrictions::removeConflictsFromCourseRestrictions(CoursesOfferedList curCourse, QList<QString> classConstraints){
    QList<courseInfo> finalList;

    //need to go through each course's sections and remove any section in conflict, does one course at a time
    foreach(courseInfo c, curCourse.coursesOffered){
        finalList.append(c); //add it to the list, always assume it will not have a restriction

        foreach(QString curRestriction, classConstraints){ //now we go through the list of restrictions one by one and compare this section to that restriction
            QStringList splitStr = curRestriction.split(" ", QString::SkipEmptyParts);

            if (splitStr.size() == 1){ //only one field, thus a full day is blocked
                if (splitStr[0].compare("Thursday", Qt::CaseInsensitive) == 0 &&
                         c.day.contains("R", Qt::CaseInsensitive)){ //this is the exception, Thursday is represented not by T, but by R
                    //remove from final list
                    finalList.removeLast();
                    break;
                }
                else if (c.day.contains(splitStr[0][0], Qt::CaseInsensitive) &&
                         splitStr[0].compare("Thursday", Qt::CaseInsensitive) != 0){
                    //remove from final list, as it conflicts
                    finalList.removeLast();
                    break; //no need to continue the loop, already removed it
                }
                else{
                    //reached when blocking a day, but no class section for that day, no need to do anything
                }
            }
            else if (splitStr.size() == 4){ //has more than one field; meaning has day(s) and start and end time that we want to block
                bool found = false;

                //first check if the day of the week is going to be blocked, splitStr[0] contains the day(s) blocked
                for (int i = 0; i < splitStr[0].size(); i++){
                    if (splitStr[0].compare("Thursday", Qt::CaseInsensitive) == 0 && c.day.contains("R", Qt::CaseInsensitive)){
                        found = true;
                        break;
                    }
                    else if (splitStr[0].compare("Thursday", Qt::CaseInsensitive) != 0 &&
                             c.day.contains(splitStr[0][i], Qt::CaseInsensitive)){
                        found = true;
                        break;
                    }
                }
                if (found == true){ //need to block this class if the time is correctly chosen:
                    //now check if the blocked off time is contained within the current section's class time
                    if ( (splitStr[1] > c.classStartTime && splitStr[1] <= c.classEndTime) ||
                            (splitStr[1] == c.classStartTime) ||
                            (splitStr[1] < c.classStartTime && splitStr[3] >= c.classEndTime) ||
                            (splitStr[1] < c.classStartTime && splitStr[3] < c.classEndTime && splitStr[3] > c.classStartTime) ){
                        //remove from list
                        finalList.removeLast();
                        break;
                    }
                }
            }
        }
    }
    return finalList;
}

