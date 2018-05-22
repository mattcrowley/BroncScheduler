/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: ClassRestrictionsMainMenu.h
 * Description: This is the main menu class for the user's restrictions. From here they can prioritize online
 *              courses in their schedule, or open up a new window to block more specific times.
 */

#include "classrestrictionsmainmenu.h"

//this constructor sets up the UI, and updates already selected and saved values
ClassRestrictionsMainMenu::ClassRestrictionsMainMenu(HandleAllClassesParsing *h, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClassRestrictionsMainMenu)
{
    ui->setupUi(this);
    this->setParent(parent);
    this->setWindowTitle("Select a Time Restriction Option");
    tempCopyParent = h;

    //update the display if the user chose already to prefer online courses
    if (h->preferOnlineClasses == true){
        preferOnlineClasses = true;
        ui->preferOnlineClassesBox->setChecked(true);
    }
    else{
        preferOnlineClasses = false;
        ui->preferOnlineClassesBox->setChecked(false);
    }
}


ClassRestrictionsMainMenu::~ClassRestrictionsMainMenu()
{
    delete ui;
}


void ClassRestrictionsMainMenu::on_chooseTimeRestrictionsButton_clicked()
{
    TimeRestrictionsDialog *time = new TimeRestrictionsDialog(tempCopyParent, this);
    time->exec();
}

void ClassRestrictionsMainMenu::on_buttonBox_accepted()
{
    if (ui->preferOnlineClassesBox->isChecked() == true){
        preferOnlineClasses = true;
    }
    else
        preferOnlineClasses = false;
}
