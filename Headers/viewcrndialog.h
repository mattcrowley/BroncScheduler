/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: ViewCRNDialog
 * Description: This class shows the popup window containing all of your course registration numbers
 *              needed to sign up for your classes in ASSIST.
 */

#ifndef VIEWCRNDIALOG_H
#define VIEWCRNDIALOG_H

#include <QDialog>

namespace Ui {
class ViewCRNDialog;
}

class ViewCRNDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewCRNDialog(QList<int> courseRegNum, QWidget *parent = 0);
    ~ViewCRNDialog();
    void loadTextFromList();

private:
    Ui::ViewCRNDialog *ui;
    QList<int> courseRegNumbers;
};

#endif // VIEWCRNDIALOG_H
