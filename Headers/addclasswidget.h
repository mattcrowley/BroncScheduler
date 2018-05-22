/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: AddClassWidget.h
 * Description: This class creates the add class widget, with all of its fields. It contains the functionality
 *              of subject changing (which loads classes dynamically from UTPA's webpage), class form deletion,
 *              and ensuring the course being added to the list has a course title and number that is valid.
 */

#ifndef ADDCLASSWIDGET_H
#define ADDCLASSWIDGET_H

#include <QFrame>
#include "coursesofferedlist.h"
#include "handleallclassesparsing.h"
#include "ui_addclasswidget.h"
#include "qdebug.h"
#include <qprogressdialog.h>

namespace Ui { //to utilize the user interface
    class addClassWidget;
}

class HandleAllClassesParsing; //forward declaration

class addClassWidget : public QFrame
{
    Q_OBJECT

public:
    explicit addClassWidget(HandleAllClassesParsing *newH, QList<CoursesOfferedList> subList, int numCourse, QWidget *parent = 0);

    void setSubjectComboBox(QList<CoursesOfferedList> subList);


    void updateQProgressDialogValue(int val);
    void setQProgressMaxValue(int max);

    bool operator == (const addClassWidget &otherForm); //to verify if this class was already added

    ~addClassWidget();
    Ui::addClassWidget *ui; //moved to public to allow others parent to use

private slots:
    void on_subjectComboBox_currentIndexChanged(int index); //parse webpage based on index

    void on_checkCourseButton_clicked(); //ensure no other class of the same was added

    void on_deleteClassButton_clicked();

    void on_classSelectionWidget_destroyed();

private:
    HandleAllClassesParsing *h;
    QProgressDialog *p;

    QString progressDialogTextConst; //used to show that we are loading multiple pages of classes
};

#endif // ADDCLASSWIDGET_H
