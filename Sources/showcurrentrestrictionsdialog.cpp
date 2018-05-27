/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: ShowCurrentRestrictionsDialog
 * Description: This class shows a window containing all of your chosen restrictions.
 */

#include "Headers/showcurrentrestrictionsdialog.h"
#include "ui_showcurrentrestrictionsdialog.h"

ShowCurrentRestrictionsDialog::ShowCurrentRestrictionsDialog(QList<QString> res, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowCurrentRestrictionsDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Your Schedule Restrictions");
    curRestraints = res;

    std::sort(curRestraints.begin(), curRestraints.end()); //sort for better displaying

    //now display restraints in the table
    addRestraints();
}

ShowCurrentRestrictionsDialog::~ShowCurrentRestrictionsDialog()
{
    delete ui;
}

//this function loads the restraints into a simple text box for simple way to view any constraints
void ShowCurrentRestrictionsDialog::addRestraints(){
    foreach(QString str, curRestraints){
        ui->restraintsListBox->addItem(str);
    }
}
