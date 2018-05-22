/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: HandleAllClassesParsing
 * Description: This class is a huge one. It started out parsing UTPA's webpage for all of its data, and ended up containing
 *              some of the logic of the application's process. It contains functions to ensure we can actually
 *              add a class field, show chosen constraints, add class to the list of classes chosen, delete those classes,
 *              and the logic to update the numbers next to each chosen class.
 *
 *              Basically, it acts as the container of all the logic for where you select classes, get their data from
 *              UTPA's webpage, and restraints chosen.
 */

#ifndef HANDLEALLCLASSESPARSING_H
#define HANDLEALLCLASSESPARSING_H

#include <QWidget>
#include <QMessageBox>

#include <QStackedWidget>
#include "coursesofferedlist.h"
#include <QtWebKit>
#include "ui_selectsemester.h"
#include "addclasswidget.h"
#include "algorithm"
#include "classrestrictionsmainmenu.h"

class addClassWidget;

class HandleAllClassesParsing : public QStackedWidget
{
    Q_OBJECT
public:
    explicit HandleAllClassesParsing(QWidget *parent = 0);
    ~HandleAllClassesParsing();

    void start(QList<CoursesOfferedList> subjectList, int semesterID, Ui::SelectSemester *u);

    QList<CoursesOfferedList> mSubjectList;
    QList<courseInfo> getClassesFromSubject(int index);

    int subjIndex ;
    int numTotalCourses ; //used for debugging
    int numPages;
    int chosenSemesterCode;
    QList<QString> classConstraints;
    QList<courseInfo> chosenCourses; //contains basic info on chosen courses
    QList<CoursesOfferedList> chosenCourseList; //contains all sections of each chosen course, more in depth
    int numberOfCourseFields;
    bool preferOnlineClasses;

    bool addChosenClass(const courseInfo newC);
    bool deleteClassForm(courseInfo newC);
    bool canAddNewClassForm; //to see if we can actually add a new class, or if one still needs to be selected

    void addAllSectionsToCourseList(); //this populates chosenCourseList with all the class sections

    Ui::SelectSemester *ui;

    void resetData();


signals:

public slots:
    void addNewClassField();
    void getWebPage(QString, addClassWidget *newForm);
    void showConstraintsMainMenu();
    void test(QNetworkReply::NetworkError code);
    void moveScrollBarToBottom(int min, int max);

    //slot to update the label on the addClassWidget form upon deletion of said widget:
    void updateRemainingClassForms(addClassWidget *deletedWidget);

private slots:
    void replyFinished(QNetworkReply *);
    void waitForSignal(QObject *sender, const char *signal, int timeout);
    void handleHtmlString(QString);
    void deriveInfo(QString);

    courseInfo handleExtraInfo(courseInfo, QString, QString);

    courseInfo getCourseInfoFrom(courseInfo, QStringList, int);

    QString copyString(QString);


private:
    QNetworkAccessManager* m_manager;
    addClassWidget *newestFormAdded;

    int maxPages; //holds the total number of pages we need to traverse through to get all classes in a certain subject
};

#endif // HANDLEALLCLASSESPARSING_H
