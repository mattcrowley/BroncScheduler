/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: DisplayDetailedClassInfo.h
 * Description: This class is called when the user double clicks a cell in their schedule, which brings up
 *              all the info for that specific section. This prints and displays all the data for the user.
 */

#ifndef DISPLAYDETAILEDCLASSINFO_H
#define DISPLAYDETAILEDCLASSINFO_H

#include <QDialog>
#include "courseinfo.h"

namespace Ui {
class DisplayDetailedClassInfo;
}

class DisplayDetailedClassInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayDetailedClassInfo(QWidget *parent = 0);
    ~DisplayDetailedClassInfo();

    void setCourseInfo(courseInfo item); //item is passed in after generating it from data in a table

private:
    Ui::DisplayDetailedClassInfo *ui;
};

#endif // DISPLAYDETAILEDCLASSINFO_H
