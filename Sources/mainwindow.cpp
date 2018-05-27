/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: MainWindow
 * Description: The class behind the starting window, sets up the basic window size, its header image, and the buttons
 *              you click to continue on with the program.
 */

#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "Headers/helpmessagedialog.h"


/* Function Name: MainWindow constructor
 * Description: This function is the constructor, and loads the UI, and the image I chose for the main menu.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
MainWindow::MainWindow(QWidget *parent) :  
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Bronc Scheduler Main Menu");

    QPixmap image;
    QImage  *imageObject;
    QGraphicsScene *scene;

    imageObject = new QImage();
    imageObject->load("../welcomeScreenImg.jpg");

    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}

MainWindow::~MainWindow()
{
    //qDebug() << "now deleting main window";
    //delete ui;
    deleteLater();
}


void MainWindow::on_exitButton_clicked()
{
    QCoreApplication::instance()->exit();
}

void MainWindow::on_helpButton_clicked()
{
    HelpMessageDialog helpWindow;
    helpWindow.exec();
}

/* Function Name: on_startButton_clicked
 * Description: This function will start the main program. We never come back to the main menu, so it is necessary to give all control
 *              to the SelectSemester class. We switch the central widget to SelectSemester so it does not look like a new window
 *              is popping up, which took a while to implement correctly.
 *
 *              The function connects the menu items to functions in the SelectSemester class, since we can never come back here
 *              for any logic or functions.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void MainWindow::on_startButton_clicked()
{
    qDebug() << this->size();
    SelectSemester *s = new SelectSemester(this);
    s->setAttribute(Qt::WA_DeleteOnClose);

    connect (ui->actionGo_Back_To_Select_Semesters, SIGNAL(triggered()), s, SLOT(goBackToSelectSemestersOption()));
    connect (ui->actionView_Help, SIGNAL(triggered()), s, SLOT(viewHelpDialog()));

    ui->actionGo_Back_To_Select_Semesters->setEnabled(true);
    ui->actionView_Help->setEnabled(true);
    this->setCentralWidget(s); //now this switches screens to my stacked widget containing other things
}
