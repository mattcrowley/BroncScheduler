/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: ExportOptions
 * Description: This class shows the window when the user selects to export when showing the user's schedule.
 *              Implements the functionality for the view CRN's button, and creation of the PDF.
 */

#ifndef EXPORTOPTIONS_H
#define EXPORTOPTIONS_H

#include <QDialog>
#include "addclasswidget.h"
#include "ui_exportoptions.h"
#include "viewcrndialog.h" //to show the CRN's of the current schedule
#include <qprinter>
#include <qfiledialog.h>

namespace Ui {
class ExportOptions;
}

class ExportOptions : public QDialog
{
    Q_OBJECT

public:
    explicit ExportOptions(QTableWidget *outputTable, QList<int> courseRegNum, QWidget *parent = 0);
    ~ExportOptions();

private slots:
    void on_viewCRNsButton_clicked();

    void on_createCondensedPDFTableButton_clicked();

private:
    Ui::ExportOptions *ui;
    QList<int> courseRegNumbers;
    QTableWidget *tableCopy;
};

#endif // EXPORTOPTIONS_H
