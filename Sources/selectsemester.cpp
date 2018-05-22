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

#include "selectsemester.h"
#include "eliminaterestrictions.h"
#include <QScrollBar>
#include <QTextEdit>
#include <algorithm>
#include "exportoptions.h"
#include <QMenu>
#include "displaydetailedclassinfo.h"
#include "mainwindow.h"
#include "showcurrentrestrictionsdialog.h"
#include <QToolTip>
#include "helpmessagedialog.h"



SelectSemester::SelectSemester(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::SelectSemester)
{
    ui->setupUi(this);
    this->setParent(parent);
    parentWindow = parent;
    //h = new HandleAllClassesParsing;

    this->setWindowTitle("Choose a Semester");
    parentWindow->setWindowTitle("Choose a Semester");

    ui->stackedWidget->setCurrentWidget(ui->chooseSemesterPage);
    semestersDataFileName = "../data/semestersInfo.txt"; //the path to the file we inspect to make things quicker
    subjectListFileName = "../data/subjectList.txt";

    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(replyFinished(QNetworkReply*)));
    //h = new HandleAllClassesParsing(this);
}

SelectSemester::~SelectSemester()
{
    //qDebug() << "deleting select semesters ";
    //delete h;
    //delete ui;
    deleteLater();
}


/* Function Name: on_submitButton_clicked
 * Description: This is called when the Choose Classes button is pressed, it checks to make sure a semester was selected,
 *              then brings you to the course selection screen.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::on_submitButton_clicked()
{
    if (ui->listOfSemesters->count() > 0){
        int id = ui->listOfSemesters->currentIndex();  //gets index of choice

        if (id > semesterIntCode.size()){
            //qDebug () << "ERROR INVALID SEMESTER";
            QMessageBox::critical(this, "Error Invalid Semester Selected", "Error, Invalid Semester Selected.");
        }
        else{
            h = new HandleAllClassesParsing(this);

            id = semesterIntCode[id]; //used when loading webpages

            ui->chooseClassesPage->setEnabled(true);
            ui->chooseClassesPage->show();
            ui->stackedWidget->setCurrentIndex(1);
            this->setWindowTitle("Select classes for " + ui->listOfSemesters->currentText());
            parentWindow->setWindowTitle("Select classes for " + ui->listOfSemesters->currentText());

            h->start(this->subjectList, id, ui);
            qDebug() << "back to selectsemester";
        }
    }
    else{
        //no items selected yet! So show message to user telling them to select a semester
        QMessageBox::information(this, "Select a Semester", "Please select a semester from the list, or hit the \"Load Semesters from a File\" button to continue.");
    }
}



/* Function Name: on_findSemestersButton_clicked
 * Description: This function is called when the user hits load semesters button. This was changed halfway through to not be a
 *              dynamic semester loader, but to load from a file. If those files do not exist, then it shows a message
 *              stating you should click a button for more info, to load semesters dynamically.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::on_findSemestersButton_clicked()
{
    //first attempt to read from a file
    QFile semesters(semestersDataFileName);
    QFile subjList(subjectListFileName);
    ui->listOfSemesters->clear();
    this->semesterIntCode.clear();

    QFileInfo fileInfo; //this will hold the file name of the file we cannot open

    if (!semesters.open(QIODevice::ReadOnly)){
        fileInfo.setFile(semesters);
        ui->findSemestersButton->setEnabled(false);
        QMessageBox::information(0, "Error", fileInfo.fileName() + " could not be opened");
        //ui->getSemestersDynamicallyButton->setStyleSheet("background-color: rgb(255, 0, 0);");
        QToolTip::showText( ui->getSemestersDynamicallyButton->mapToGlobal(QPoint( 0, 0 )) , "Click here to find semesters",  ui->getSemestersDynamicallyButton,  ui->getSemestersDynamicallyButton->rect(), 5000);

    }
    else if (!subjList.open(QIODevice::ReadOnly)){
        fileInfo.setFile(subjList);
        ui->findSemestersButton->setEnabled(false);
        QMessageBox::information(0, "Error", fileInfo.fileName() + " could not be opened");
        //ui->getSemestersDynamicallyButton->setStyleSheet("background-color: rgb(255, 0, 0);");
        QToolTip::showText( ui->getSemestersDynamicallyButton->mapToGlobal(QPoint( 0, 0 )) , "Click here to find semesters",  ui->getSemestersDynamicallyButton,  ui->getSemestersDynamicallyButton->rect(), 5000);

    }
    else{ //successfully opened, read it, then load semesters
        QString semesterNameLine, semesterCodeLine;

        //the file format is: SemesterName <newLine> SemesterIDCode <newLine>
        while (!semesters.atEnd()){
            semesterNameLine = semesters.readLine().trimmed();
            semesterCodeLine = semesters.readLine().trimmed();

            //now add to specific dropdown locations:
            ui->listOfSemesters->addItem(semesterNameLine);
            this->semesterIntCode.append(semesterCodeLine.toInt());

            //gets rid of the pesky empty line
            semesters.readLine();
        }

        this->subjectList.clear();

        //now load the subjects from a file:
        QString subjLine;
        while (!subjList.atEnd()){
            subjLine = subjList.readLine().trimmed();

            this->subjectList.append(CoursesOfferedList(subjLine));
        }

        //now set the current index of semesters drop down to be the max:
        ui->listOfSemesters->setCurrentIndex(ui->listOfSemesters->count()-1);
        ui->findSemestersButton->setEnabled(true);
    }
}


/* Function Name: findSemestersDynamically
 * Description: This function will read the UTPA's webpage, to load the semesters from one single page. It also at the same
 *              time finds the different subjects, and saves them to a file for easier loading.
 * Preconditions: NONE
 * Postconditions: Semesters and subjects found and saved to a file.
 * Inputs: NONE
*/
void SelectSemester::findSemestersDynamically(){
    //show a message while loading information
    progress = new QProgressDialog("Loading Semesters...", 0, 0, 1, this);
    progress->setWindowTitle("Looking for Semesters...");
    progress->setWindowFlags(progress->windowFlags() & ~Qt::WindowCloseButtonHint);
    progress->show();

    ui->findSemestersButton->setEnabled(false);
    this->subjectList.clear();
    ui->listOfSemesters->clear();

    semestersDataFile.setFileName(semestersDataFileName);
    subjectListFile.setFileName(subjectListFileName);

    semestersDataFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    subjectListFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

    semestersDataFileStream.setDevice(&semestersDataFile);
    subjectListFileStream.setDevice(&subjectListFile);

    //load the webpage to get info
    QNetworkReply *r = m_manager->get(QNetworkRequest(QUrl("https://webapps.utpa.edu/aa/dm/")));
    waitForSignal(m_manager, SIGNAL(finished(QNetworkReply*)), 1000);
    connect(r, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

//used for debugging purposes, no longer of any use
void SelectSemester::replyError(QNetworkReply::NetworkError code){
    qDebug() << Q_FUNC_INFO << "Error" << code;
}


/* Function Name: waitForSignal
 * Description: This function will wait for the webpage to load, before moving onto the next.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Takes in an int timeout, to decide how long we should wait before saying page could not be loaded.
*/
bool SelectSemester::waitForSignal(QObject *sender, const char *signal, int timeout) {
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(timeout);
    timer.setSingleShot(true);

    loop.connect(sender, signal, SLOT(quit()));
    loop.connect(&timer, SIGNAL(timeout()), SLOT(quit()));
    timer.start();
    loop.exec();

    return timer.isActive();
}


/* Function Name: replyFinished
 * Description: This function is called when the webpage HTML was returned and loaded. It then parses the webpage to get
 *              semesters and subjects.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: The reply from the webpage.
*/
void SelectSemester::replyFinished(QNetworkReply *pReply){
    QTime myTimer;
    myTimer.start();

    QByteArray data=pReply->readAll();
    QString str(data);

    //now parse the html:
    QStringList s = str.split("<select id=\"semester-select\" name=\"semester\" style=\"display: none;\">");

    if (s.size() == 1){
        //show error message, could not acquire html source code
        qDebug() << "Either the internet is not connected, or the website is down";
        QMessageBox message;
        message.setTextFormat(Qt::RichText);
        message.setWindowTitle("Internet Connection Error OR UTPA Website is Offline");
        message.setText("Error: Could not acquire data. Possible errors:<br>"
                          "1. Internet is not connected. <br>2. UTPA webpage is down for maintenance\nCheck <a href='https://webapps.utpa.edu/aa/dm'>UTPA class list</a> for more info)");
        message.exec();

    }
    else{
        QStringList c = parseStuff(s[1], "<option value=\"(.*)\".*>(.*)</option>", 1); //get semesters

        ui->listOfSemesters->addItems(c);
        ui->listOfSemesters->setCurrentIndex(c.size()-1);

        int nMilliseconds = myTimer.elapsed();
        //qDebug() << nMilliseconds;

        c.clear();
        myTimer.start();

        //parse webpage to shorten time to get class subjects
        QStringList tempS = str.split(QRegExp("<select id=\"subject-select\" name=\"thesubject\" style=\"display: none;\">\\s+"));

        if (tempS.size() <= 1) //show error message, unable to split string
            qDebug() << "Error, empty string";
        else{
            parseStuff(tempS[1], "<option value=\"[A-Z]+\".*>(.*)</option>", 2); //get subjects
        }
        nMilliseconds = myTimer.elapsed();
        //qDebug() << this->subjectList.size();
        //qDebug() << nMilliseconds;
    }
    semestersDataFile.close();
    subjectListFile.close();

    ui->findSemestersButton->setEnabled(true);
    ui->getSemestersDynamicallyButton->setEnabled(true);

    emit this->closeLoadingSemestersDialog(); //since auto close does not work, had to do this
}


/* Function Name: closeLoadingSemestersDialog
 * Description: Qt does not like allowing just this, it needs a function encasing the close semesters command.
 * Preconditions: NONE
 * Postconditions: The loading semesters dialog box is closed.
 * Inputs: NONE
*/
void SelectSemester::closeLoadingSemestersDialog(){
    //qDebug() << "now closing dialog box";
    progress->close();
}



/* Function Name: parseStuff
 * Description: This function will do one of two things: get all semesters after parsing a webpage, or get all course subjects.
 *              It depends on the input value for option. Then we also output the data to a file.
 * Preconditions: That a website was loaded, and the correct string parse info was passed in.
 * Postconditions: NONE
 * Inputs: String containing the HTML for the webpage, string for the parsing to get the data we need, and an int of 1 for semester,
 *         or 2 for finding the course subjects.
*/
QStringList SelectSemester::parseStuff(QString str, QString parseStr, int option){
    QStringList c;
    QRegularExpressionMatch match;

    QRegularExpression regex(parseStr);
    QRegularExpressionMatchIterator i = regex.globalMatch(str);

    while (i.hasNext()) {
        match = i.next();

        // use match
        QString integerOrSubjName = match.captured(1); //either int or Accounting - ACC

        if (option == 2){ //get course subject titles (ACC, ACCT, etc)
            QStringList matchTemp;

            matchTemp = integerOrSubjName.split("-");

            //check to make sure "select" is not split!
            if (matchTemp.size() > 1 ){
                int index = 1;
                if (matchTemp.size() > 2) //stops pesky extra "-"
                    index = matchTemp.size()-1;

                matchTemp[index] = matchTemp[index].trimmed();
                QString t = matchTemp[index];

                subjectList.append(CoursesOfferedList(t));

                //now add this subject to a file:
                subjectListFileStream << t << endl;
            }
        }
        else{ //option 1, get semester code and semester name
            QString semesterOption = match.captured(2); //Spring 2015, etc
            if (semesterOption.size() > 5){ //want semester code
                c.append(semesterOption);
                semesterIntCode.append(integerOrSubjName.toInt());

                //write these two (semesterOption and integerOrSubjName) to the opened file:
                semestersDataFileStream << semesterOption << endl;
                semestersDataFileStream << integerOrSubjName << "\n" << endl;
            }
        }
    }
    return c;
}


void SelectSemester::on_addClassButton_clicked()
{
    h->addNewClassField(); //add class field when button was hit, test if it should be added or not
}

void SelectSemester::on_addConstraintsButton_clicked(){ //add constraints to your schedule
    h->showConstraintsMainMenu();
}


/* Function Name: on_buildScheduleButton_clicked
 * Description: This function is called when the user hits build schedules. It first removes any conflicts in any of its sections with the
 *              ones selected by the user. Then it sorts the courses by ascending order of how many sections it has, then it
 *              builds one single valid schedule. If it cannot, then it shows an error message. If it can, it moves
 *              to a new screen to show the schedule in a table.
 * Preconditions: Classes and restrictions have all been chosen.
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::on_buildScheduleButton_clicked()
{
    if (h->canAddNewClassForm == true){
        //first need to populate the chosenCourses list with actual class info:
        h->chosenCourseList.clear();
        h->addAllSectionsToCourseList();

        int index = 0;
        EliminateRestrictions elim;
        bool willContinue = true;

        //now remove all restrictions conflicting:
        if (h->classConstraints.size() != 0){
            foreach (CoursesOfferedList col, h->chosenCourseList){
                h->chosenCourseList[index].coursesOffered = elim.removeConflictsFromCourseRestrictions(col, h->classConstraints);
                index++;
            }
        }
        else{ //no constraints, possible to have lots of schedules! show a message informing the user
            int totalSchedules = 1;
            for(int i = 0; i < h->chosenCourseList.size(); i++)
                totalSchedules *= h->chosenCourseList[i].coursesOffered.size();
            QString msgText = "You have not selected any constraints. This leads to a max of " + QString::number(totalSchedules) + " possible schedules. Proceed anyways?";
            int choice = QMessageBox::question(this, "No Restraints Chosen", msgText);
            if (choice != QMessageBox::Yes){
                willContinue = false;
            }
        }

        if (willContinue != false){ //will continue despite any prompts given
            //now sort the h->chosenCourseList by ascending order (num of sections in each):
            std::sort(h->chosenCourseList.begin(), h->chosenCourseList.end(), sortByNumberOfSections());

            //qDebug() << "prefer online classes: " << h->preferOnlineClasses;

            if (h->preferOnlineClasses == true){
                //then we want to sort each course in chosenCourseList in ascending order:
                int curPos = 0;
                foreach (CoursesOfferedList listOfSections, h->chosenCourseList){
                    std::sort(listOfSections.coursesOffered.begin(), listOfSections.coursesOffered.end(), sortByAscendingOrderDay()); //default is ascending order
                    h->chosenCourseList[curPos].coursesOffered = listOfSections.coursesOffered;
                    curPos++;
                }
            }

            //next build one single schedule
            createdSchedules = new BuildSchedulesList(h->chosenCourseList);

            on_nextScheduleButton_clicked();

            if (createdSchedules->allSchedulesList.size() == 0){
                //qDebug() << "NO possible schedules exist. Please refine restriction criteria";
                QMessageBox::critical(0, "No Schedules Exist", "No schedules exist with chosen restrictions and classes. Please modify either one or the other.");
            }
            else{ //at least one schedule exists, so continue on to show the schedule(s)
                ui->showSchedulePage->setEnabled(true);
                ui->showSchedulePage->show();
                ui->stackedWidget->setCurrentIndex(2);
                ui->showSchedulePage->setWindowTitle("View all Schedules for " + ui->listOfSemesters->currentText());
                parentWindow->setWindowTitle("View all Schedules for " + ui->listOfSemesters->currentText());
            }
        }
    }
    else{ //message stating not all classes have been validated, or you need to choose at least 1 class
        QMessageBox::critical(0, "Validate all courses", "You need to either validate your course(s) or remove empty class forms.");
    }
}


void SelectSemester::printAllSections(){
    foreach(CoursesOfferedList col, h->chosenCourseList){
        foreach(courseInfo c, col.coursesOffered){
            qDebug() << c.courseTitle << ": " << c.day << " " << c.classStartTime << " - " << c.classEndTime;
        }
    }
}


/* Function Name: on_prevScheduleButton_clicked
 * Description: This function shows the previous schedule displayed, if one exists. Will go back if all schedules were already
 *              generated, resulting in a list you can scroll through either way.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::on_prevScheduleButton_clicked()
{
    if (createdSchedules->stillBuilding == false || createdSchedules->currentIndex > 1){ //by the time this button can be hit, the current index is actually at the position for the next schedule, so we need to decrement it TWICE
        if (createdSchedules->currentIndex == 1 || createdSchedules->currentIndex == 0){//loop around when finished creating all schedules, and hit "prev" when at index 1 (first schedule shown)
            createdSchedules->currentIndex = createdSchedules->allSchedulesList.size()-1;
        }
        else{
            createdSchedules->currentIndex -= 1; //this gets the "cur" schedule shown
            createdSchedules->currentIndex -= 1; //this gets the actual "prev" schedule we want
        }
        if (createdSchedules->currentIndex < 0)
            createdSchedules->currentIndex = 0;

        if (createdSchedules->allSchedulesList.size() == 0){
            //qDebug() << "NO MORE SCHEDULES PREVIOUS";
        }
        else{
            ui->scheduleTable->setRowCount(0); //clears the table of old data

            //determine the text below the schedule table:
            QString curSchedule = QString::number((createdSchedules->currentIndex)+1); //nice label for the user
            QString totalSchedules;

            if (createdSchedules->stillBuilding == true){ //unknown how many total schedules there are
                totalSchedules = "?";
            }
            else{
                totalSchedules = QString::number(createdSchedules->allSchedulesList.size());
            }
            ui->curScheduleText->setText("Schedule #" + curSchedule + "/" + totalSchedules);

            outputScheduleToTable();
            createdSchedules->incrementCurrentIndex(); //then this gets the "next" schedule, or previously the currently shown schedule
        }
    }
}


/* Function Name: on_nextScheduleButton_clicked
 * Description: This function will generate the next schedule possible, or it will go to a previously created one, depending
 *              on whether all schedules were generated or not already.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::on_nextScheduleButton_clicked()
{
    //prev button was hit before, so no need to generate a new one
    if (createdSchedules->stillBuilding == true && createdSchedules->currentIndex < createdSchedules->allSchedulesList.size()){
        qDebug() << "not generating new schedule";
    }
    else{ //generate a new schedule (if it exists)
        createdSchedules->createScheduleList(h->chosenCourseList); //return value not needed anymore due to making code more concise
    }
    ui->scheduleTable->setRowCount(0); //clear the table of old data

    //determine the text below the schedule table:
    QString totalSchedules;
    if (createdSchedules->stillBuilding == true){ //unknown how many total schedules there are
        totalSchedules = "?";
    }
    else{
        totalSchedules = QString::number(createdSchedules->allSchedulesList.size());

        //handle going past the final amount of classes, loop the number back around
        if (createdSchedules->currentIndex >= createdSchedules->allSchedulesList.size()){
            createdSchedules->currentIndex = 0;
        }
    }
    if (createdSchedules->allSchedulesList.size() == 0){
        //no schedules created, error! return back with nothing done here
    }
    else{
        QString curSchedule = QString::number((createdSchedules->currentIndex)+1);

        ui->curScheduleText->setText("Schedule #" + curSchedule + "/" + totalSchedules); //set the label under the table

        outputScheduleToTable();

        createdSchedules->incrementCurrentIndex(); //increment for next schedule the user may want to display
        //qDebug() << "NOW THE CUR index IS: " << createdSchedules->currentIndex;
    }
}


/* Function Name: outputScheduleToTable
 * Description: This function will create the table itself for the schedule. It analyzes the class schedule, sorts it by time,
 *              then outputs them to cells, and combines classes that have similar times for different days of the week.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::outputScheduleToTable(){
    //get the schedule info we are working with
    QList<courseInfo> temp = createdSchedules->allSchedulesList.at(createdSchedules->currentIndex);

    //sort in order of starting time, so the table rows will be in the correct order
    std::sort(temp.begin(), temp.end(), sortByStartTime());

    ui->scheduleTable->setRowCount(temp.size()); //how many rows need to be created? Max number is this, decreases if possible
    int i = 0; //counter for cur row to add to

    foreach (courseInfo item, temp){
        //these variables NEED to be in here, since each time the table takes possession of these objects when we set it
        QTableWidgetItem *tableCell = new QTableWidgetItem(""); //the data that the cell will hold
        QTableWidgetItem *tableHeader = new QTableWidgetItem("");

        //qDebug() << item.courseTitle << " " << item.day << " " << item.classStartTime << " - " << item.classEndTime;

        //add to the table
        tableCell->setText(item.getStringForSchedule());
        tableHeader->setText(item.classStartTime + " - " + item.classEndTime);

        if (item.classStartTime == "" && item.classEndTime == ""){ //online class, need to retitle the header and change days to show up for every day
            tableHeader->setText("Online Class");
            item.day = "MWTRF"; //to print in ALL cells
        }
        ui->scheduleTable->setVerticalHeaderItem(i, tableHeader); //create the new header in the row

        ui->scheduleTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->scheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

        //find whether the header for the row already exists and just add it to the same row, or create a new one
        //created to make the table as small as possible
        int newRow = determineRow(i, item.classStartTime, item.classEndTime);

        if (newRow != -1){ //not an invalid return value, thus the row was already found created
            //delete the row at pos "i":
            ui->scheduleTable->removeRow(i); //no need for an extra row with the same time
            i--; //deleted the row, so need to decrement the counter, no need for the extra row

            //classes w/multiple days need to be printed multiple times
            determineColumns(newRow, item.day, tableCell, item.enrolledCurrently, item.totalSeats);
            ui->scheduleTable->resizeRowToContents(newRow);

        }
        else{ //invalid return, row not found, continue with using row we added
            //classes w/multiple days need to be printed multiple times
            determineColumns(i, item.day, tableCell, item.enrolledCurrently, item.totalSeats);
            ui->scheduleTable->resizeRowToContents(i);
        }

        i++; //increment current row counter for the next class to be analyzed
    }
}


/* Function Name: determineRow
 * Description: This function will determine whether a row can be combined into another row, or if it is not possible.
 *              Done so the table size is as small as possible.
 *
 *              Returns -1 if you cannot combine rows, otherwise it returns the row that can be combined with the current selection.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: int for the curRow we are on, and strings containing starting and ending time
*/
int SelectSemester::determineRow(int curRow, QString startTime, QString endTime){
    QString search = startTime + " - " + endTime;

    for (int i = 0; i < curRow; i++){
        qDebug() << ui->scheduleTable->verticalHeaderItem(i)->text();
        if (i != curRow && (ui->scheduleTable->verticalHeaderItem(i)->text().compare(search) == 0) && search != ""){
            return i;
        }
    }
    return -1;
}



/* Function Name: determineColumns
 * Description: This function will take the passed in cell, and copy it over to all sections in the row, according to how
 *              many days the class is on, and determines which column(s) Monday is column 0, Tuesday is column 1, etc.
 *
 *              If the class is full, it also changes the background color to red.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Int containing the current row we are on, the days to copy information to, the cell containing info we want in the table,
 *         how many people are enrolled, and int for totalSeats the class contains, to use to color the cell.
*/
//this function determines which columns of the table the data needs to go in
void SelectSemester::determineColumns(int row, QString days, QTableWidgetItem *tableCell, int enrolledCur, int totalSeats){
    QTableWidgetItem *tempCell; //needed because the table takes "possession" of it each time
    int col = -1;

    for (int i = 0; i < days.size(); i++){
        tempCell = new QTableWidgetItem(tableCell->text());

        if (days[i] == 'M'){
            col = 0;
        }
        else if (days[i] == 'T'){
            col = 1;
        }
        else if (days[i] == 'W'){
            col = 2;
        }
        else if (days[i] == 'R'){
            col = 3;
        }
        else if (days[i] == 'F'){
            col = 4;
        }
        else{
            qDebug() << "Saturday class??";
            QMessageBox::critical(this, "Not Supported", "This program does not currently support Saturday classes");
        }

        ui->scheduleTable->setItem(row,col, tempCell);

        //now determine whether the class is full (red background), or has positions available (white background):
        if (enrolledCur >= totalSeats)//full class
            ui->scheduleTable->item(row, col)->setBackgroundColor(Qt::red);

    }

}

void SelectSemester::on_showConstraintsButton_clicked()
{
    qDebug() << "showing restrictions";
    foreach (QString s, h->classConstraints)
        qDebug() << s;

    ShowCurrentRestrictionsDialog curRes(h->classConstraints, this);
    curRes.exec();
}

void SelectSemester::on_goBackClassSelectionButton_clicked()
{
    ui->chooseClassesPage->setEnabled(true);
    ui->chooseClassesPage->show();
    ui->stackedWidget->setCurrentWidget(ui->chooseClassesPage);
    parentWindow->setWindowTitle("Select classes for " + ui->listOfSemesters->currentText());
}


/* Function Name: on_exportButton_clicked
 * Description: This function is called to show the different export options available to the user. Either
 *              show the CRN's, or export to PDF.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::on_exportButton_clicked()
{
    if (createdSchedules->currentIndex > 0){
        //build the list of the courseReg numbers:
        QList<int> courseNumbers;
        QList<courseInfo> temp = createdSchedules->allSchedulesList.at(createdSchedules->currentIndex-1);

        foreach (courseInfo item, temp){
            courseNumbers.append(item.CRN);
        }

        ExportOptions *expWindow = new ExportOptions(ui->scheduleTable, courseNumbers, this);
        expWindow->setAttribute(Qt::WA_DeleteOnClose);

        expWindow->exec();
    }
}


/* Function Name: on_getSemestersDynamicallyButton_clicked
 * Description: This function is called when you cannot load semesters from a file, and want to load them dynamically, as I originally
 *              required the users to do.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::on_getSemestersDynamicallyButton_clicked()
{
    int buttonPressed = QMessageBox::question(this, "Find Semesters Dynamically", "If you are not seeing the semester you are looking for, this program can search <a href='https://webapps.utpa.edu/aa/dm'>UTPA's webpage</a> <br><br>Would you like to load semesters from UTPA's website dynamically?", QMessageBox::Yes | QMessageBox::No);

    if (buttonPressed == QMessageBox::Yes){ //load semesters dynamically
        //qDebug() << "YES";
        ui->getSemestersDynamicallyButton->setEnabled(false);
        findSemestersDynamically();
    }
}



/* Function Name: on_scheduleTable_cellDoubleClicked
 * Description: This function is called when you double click a cell in the table showing your schedule.
 *              It then outputs all the data for the section, like teacher, time, spots open, etc, and shows it to you.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: The int for row and column of which cell was clicked.
*/
void SelectSemester::on_scheduleTable_cellDoubleClicked(int row, int column)
{
    courseInfo foundData;
    QTableWidgetItem *cellText = ui->scheduleTable->item(row, column);

    //make sure the cell actually has text in it, either 3 lines or none
    if (cellText != NULL && cellText->text() != ""){
        //now cellText contains 3 lines, need to split it up, find the title, match it to allSchedulesList, then output the data:
        QStringList cellListText = cellText->text().split(QRegExp("\\s+"), QString::SkipEmptyParts); //split by spaces gets: 0 = subj, 1=courseNum, 2 = "section"Sec", 3 = actual section number

        //need to get the currently shown schedule
        QList<courseInfo> temp = createdSchedules->allSchedulesList.at(createdSchedules->currentIndex-1);

        //now use index 0 and 1 and 3 to search:
        foreach (courseInfo curCourse, temp){
            if ( curCourse.subject.compare(cellListText[0], Qt::CaseInsensitive) == 0 &&
                curCourse.number == cellListText[1].toInt() &&
                curCourse.section == cellListText[3].toInt() )
            {
                //have a match, so we need to save this data, and output it:
                //qDebug() << "found the data in the cell!";
                foundData = curCourse;
                break;
            }
        }

        //now take foundData and output it:
        DisplayDetailedClassInfo *cellInfo = new DisplayDetailedClassInfo(this);
        cellInfo->setAttribute(Qt::WA_DeleteOnClose);
        cellInfo->setCourseInfo(foundData);
        cellInfo->exec();
    }
    else{
        //qDebug() << "null";
    }
}


/* Function Name: on_deleteCurrentScheduleButton_clicked
 * Description: This function will delete the currently shown schedule. It then generates the next schedule possible, if still generating
 *              them. If it finds no new schedules, it goes to the next schedule in the list (the first one). If there are no
 *              schedules left after deleting them, it shows an error message, then boots you back to the main screen.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::on_deleteCurrentScheduleButton_clicked()
{
    createdSchedules->allSchedulesList.removeAt(createdSchedules->currentIndex-1); //remove the schedule from the list
    createdSchedules->currentIndex-=1;
    on_nextScheduleButton_clicked(); //now next schedule generation

    //no new schedule generated? then show message to user, then go back to class selection screen
    if (createdSchedules->allSchedulesList.size() == 0){
        QMessageBox::critical(this, "All Schedules Deleted", "You have removed all possible schedules. Please go back to change your restrictions or classes");

        //qDebug() << "ending message, now back to class selection";

        on_goBackClassSelectionButton_clicked(); //booted to the class selection screen
    }
}


/* Function Name: goBackToSelectSemestersOption
 * Description: This function is called when the user selects it from the menu bar. Logic is placed here, even if the actual
 *              generated code is in the mainwindow file. Simply resets some data, and brings you back to the screen
 *              to reselect a semester.
 *
 *              Cannot be called on main screen, or chooseSemester page.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void SelectSemester::goBackToSelectSemestersOption(){
    if (ui->stackedWidget->currentWidget() != ui->chooseSemesterPage){ //make sure it is not called on the chooseSemester screen
        ui->chooseSemesterPage->setEnabled(true);
        ui->chooseSemesterPage->show();
        ui->stackedWidget->setCurrentWidget(ui->chooseSemesterPage);
        parentWindow->setWindowTitle("Choose a Semester");

        //now delete stuff:
        h->resetData();
        QLayoutItem *child;
        while ((child = h->ui->verticalLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }

        allPossibleSchedulesList.clear();
        createdSchedules = new BuildSchedulesList( QList<CoursesOfferedList> ());
    }
}

void SelectSemester::viewHelpDialog(){
    HelpMessageDialog helpMsg;
    helpMsg.exec();
}
