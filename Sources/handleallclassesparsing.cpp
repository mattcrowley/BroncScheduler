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

#include "handleallclassesparsing.h"
#include <QScrollBar>


HandleAllClassesParsing::HandleAllClassesParsing(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::SelectSemester)
{
    numberOfCourseFields = 0;
    subjIndex = 0; //which subject we are looking for (defaults to "select")
    numTotalCourses = 0; //for debugging purposes
    numPages = 0; //used for the message when loading classes
    maxPages = 1;

    canAddNewClassForm = true;
    preferOnlineClasses = false;

    this->setParent(parent);

    m_manager = new QNetworkAccessManager(this);

    //used when searching for sections of a certain subject
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(replyFinished(QNetworkReply*)));
}

HandleAllClassesParsing::~HandleAllClassesParsing(){
    //qDebug() << "deleting handleparsing";
    //delete ui;
    deleteLater();

}

/* Function Name: start
 * Description: This function is called when we selected a semester, simply inits it with a single class field.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: The subjects we have found, and the semester we chose.
*/
void HandleAllClassesParsing::start(QList<CoursesOfferedList> subjectList, int semesterID, Ui::SelectSemester *u){
    mSubjectList = subjectList;
    chosenSemesterCode = semesterID;
    ui = u;
    addNewClassField(); //init with a class field already loaded
    canAddNewClassForm = false;
}


/* Function Name: getWebPage
 * Description: This function will load the UTPA webpage, and then waits for it to finish loading, then calls reply finished.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Takes in a string of the webpage, and the newest form added, so we can update the current progress display.
*/
//this function will call the "replyFinished" function when the html was read in
void HandleAllClassesParsing::getWebPage(QString webpage, addClassWidget *newForm){
    newestFormAdded = newForm;
    QNetworkReply *r = m_manager->get(QNetworkRequest(QUrl(webpage)));
    waitForSignal(m_manager, SIGNAL(finished(QNetworkReply*)), 50000);
    connect(r, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(test(QNetworkReply::NetworkError)));
}


/* Function Name: replyFinished
 * Description: Called when we get a response from the webpage being loaded, which means a valid internet connection.
 *              This function will acquire all data for all classes in the chosen subject, and save them for later use. This
 *              requires multiple pages often times, so it recursively calls itself at times.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Network Reply, saying that it was successful, which contains all data we need.
*/
void HandleAllClassesParsing::replyFinished(QNetworkReply *reply){
    QByteArray data=reply->readAll();
    QString str(data);

    //qDebug() << "reading " << mSubjectList.at(subjIndex).subjectName;

    if (str == ""){
        qDebug() << "Error. Internet not connected";
        QMessageBox::critical(this, "Error. Internet not connected", "Error, internet not connected");
    }
    //else if (){ //this should test if the website is offline for maintenance (occurs at 3am usually)

    //}
    else{
        numPages++;

        //now do something with str: split it
        handleHtmlString(str);

        //now determine if a new page exists:
        //if any page, it will be surrounded by: <a class="icon-link next-table" href="<URL>"
        QRegularExpressionMatch match;

        QRegularExpression regex("<div class=\"table-pagination\">(.*)<a class=\"icon-link next-table\" href=\"(.*)\">Next Page", QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatchIterator i = regex.globalMatch(str);

        while (i.hasNext()) {
            match = i.next();
            //qDebug() << "next" << match.captured(1);

            //match.captured(1) contains our max page num, need to split it
            if (maxPages == 1){
                QString pageStr = match.captured(1);
                QStringList pages = pageStr.split("/");

                //now pages[1] contains the num we want
                maxPages = pages[1].trimmed().toInt();

                qDebug() << "num pages: " << maxPages;

                //now update the qprogressdialog with how many pages max to look up
                newestFormAdded->setQProgressMaxValue(maxPages);
            }

            //now update qprogressdialog (actual page we are at) in addClassWidget
            newestFormAdded->updateQProgressDialogValue(numPages);

            // use match
            QString newURL = "https://webapps.utpa.edu"; //generic url
            //qDebug() << match.captured(2);

            newURL += match.captured(2);

            //qDebug() << newURL;

            getWebPage(newURL, newestFormAdded); //now go to parse the next webpage...
        }

        numPages = 0;
        //reset max num pages:
        maxPages = 1;
        newestFormAdded->setQProgressMaxValue(1);
    }
}


/* Function Name: handleHtmlString
 * Description: This function calls another function to get the class info for each of the classes listed on
 *              the current page into a format more readable. Passes in a str containing the info for one
 *              class only to another function to split up further.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: The str containing all class information
*/
void HandleAllClassesParsing::handleHtmlString(QString str){
    QStringList split = str.split("<td><a class=\"trigger\" id=");

    //qDebug() << split.size();

    if (split.size() > 1){
        //now use each of 1->split.size()-1 to get the 25 classes max per page
        for (int i = 1; i <= split.size()-1; i++)
            deriveInfo(split[i]); //now gets the info from one single class at a time
    }
    else{
        qDebug() << "no classes for this course";
    }
}

/* Function Name: deriveInfo
 * Description: This function takes the info for one specific section, saving all data for later.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: String containing HTML with class section data.
*/
//takes in a string of one single class of info to derive
void HandleAllClassesParsing::deriveInfo(QString string){
    courseInfo newC;

    QStringList classInfo = string.split("<td>", QString::SkipEmptyParts);

    //set the subject:
    newC.setSubject(this->mSubjectList[this->subjIndex].subjectName);

    //classInfo[2] has courseNumber
    //classInfo[3] has sectionNumber
    //classInfo[4] has CRN
    //classInfo[5] has class title
    //classInfo[6] has professorName
    //classInfo[7] has classMeetingTime
    for (int i = 2; i < 8; i++){
        newC = getCourseInfoFrom(newC, classInfo, i);
    }

    //classInfo[7] has last values, need to split it up. Has multiple fields to it
    newC = handleExtraInfo(newC, classInfo[7], "<strong>(.*)</strong>(.*)<.*>(.*)");

    //now add the course to the subject's list!
    mSubjectList[subjIndex].coursesOffered.append(newC);
    numTotalCourses++;
}



/* Function Name: handleExtraInfo
 * Description: This function handles splitting the last part of the string, that contains the more
 *              difficult data, such as department, how many seats, its location, if a lab, or its course description.
 * Preconditions: NONE
 * Postconditions: All data is correctly interpreted and stored into newC
 * Inputs: The course to save info to, string containing the data, and parseStr needed to split the string.
*/
//takes in the string that has dept, college, descr, etc in it
//then the way to parse to get the right info
courseInfo HandleAllClassesParsing::handleExtraInfo(courseInfo newC, QString str, QString parseStr){
    int index = 0; //0-6, each has different data to take
    bool isLab = false;
    bool isUnderGradC = false;
    QStringList seats;
    QStringList q;
    QString loc = "";
    QString num;

    QString l0, l1, l2, l3;

    QRegularExpressionMatch match;

    QRegularExpression regex(parseStr);
    QRegularExpressionMatchIterator i = regex.globalMatch(str);

    while (i.hasNext()) {
        match = i.next();

        QString textYouWant = match.captured(2);
        l0=match.captured(0);
        l1=match.captured(1);
        l2=match.captured(2);
        l3=match.captured(3);
        //qDebug() << textYouWant;

        switch(index){
            case 0: //department
                newC.setDepName(textYouWant.trimmed());
                break;
            case 1: //College
                newC.setCollegeName(textYouWant.trimmed());
                break;
            case 2: //instruction type
                num = QString::number(newC.number);
                if (num[1] == '1'){ //1 credit class is a lab
                    isLab = true;
                }
                newC.setIsLab(isLab);
                break;
            case 3: //location, need to do stuff here...
                q = textYouWant.split(QRegExp("(\\< |\\>)"));

                if (q.size() >= 3){ //success in splitting
                    loc = copyString(q[1].trimmed());
                    loc += " " + q[2].trimmed();
                }
                newC.setClassLoc(loc);
                break;
            case 4: //enrolled / total seats
                seats = (textYouWant.trimmed()).split("/");

                if (seats.size() >= 2){
                    newC.setEnrolledCur(seats[0].toInt());
                    newC.setTotalSeats(seats[1].toInt());
                }
                else
                    qDebug() << "ERROR IN SPLITTING STRING!";

                break;
            case 5: //class credits, not really necessary
                break;
            case 6: //class level
                if (newC.number < 5000)
                    isUnderGradC = true;
                newC.setIsUnderGradClass(isUnderGradC);
                break;
        }
        index++;
        isLab = false; //reset these two values for next loop
        isUnderGradC = false;
    }

    QStringList l = str.split("<p class=\"more-info4\">");
    QStringList ll = l[1].split("</strong>");
    QStringList lll = ll[1].split("</p>");

    //qDebug() << lll[0].trimmed(); this is the description
    newC.setDesc(lll[0]);
    return newC;
}


/* Function Name: getCourseInfoFrom
 * Description: This function deals with the simple data, not enclosed by much HTML, and sets the CRN, section, instructor
 *              start and ending time. Each time the function is called, it gets one of the pieces of info.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Course to save info to, the list of info we need, and the index containing the needed value (2-7).
*/
courseInfo HandleAllClassesParsing::getCourseInfoFrom(courseInfo newC, QStringList info, int index){
    //now scan classInfo until reaching </td> (before I knew a bit of how to split strings using expressions
    QString crsInfo = copyString(info[index]); //data is enclosed by <td> </td>, so we want all data in between

    //now set the corresponding variable
    switch (index){
        case 2: newC.setNumber(crsInfo.toInt());
                break;
        case 3: newC.setSection(crsInfo.toInt());
                break;
        case 4: newC.setCRN(crsInfo.toInt());
                break;
        case 5: newC.setcourseTitle(crsInfo);
                break;
        case 6: newC.setInstructor(crsInfo);
                break;
        case 7: //now split the string to get days of class, startTime, and endTime
                QStringList temp = crsInfo.split(" ", QString::SkipEmptyParts);

                if (temp.size() >= 1){ //successfully split date
                    newC.setClassDay(temp[0].trimmed());

                    if (temp.size() == 4){ //has sTime and eTime //temp[2] has "-"
                        newC.setClassStartTime(temp[1].trimmed());
                        newC.setClassEndTime(temp[3].trimmed());
                    }
                }
                else{
                    //online class or does not yet have a time...
                }

                break;
    }
    return newC;
}


/* Function Name: copyString
 * Description: This function is used since the data needed is enclosed between <td> tags, so we want all in between that.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs:
*/
//copies the string up until a character <
QString HandleAllClassesParsing::copyString(QString copyFrom){
    QString final = "";

    for (int i = 0; i < copyFrom.size(); i++){
        if (copyFrom[i] != '<')
            final += copyFrom[i];
        else
            break;
    }

    return final;
}


/* Function Name: waitForSignal
 * Description: This function was created to allow the program to wait for a reply from the loaded webpage,
 *              as without it, it tries to load them all at the same time.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Where the call comes from, and how long it takes until a timeout occurs.
*/
void HandleAllClassesParsing::waitForSignal(QObject *sender, const char *signal, int timeout) {
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(timeout);
    timer.setSingleShot(true);

    loop.connect(sender, signal, SLOT(quit()));
    loop.connect(&timer, SIGNAL(timeout()), SLOT(quit()));

    timer.start();
    loop.exec();

    //return timer.isActive();
}

//debug function to test if we can handle timeouts or not
void HandleAllClassesParsing::test(QNetworkReply::NetworkError code){
    qDebug() << Q_FUNC_INFO << "Error" << code;
}


/* Function Name: updateRemainingClassForms
 * Description: This function is called when a class selection form is deleted at the user's request. Since we deleted
 *              a class added to our list, we now need to update the number next to most (if not all) of the visible class forms.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Takes in the deleted widget, so we skip that value when renumbering things.
*/
void HandleAllClassesParsing::updateRemainingClassForms(addClassWidget *deletedWidget){
    //now update each of the field's numForm label that states which class number it is:

    int num = 1; //the number for the cur pos of the changed field
    for (int i = 0; i < ui->verticalLayout->count(); i++){
        QWidget *t = ui->verticalLayout->itemAt(i)->widget();
        addClassWidget *a = dynamic_cast<addClassWidget *> (t);
        if (a == deletedWidget) //since we are deleting this form, we dont want to include in count
            continue;
        a->ui->numField->setText(QString::number(num));
        num++;
    }
}


/* Function Name: addNewClassField
 * Description: This function is called when trying to add a new class field. It shows a message saying you cannot
 *              add a new field if there are still course(s) with non-validated class info. This allows no one
 *              to have fields that do not correspond to a class shown.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void HandleAllClassesParsing::addNewClassField(){
    //qDebug() << "Number of total classes: " << chosenCourses.size();
    //qDebug() << "Number of forms: " << numberOfCourseFields;
    if (canAddNewClassForm == true){
        QScrollBar* scrollbar = ui->scrollArea->verticalScrollBar();
        addClassWidget *mWid = new addClassWidget(this, mSubjectList, chosenCourses.size()+1);
        ui->verticalLayout->setAlignment(Qt::AlignTop);
        ui->verticalLayout->addWidget(mWid); //should this be commented or not?

        //this scrollbar is a pain to scroll all the way down
        connect(scrollbar, SIGNAL(rangeChanged(int,int)), this, SLOT(moveScrollBarToBottom(int, int)));

        newestFormAdded = mWid;
        numberOfCourseFields++;
        canAddNewClassForm = false;
    }
    else{ //display a message to user stating WHY they cannot add a course field
        QMessageBox::warning(this, "Cannot Add Course Field", "Warning: Cannot add a course field when there is still a course field whose class has not been chosen yet.");
    }
}

void HandleAllClassesParsing::moveScrollBarToBottom(int min, int max){
    ui->scrollArea->verticalScrollBar()->setValue(max);
}

QList<courseInfo> HandleAllClassesParsing::getClassesFromSubject(int index){
    //qDebug() << mSubjectList[index].coursesOffered.count();
    return mSubjectList[index].coursesOffered;
}


/* Function Name: showConstraintsMainMenu
 * Description: This function shows the menu to allow constraints to be chosen. Then receives data on whether a user prefers
 *              online courses or not from their choices.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void HandleAllClassesParsing::showConstraintsMainMenu(){
    //qDebug() << "Showing restriction main menu";
    ClassRestrictionsMainMenu *restMenu = new ClassRestrictionsMainMenu(this, this);
    restMenu->setAttribute(Qt::WA_DeleteOnClose);

    //restMenu->show();
    restMenu->exec(); //this "pauses" the main window, stealing focus, not allowing user to mess with the main window

    if (restMenu->preferOnlineClasses)
        preferOnlineClasses = true;
    else
        preferOnlineClasses = false;

    //qDebug() << "done with restraints menu";
}



/* Function Name: addChosenClass
 * Description: This function will first test to see if the class we try to add is already in our selection, then refuses to add it.
 *              Else it adds the course, and displays a message, and allows you to add a new class form if you desire.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: The course we need to see if it is already in our chosenCourses list is passed in.
*/
bool HandleAllClassesParsing::addChosenClass(const courseInfo newC){
    if (chosenCourses.contains(newC) == false){
        chosenCourses.append(newC);
        qDebug () << "Added class.";
        QMessageBox::information(this, "Class Successfully Added", "Class Titled\n" + newC.courseTitle.trimmed() + "\nwas added to the list of chosen classes.");

        if (chosenCourses.size() == numberOfCourseFields){
            canAddNewClassForm = true;
        }
        else
            canAddNewClassForm = false;

        return true;
    }
    else{
        qDebug() << "Error. That course was already added";
        QMessageBox::warning(this, "Course Already Chosen", "Course not added. The course called \n" + newC.courseTitle.trimmed() + "\nwas already in the list.");
        return false;
    }
}



/* Function Name: deleteClassForm
 * Description: This function removes the chosen course from the list, if it is in there, and then determines if we
 *              should remove the course field or just clear the data.
 *              Returns true if we should delete the form, else false for dont delete.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Takes in the courseInfo of the course to delete.
*/
bool HandleAllClassesParsing::deleteClassForm(courseInfo newC){
    bool canDelete = true;

    if (chosenCourses.contains(newC) == true){ //delete this course from the chosenCourses list
        chosenCourses.removeOne(newC);
    }

    //now determine if we should remove the form or not...
    if (numberOfCourseFields > 1){ //remove the field
        numberOfCourseFields--;

    }
    else{ //only one field in the form
        canDelete = false;
    }

    if (numberOfCourseFields == chosenCourses.size())
        canAddNewClassForm = true;
    else
        canAddNewClassForm = false;

    return canDelete;
}


/* Function Name: addAllSectionsToCourseList
 * Description: This function will take our basic info in our chosenCourses list, and convert it to actual usable data.
 *              This new data will be stored in a new list called chosenCourseList, which contains not just the title and subject,
 *              but instructor info, and more.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void HandleAllClassesParsing::addAllSectionsToCourseList(){
    int index = 0;
    foreach(courseInfo c, chosenCourses){
        //search master list for the correct course...
        foreach(CoursesOfferedList col, mSubjectList){
            if (col.subjectName == c.subject){ //correct subject matches, so add each section
                chosenCourseList.append(CoursesOfferedList(col.subjectName));

                foreach(courseInfo course, col.coursesOffered){
                    if (course.courseTitle == c.courseTitle && course.number == c.number){ //fairly certain I have a function that does this...
                        this->chosenCourseList[index].coursesOffered.append(course); //add the section
                    }
                }
                break;
            }

        }
        index++;
    }
}



/* Function Name: resetData
 * Description: This function resets the handleAllClassesParsing data when we go back and forth between different semesters.
 *              It would not be good to save the data between different semesters.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs:
*/
void HandleAllClassesParsing::resetData(){
    numberOfCourseFields = 0;
    subjIndex = 0; //which subject we are looking for (defaults to "select")
    numTotalCourses = 0; //for debugging purposes
    numPages = 0;
    maxPages = 1;

    canAddNewClassForm = true;
    preferOnlineClasses = false;

    chosenCourses.clear();
    chosenCourseList.clear();
    classConstraints.clear();
}
