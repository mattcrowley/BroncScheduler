/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: SelectSemester
 * Description: Another large, main class.
 *
 *              This class is basically the one that drives the whole program (after the main window). It handles switching frames
 *              in the stacked widget, as well as the logic for selecting a semester from a list.
 *
 *              It searches UTPA's webpage for semester info (found on many of the webpages), then loads them into a drop down
 *              menu to choose from. Can get the semesters dynamically at run time, or from a file.
 *
 *              It also handles the showing of each different schedule. It calls other functions to remove restrictions or invalid
 *              schedules, then shows them in a table one by one. Also includes the functionality for previous schedule, next schedule,
 *              and more buttons.
 *
 *              This was done later in development, when I realized Qt hates you, and refuses to allow renaming of files.
 *              This class handles the logic for the stackedWidget, which is 3 frames in one: loadSemesters, chooseClasses, and
 *              showSchedules. I did not realize all the logic had to be in one class, so it ended like this, with a name
 *              that does not necessarily describe everything. Should be called StackedWidgetLogic
 */

#ifndef SELECTSEMESTER_H
#define SELECTSEMESTER_H

#include <QWidget>
#include <QtWebKit>
#include <QWebPage>
#include <QWebView>
#include <QWebFrame>
#include <QStackedWidget>
#include "handleallclassesparsing.h"
#include "courseinfo.h"
#include "coursesofferedlist.h"
#include "ui_selectsemester.h"
#include "buildscheduleslist.h"


namespace Ui {
class SelectSemester;
}

class SelectSemester : public QStackedWidget
{
    Q_OBJECT

public:
    explicit SelectSemester(QWidget *parent = 0);

    QList<QList<courseInfo> > allPossibleSchedulesList; //this holds a list of courses which together make a single schedule
                                                    //the outer QList contains all possible schedules
    ~SelectSemester();

private slots:
    void on_submitButton_clicked();

    void on_findSemestersButton_clicked();
    void findSemestersDynamically(); //replaces old on_findSemestersButton_clicked()

    bool waitForSignal(QObject *sender, const char *signal, int timeout);
    void replyFinished(QNetworkReply*pReply);

    QStringList parseStuff(QString str, QString parseStr, int option);

    void on_addClassButton_clicked();
    void on_addConstraintsButton_clicked();
    void on_buildScheduleButton_clicked();

    void printAllSections(); //debug function
    void replyError(QNetworkReply::NetworkError code=QNetworkReply::ConnectionRefusedError); //debug function

    void on_nextScheduleButton_clicked();
    void outputScheduleToTable();

    int determineRow(int curRow, QString startTime, QString endTime);
    void determineColumns(int row, QString days, QTableWidgetItem *tableCell, int enrolledCur, int totalSeats);

    void on_showConstraintsButton_clicked();
    void on_prevScheduleButton_clicked();

    void on_goBackClassSelectionButton_clicked();
    void on_exportButton_clicked();


    void on_getSemestersDynamicallyButton_clicked();

    void on_scheduleTable_cellDoubleClicked(int row, int column);
    void on_deleteCurrentScheduleButton_clicked();

    void goBackToSelectSemestersOption();
    void viewHelpDialog();

    void closeLoadingSemestersDialog();

private:
    Ui::SelectSemester *ui;
    QNetworkAccessManager* m_manager;
    QList<int> semesterIntCode;

    QList<CoursesOfferedList> subjectList;
    HandleAllClassesParsing *h;
    BuildSchedulesList *createdSchedules;

    QFile semestersDataFile;
    QFile subjectListFile;

    QTextStream semestersDataFileStream;
    QTextStream subjectListFileStream;

    QString semestersDataFileName;
    QString subjectListFileName;

    QWidget *parentWindow;
    QProgressDialog *progress;
};


struct sortByNumberOfSections{
    bool operator() (CoursesOfferedList left, CoursesOfferedList right){
        return left.coursesOffered.size() < right.coursesOffered.size();
    }
};

#endif // SELECTSEMESTER_H
