/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: AddClassWidget.h
 * Description: This class creates the add class widget, with all of its fields. It contains the functionality
 *              of subject changing (which loads classes dynamically from UTPA's webpage), class form deletion,
 *              and ensuring the course being added to the list has a course title and number that is valid.
 */

#include "Headers/addclasswidget.h"

//takes in a copy of the parent, the subjectList, and which form number the class is
addClassWidget::addClassWidget(HandleAllClassesParsing *newH, QList<CoursesOfferedList> subList, int numCourse, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::addClassWidget)
{
    ui->setupUi(this);
    this->setParent(parent);

    setSubjectComboBox(subList); //add subjects to the form created
    ui->numField->setText(QString::number(numCourse)); //set the label to display "1" or "2" for the position of the class, to help users

    h = newH; //save for future use
}


/* Function Name: setSubjectComboBox
 * Description: This function simply adds all subjects obtained from the UTPA webpage to the dropdown menu,
 *              to allow the user to select one of them.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: A list of subjects, like ACC, ACCT, etc. Used to populate the form drop down menu.
*/
void addClassWidget::setSubjectComboBox(QList<CoursesOfferedList> subList){
    //ui->subjectComboBox->addItems(subList);
    for(int i = 0; i < subList.count(); i++){
        ui->subjectComboBox->addItem(subList[i].subjectName);
    }
}


addClassWidget::~addClassWidget()
{
    //qDebug() << "deleted add class widget";
    delete ui;
}


/* Function Name: on_subjectComboBox_currentIndexChanged
 * Description: This function will be called when the user changes the index for the subject drop down.
 *              It takes the index obtained, and loads the selected subject's classes, either from memory
 *              if it was already selected, or loads it from UTPA's webpage dynamically.
 *
 *              A fix to a problem I noticed was disabling controls when the user changes the subject index.
 *              Without this, it loads all indexes it comes across when scrolling down without clicking on the
 *              subject dropdown directly.
 * Preconditions: The user selected an index other than the current one, and hopefully did not change indexes before
 *                the controls were disabled.
 * Postconditions: NONE
 * Inputs: Int containing the index selected by the user.
*/
void addClassWidget::on_subjectComboBox_currentIndexChanged(int index)
{
    ui->subjectComboBox->setEnabled(false); //to stop scrolling from screwing up results
    ui->classNameComboBox->clear();

    if (index == 0){ //chose the "Select" option, so just clear things
        ui->classNameComboBox->clear();
    }
    else{
        if (h->mSubjectList[index-1].coursesOffered.count() == 0){ //subject not searched before
            //show a wait message:
            QProgressDialog progress("Parsing HTML...", 0, 0, 1, this);
            p = new QProgressDialog(&progress);
            p->setValue(0);
            p->setMaximum(1);
            p->setCancelButton(0);
            p->setLabelText("Loading " + h->mSubjectList[index-1].subjectName + " courses...");
            p->setWindowTitle("Loading courses");
            p->setWindowModality(Qt::WindowModal); //not allowing input to the parent
            p->setWindowFlags(p->windowFlags() & ~Qt::WindowCloseButtonHint);
            p->setAutoClose(true);
            progressDialogTextConst = p->labelText();
            p->show();
            //end long wait message setup

            //get the subj index chosen (-1 because of "select" being the first option), then get the data
            h->subjIndex = index-1;
            h->getWebPage("https://webapps.utpa.edu/aa/dm/index.cfm?dm_name=&thecollege=SELECT&thesubject=" + ui->subjectComboBox->itemText(index) + "&semester=" + QString::number(h->chosenSemesterCode), this);
            h->mSubjectList[index-1].coursesOffered = h->getClassesFromSubject(index-1);

            //now sort those acquired classes according to their 4 digit code
            std::sort(h->mSubjectList[index-1].coursesOffered.begin(), h->mSubjectList[index-1].coursesOffered.end(), sortStuff());
        }

        QString tempCourseName = "";
        int countNumSections = 1;

        //add each course title to the comboBox
        foreach (courseInfo c, h->mSubjectList[index-1].coursesOffered){
            if (tempCourseName.compare(QString::number(c.number)+ " - " + c.courseTitle.trimmed(), Qt::CaseInsensitive) == 0){ //prev course is the same as this one!
                //delete last course added
                ui->classNameComboBox->removeItem(ui->classNameComboBox->count()-1); //removes at last location
                countNumSections++;
            }
            else{ //not the same as the previous class, so reset number of sections
                countNumSections = 1;
            }
            tempCourseName = QString::number(c.number)+ " - " + c.courseTitle.trimmed();
            this->ui->classNameComboBox->addItem(tempCourseName + " (" + QString::number(countNumSections) + ")");
        }

        if (ui->classNameComboBox->count() == 0){
            //qDebug() << "no classes for this course";
            ui->classNameComboBox->addItem("No classes found.");
        }
    }
    ui->subjectComboBox->setEnabled(true); //reenable now
}


/* Function Name: updateQProgressDialogValue
 * Description: This function updates the current progress message shown when loading classes from UTPA's webpage.
 *              It specifically updates which page we are on, allowing better updates for the user to see.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Takes in the current new value we are at.
*/
void addClassWidget::updateQProgressDialogValue(int val){ //which page we are currently looking at
    p->setValue(val);
    p->setLabelText(progressDialogTextConst + "\nPage " + QString::number(val) + " of " + QString::number(p->maximum()) );
}


/* Function Name: setQProgressMaxValue
 * Description: This function is used to determine how many pages we have in total to look at for loading all courses.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Takes in the determined max number of pages (int).
*/
void addClassWidget::setQProgressMaxValue(int max){
    p->setMaximum(max); //total number of pages
}



/* Function Name: on_checkCourseButton_clicked
 * Description: This is run when the user selects "Add" next to a chosen class. Ensures that the user selected a class subject
 *              and class title, and makes sure the class was not already added to the chosen classes by calling
 *              it's parent function.
 *
 *              Note that the course "Added" to the chosen selection of user's class simply contain the number and
 *              class title, nothing more. Detailed info is grabbed later, once they decide to build the schedule.
 * Preconditions: NONE
 * Postconditions: Either an appropriate error message is show, or the class is added.
 * Inputs: NONE
*/
void addClassWidget::on_checkCourseButton_clicked()
{
    if (ui->classNameComboBox->currentText() != ""){
        courseInfo newC;
        int curSubjIndex=-1;

        curSubjIndex = ui->subjectComboBox->currentIndex()-1;
        QString curClassText = ui->classNameComboBox->currentText(); //need to split this text, since it contains unnecessary info

        if (curClassText == "No classes found."){
            qDebug() << "No classes found, cannot add this course";
            QMessageBox::warning(this, "No classes for this subject", "Warning: This subject does not have any classes. Selected course was NOT added to the list of chosen courses.");
        }
        else{
            //split the info in curClassText it is of format: 1101 - CLASSTITLE (num)
            QStringList splitClassText = curClassText.split(" ", QString::SkipEmptyParts); //0=number, 1="-", 2 = class title

            //search for the correct class number (ex: 1101) from the current selected subject, and get the title for it
            foreach(courseInfo c, h->mSubjectList[curSubjIndex].coursesOffered){
                if (splitClassText[0].trimmed().toInt() == c.number){ //found our match:
                    newC.setcourseTitle(c.courseTitle); //splitClassText[2] is in multiple parts, easier to find this way
                    break;
                }
            }

            newC.setNumber(splitClassText[0].trimmed().toInt()); //save the basic info only!
            newC.setSubject(ui->subjectComboBox->currentText());

            //now call handleallclassesparsing function addChosenClass
            bool success = h->addChosenClass(newC);

            if (success == true){
                //now disable combo boxes so we cannot edit it at all
                ui->subjectComboBox->setEnabled(false);
                ui->classNameComboBox->setEnabled(false);
                ui->checkCourseButton->setEnabled(false); //also disable addcourse button
            }
        }
    }
    else{ //show a message saying that either subj or class title was not chosen
        QMessageBox::warning(this, "Subject or Course Title not chosen", "Warning: Cannot validate course. Either the subject or class section was not selected.");
    }
}


/* Function Name: on_deleteClassButton_clicked
 * Description: This function is called when the user hits the red X next to the class selection form. It will either delete
 *              the form entirely (if blank and not the only form), delete the info (if it contains something), or do nothing.
 * Preconditions: The red X button was clicked.
 * Postconditions: NONE
 * Inputs: NONE
*/
void addClassWidget::on_deleteClassButton_clicked()
{
    //first is see if class was selected and has been added to our chosen course selection, delete it from the selection
    if (ui->classNameComboBox->currentText() != "" && ui->classNameComboBox->isEnabled() == false){
        courseInfo newC;

        if (ui->classNameComboBox->currentText() != "No classes found."){
            newC.setNumber(h->chosenCourses.at( (ui->numField->text().toInt()) -1).number); //get the class that we need to delete from the list using its index-1
            newC.setcourseTitle(h->chosenCourses.at( (ui->numField->text().toInt()) -1).courseTitle);
        }

        bool success = h->deleteClassForm(newC); //tests if we can delete this form entirely, or just reset its fields

        //show message stating the class that has been deleted
        QMessageBox::information(this, "Class Deleted", "Class called\n" + newC.courseTitle.trimmed() + "\nwas deleted.");

        if (success == true){ //delete the form
            //now we need to update the numForm label for all classes (one before, and all after):
            emit h->updateRemainingClassForms(this);

            //finally delete this class form
            delete this;
        }
        else{ //only reenable the fields and set back to default
            ui->subjectComboBox->setCurrentIndex(0);
            ui->subjectComboBox->setEnabled(true);

            ui->classNameComboBox->setCurrentIndex(0);
            ui->classNameComboBox->setEnabled(true);

            ui->checkCourseButton->setEnabled(true);
        }
    }

    //if we did not add the chosen course to our final list, simply remove the data, don't delete the field
    else if (ui->classNameComboBox->currentText() != "" && ui->classNameComboBox->isEnabled() == true ){
        //qDebug() << "removing non-added class";
        ui->subjectComboBox->setCurrentIndex(0);
        ui->subjectComboBox->setEnabled(true);

        ui->classNameComboBox->setCurrentIndex(0);
        ui->classNameComboBox->setEnabled(true);

        ui->checkCourseButton->setEnabled(true);
    }
    else if (h->numberOfCourseFields > 1 && h->numberOfCourseFields > h->chosenCourses.size()){
        //simply delete the extra form
        h->numberOfCourseFields--;
        h->canAddNewClassForm = true;
        delete this;
    }
}


/* Function Name: operator ==
 * Description: This function tests for equality between this form and another. Makes for easier and quicker comparisons when
 *              testing if we are adding the same class or not.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: The other class form to test against.
*/
bool addClassWidget::operator == (const addClassWidget &otherForm){
    if (this->ui->subjectComboBox->currentText() == otherForm.ui->subjectComboBox->currentText() &&
            this->ui->classNameComboBox->currentText() == otherForm.ui->classNameComboBox->currentText())
        return true;
    return false;
}


void addClassWidget::on_classSelectionWidget_destroyed()
{
    qDebug() << "destroyed class selectionWidget";
}
