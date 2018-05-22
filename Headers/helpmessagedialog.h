/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: HelpMessageDialog
 * Description: This class contains a single message that will display a help message at any time, explaining briefly
 *              how to operate the program. Implemented as a class due to easy reuse, and QT does not like
 *              messageboxes shown with text too long.
 */

#ifndef HELPMESSAGEDIALOG_H
#define HELPMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class HelpMessageDialog;
}

class HelpMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpMessageDialog(QWidget *parent = 0);
    ~HelpMessageDialog();

private:
    Ui::HelpMessageDialog *ui;
};

#endif // HELPMESSAGEDIALOG_H
