/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: ClassRestrictionsMainMenu.h
 * Description: This is the main menu class for the user's restrictions. From here they can prioritize online
 *              courses in their schedule, or open up a new window to block more specific times.
 */

#ifndef CLASSRESTRICTIONSMAINMENU_H
#define CLASSRESTRICTIONSMAINMENU_H

#include <QDialog>
#include "handleallclassesparsing.h"
#include "ui_classrestrictionsmainmenu.h"
#include "timerestrictionsdialog.h"

namespace Ui {
class ClassRestrictionsMainMenu;
}

class ClassRestrictionsMainMenu : public QDialog
{
    Q_OBJECT

public:
    explicit ClassRestrictionsMainMenu(HandleAllClassesParsing *h, QWidget *parent = 0);
    ~ClassRestrictionsMainMenu();

    bool preferOnlineClasses;

private slots:
    void on_chooseTimeRestrictionsButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::ClassRestrictionsMainMenu *ui;
    HandleAllClassesParsing *tempCopyParent;
};

#endif // CLASSRESTRICTIONSMAINMENU_H
