/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: HelpMessageDialog
 * Description: This class contains a single message that will display a help message at any time, explaining briefly
 *              how to operate the program. Implemented as a class due to easy reuse, and QT does not like
 *              messageboxes shown with text too long.
 */

#include "helpmessagedialog.h"
#include "ui_helpmessagedialog.h"

HelpMessageDialog::HelpMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpMessageDialog)
{
    ui->setupUi(this);
}

HelpMessageDialog::~HelpMessageDialog()
{
    delete ui;
}
