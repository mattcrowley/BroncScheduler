/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: ViewCRNDialog
 * Description: This class shows the popup window containing all of your course registration numbers
 *              needed to sign up for your classes in ASSIST.
 */


#include "Headers/viewcrndialog.h"
#include "ui_viewcrndialog.h"
#include <qdebug.h>

ViewCRNDialog::ViewCRNDialog(QList<int> courseRegNum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewCRNDialog)
{
    ui->setupUi(this);
    this->courseRegNumbers = courseRegNum;

    //now load each item in the above list into the text field:
    loadTextFromList();

}

ViewCRNDialog::~ViewCRNDialog()
{
    delete ui;
}

void ViewCRNDialog::loadTextFromList(){
    qDebug() << "now loading text...";
    foreach (int crnNum, courseRegNumbers){
        ui->CRNField->append(QString::number(crnNum) + " ");
    }
}
