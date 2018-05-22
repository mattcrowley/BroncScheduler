/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: ShowCurrentRestrictionsDialog
 * Description: This class shows a window containing all of your chosen restrictions.
 */

#ifndef SHOWCURRENTRESTRICTIONSDIALOG_H
#define SHOWCURRENTRESTRICTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class ShowCurrentRestrictionsDialog;
}

class ShowCurrentRestrictionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowCurrentRestrictionsDialog(QList<QString> res, QWidget *parent = 0);
    ~ShowCurrentRestrictionsDialog();

    void addRestraints();

private:
    Ui::ShowCurrentRestrictionsDialog *ui;
    QList<QString> curRestraints;
};

#endif // SHOWCURRENTRESTRICTIONSDIALOG_H
