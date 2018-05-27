/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: BuildSchedulesList.h
 * Description: This class is responsible for generating the schedules based on the classes the user has selected.
 *              It generates them one at a time, in order to speed up the process, and if it generates an invalid
 *              schedule, it stops, removes it, then starts generating a new schedule.
 */

#include "Headers/buildscheduleslist.h"
#include "qdebug.h"

//constructor for creating schedule
BuildSchedulesList::BuildSchedulesList(QList<CoursesOfferedList> chosenCourseList)
{
    for (int i = 0; i < chosenCourseList.size(); i++){
        indexes.append(0); //this indexes array simply holds which index we are currently on for each course
    }

    stillBuilding = true; //still have schedules to generate
    currentIndex = 0;
}

//precondition: sorted by ascending order (num of sections), and all have at least one section
/* Function Name: createScheduleList
 * Description: This function generates one schedule per function call, to ensure the user does not have to wait long.
 *              What it does is each of the indexes in the indexes array corresponds to a different class section. When
 *              you increment the last index in the array, you either have a new schedule from all the indexes put together,
 *              or you exceeded the bounds, needing to go back, reset the curIndex to zero, then increment the index to
 *              the left of the current index, if it exists.
 *
 *              Ex: say 4 classes, first possible schedule is: 0000, then 0001, then 0002 (2 is too large, so skips this and goes) 0010.
 *                  Etc, until all schedules were created.
 *
 * Preconditions: The chosen course list has its courses sorted from lowest number of sections to highest at the end.
 * Postconditions: NONE
 * Inputs: List of user selected courses and their sections sorted in ascending order
*/
bool BuildSchedulesList::createScheduleList(QList<CoursesOfferedList> chosenCourseList){
    bool generated = false;

    //this loops runs until we have a valid schedule, or exhausted all of our options
    while(stillBuilding && (indexes[0] <= chosenCourseList[0].coursesOffered.size()-1) ){ //main loop
        //call function to create the schedule at index of each item in the indexes list
        QList<courseInfo> temp = createScheduleFromIndexes(chosenCourseList, indexes);

        //test to see if the schedule is valid or not:
        bool isValid = this->isScheduleValid(temp);

        //now call function to increment indexes and see if we need to continue this loop, or we are done
        indexes = incrementIndexes(indexes, chosenCourseList);

        if (isValid == true){
            allSchedulesList.append(temp);
            foreach(courseInfo c, temp){
                qDebug() << c.courseTitle << " " << c.day << " " << c.classStartTime << " - " << c.classEndTime;
            }

            generated = true;
            break; //exit the loop, we got our single schedule
        }
    }

    if (generated == true){ //able to create a valid schedule
        //qDebug() << "GENERATED VALID SCHEDULE";
    }
    else{ //invalid schedule, all possible schedules created
        //qDebug() << "ALL SCHEDULES GENERATED, NO MORE LEFT" ;
        stillBuilding = false;
    }

    return generated;
}


/* Function Name: createScheduleFromIndexes
 * Description: The size of the indexes array is the same as the chosenCourseList size, allowing direct uses.
 *              For each index position of indexes, it corresponds to the index of the class in the chosenCourseList,
 *              and the value in the indexes array corresponds to which section of the course we want to add to the schedule
 * Preconditions: That indexes has valid indexes for ALL of its index values
 * Postconditions: NONE
 * Inputs: Takes in the user chosen classes, with each of their subjects, and the indexes of the subjects the current
 *         schedule will add.
*/
//this function will create a QList<courseInfo>, and populate it with different sections at passed in indexes from chosenCourseList
QList<courseInfo> BuildSchedulesList::createScheduleFromIndexes(QList<CoursesOfferedList> chosenCourseList, QList<int> indexes){
    QList<courseInfo> newSchedule;

    for (int i = 0; i < chosenCourseList.size(); i++){
        newSchedule.append(chosenCourseList[i].coursesOffered.at(indexes[i])); //find the section of the class at a certain index
    }
    return newSchedule;
}


/* Function Name: incrementIndexes
 * Description: This function increases the indexes according to previous constraints: starting at far right, and if
 *              that number is too large, then increment it to zero, then add one to number to the left one, and so on.
 *
 *              Returns an array corresponding to the new schedule (not test if valid here!)
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: The array of indexes and chosen courses
*/
QList<int> BuildSchedulesList::incrementIndexes(QList<int> indexes, QList<CoursesOfferedList> chosenCourses){
    int curChangeIndex = chosenCourses.size()-1;

    for (int i = indexes.size()-1; i >= 0; i--){ //starts at last index value, works way to first
        if ( (indexes[curChangeIndex] + 1) <= (chosenCourses[curChangeIndex].coursesOffered.size()-1) ) {
            //can successfully increment it, thus the next schedule will be based on this new index determined
            indexes[curChangeIndex] += 1;

            //now for all indexes > curChangeIndex, change them to 0, so we can generate all schedules
            for (int j = curChangeIndex+1; j <= indexes.size()-1; j++){
                indexes[j] = 0;
            }
            break;
        }
        else{ //index is now larger than the course section numbers, move on to next index (one to the left)
            curChangeIndex--;
            if (curChangeIndex < 0){
                indexes[0] += 1;
                qDebug() << "curChangeIndex is less than 0 now";
            }
        }
    }

    return indexes;
}


/* Function Name: isScheduleValid
 * Description: Takes in a list of course sections, and determines whether any times conflict with each other.
 *              If times conflict, invalid schedule, so return false. Else returns true.
 *
 *              Uses two nested loops: one outer to go over the list, the inner to test each class after the outer
 *              loop class.
 * Preconditions: Valid and complete schedule was created and generated
 * Postconditions: true or false is returned.
 * Inputs: The generated schedule.
*/
bool BuildSchedulesList::isScheduleValid(QList<courseInfo> schedule){
    bool isValid = true;

    for(int firstC = 0; firstC < schedule.size(); firstC++){
        if (isValid == false)
            break; //no need to continue, schedule is not valid, toss out

        for (int secondC = firstC+1; secondC < schedule.size(); secondC++){
            QString secondStr = schedule[secondC].day + " " + schedule[secondC].classStartTime + " - " + schedule[secondC].classEndTime;
            QStringList splitStr = secondStr.split(" ", QString::SkipEmptyParts); //restraints for the second class

            //Thursday is a special day, since it is represented as an R, not T
            if (splitStr.size() == 1){ //only one field, thus only one day is blocked
                if (splitStr[0].compare("Thursday", Qt::CaseInsensitive) == 0 &&
                         schedule[firstC].day.contains("R", Qt::CaseInsensitive)){
                    isValid = false;
                    break;
                }
                else if (schedule[firstC].day.contains(splitStr[0][0], Qt::CaseInsensitive) &&
                         splitStr[0].compare("Thursday", Qt::CaseInsensitive) != 0){
                    //not valid schedule then
                    isValid = false;
                    break; //no need to continue the loop, already removed it
                }
                else{
                    //reached when blocking a day, but no class section for that day, no need to do anything
                }
            }
            else if (splitStr.size() == 4){ //has more than one field; meaning has day(s) and time for the class
                bool found = false;
                //first check if the day of the week is going to be blocked
                for (int i = 0; i < splitStr[0].size(); i++){
                    if (schedule[firstC].day.contains(splitStr[0][i], Qt::CaseInsensitive)){
                        found = true;
                        break;
                    }
                }
                if (found == true){ //need to block this class if the time is correctly chosen:
                    //now check if the blocked off time is contained within the current section's class time
                    if ( (splitStr[1] > schedule[firstC].classStartTime && splitStr[1] < schedule[firstC].classEndTime) ||
                            (splitStr[1] == schedule[firstC].classStartTime) ||
                            (splitStr[1] < schedule[firstC].classStartTime && splitStr[3] >= schedule[firstC].classEndTime) ||
                            (splitStr[1] < schedule[firstC].classStartTime && splitStr[3] < schedule[firstC].classEndTime && splitStr[3] > schedule[firstC].classStartTime) ){
                        //remove from list
                        isValid = false;
                        break;
                    }
                }
            }
        }
    }
    return isValid;
}


/* Function Name: incrementCurrentIndex
 * Description:  This function simply increases the current index by one, does not handle if it gets too big.
 *               My previous attempt at having this function handle the logic did not end well, as it was simpler
 *               to handle elsewhere.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void BuildSchedulesList::incrementCurrentIndex(){
    currentIndex++;
}
