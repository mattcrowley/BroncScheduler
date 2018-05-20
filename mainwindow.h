/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: MainWindow
 * Description: The class behind the starting window, sets up the basic window size, its header image, and the buttons
 *              you click to continue on with the program.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "selectsemester.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_exitButton_clicked();

    void on_helpButton_clicked();

    void on_startButton_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
