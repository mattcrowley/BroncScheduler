/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: DisplayDetailedClassInfo.h
 * Description: This class is called when the user double clicks a cell in their schedule, which brings up
 *              all the info for that specific section. This prints and displays all the data for the user.
 */

#include "displaydetailedclassinfo.h"
#include "ui_displaydetailedclassinfo.h"

DisplayDetailedClassInfo::DisplayDetailedClassInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayDetailedClassInfo)
{
    ui->setupUi(this);
}

DisplayDetailedClassInfo::~DisplayDetailedClassInfo()
{
    delete ui;
}

/* Function Name: setCourseInfo
 * Description: This function will run when the user double clicks a table cell in their schedule, showing detailed
 *              info for that certain class.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Takes in the courseInfo value for the selected class.
*/
void DisplayDetailedClassInfo::setCourseInfo(courseInfo item){
    ui->subjBox->setText(item.subject);
    ui->numBox->setText(QString::number(item.number));
    ui->titleBox->setText(item.courseTitle);
    ui->profBox->setText(item.instructor);

    if (item.day.size() > 1){
        ui->timeBox->setText(item.day + " " + item.classStartTime + " - " + item.classEndTime);
        ui->locBox->setText(item.classLocation);
    }
    else{ //handle classes that only meet online
        ui->timeBox->setText("ONLINE COURSE");
        ui->locBox->setText("ONLINE ONLY");
    }

    ui->seatsBox->setText(QString::number(item.enrolledCurrently) + "/" + QString::number(item.totalSeats));
    ui->descBox->setText(item.description.simplified());
}
