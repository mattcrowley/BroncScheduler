/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: TimeRestrictionsDialog
 * Description: This class shows the 2 tables of times you can restrict, as well as the fields to block off certain dates.
 *              It allows functionality to click table cell sections, and change the background color.
 */

#ifndef TIMERESTRICTIONSDIALOG_H
#define TIMERESTRICTIONSDIALOG_H

#include <QDialog>
#include "QTableWidget"
#include "handleallclassesparsing.h"
#include "ui_timerestrictionsdialog.h"
#include "qdebug.h"
#include "qtablewidget.h"

namespace Ui {
class TimeRestrictionsDialog;
}

class HandleAllClassesParsing;

class TimeRestrictionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeRestrictionsDialog(HandleAllClassesParsing *h, QWidget *parent = 0);

    QList<QString> restrictionInfo;
    ~TimeRestrictionsDialog();

private slots:
    void handlePrevConstraints();

    int determineColumn(QTableWidget *table, QString textToFind);

    void toggleColor(int , int , QTableWidget *table, bool shouldAddRemove=true);

    void on_MWandTRTable_cellClicked(int row, int column);

    void on_sectionClicked(int logicalIndex);
    void on_allDayTableSectionClicked(int logicalIndex);

    void on_allDaysTable_cellClicked(int row, int column);

    void accepted();

    void handleDaysBlocked();
    void printRestrictionInfo();

private:
    Ui::TimeRestrictionsDialog *ui;
    HandleAllClassesParsing *mainParentCopy;
};

#endif // TIMERESTRICTIONSDIALOG_H
