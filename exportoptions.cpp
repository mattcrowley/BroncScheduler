/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: ExportOptions
 * Description: This class shows the window when the user selects to export when showing the user's schedule.
 *              Implements the functionality for the view CRN's button, and creation of the PDF.
 */

#include "exportoptions.h"
#include "ui_exportoptions.h"
#include "viewcrndialog.h" //to show the CRN's of the current schedule
#include <qprinter>
#include <qfiledialog.h>

ExportOptions::ExportOptions(QTableWidget *outputTable, QList<int> courseRegNum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportOptions)
{
    ui->setupUi(this);
    this->courseRegNumbers = courseRegNum;
    tableCopy = outputTable;
    this->setParent(parent);
}

ExportOptions::~ExportOptions()
{
    qDebug() << "deleting export options";
    delete ui;
}


/* Function Name: on_viewCRNsButton_clicked
 * Description: Shows the popup UI of the selected course registration numbers.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void ExportOptions::on_viewCRNsButton_clicked()
{
   //now take the registration numbers, pass it into the dialog to popup, and display them all:
    ViewCRNDialog *popupDialogCRN = new ViewCRNDialog(courseRegNumbers, this);
    popupDialogCRN->setAttribute(Qt::WA_DeleteOnClose);

    popupDialogCRN->exec();
}


/* Function Name: on_createCondensedPDFTableButton_clicked
 * Description: This function will output the current schedule onto a PDF file, allowing the user to choose where they
 *              want to save their copy of classes. This easily allows them to print their schedule, without
 *              relying on ASSIST.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void ExportOptions::on_createCondensedPDFTableButton_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOrientation(QPrinter::Landscape);

    QFileDialog savePDF;
    QString absPath = savePDF.getSaveFileName(this, "Choose a path", "../", ("PDF (*.pdf)") );

    if (absPath != ""){ //then we can save the file
        printer.setOutputFileName(absPath);

        //now some sort of "please wait" message:
        QProgressDialog *msg = new QProgressDialog("Saving to File...", 0, 0, 1, this);
        msg->show();

        QPainter painter;
        painter.begin(&printer); //I believe this is the cause of the above messagebox not showing, need a better work around

   /*     double xscale = printer.pageRect().width()/double(tableCopy->width()); //old code, very very slow, large file too
        double yscale = printer.pageRect().height()/double(tableCopy->height());
        double scale = qMin(xscale, yscale);
        painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
                           printer.paperRect().y() + printer.pageRect().height()/2);
        painter.scale(scale, scale);
        painter.translate(-width()/1.5, -height()/2);

        tableCopy->render(&painter);
        */
        const int rows = tableCopy->model()->rowCount();
        const int columns = tableCopy->model()->columnCount();
        double totalWidth = tableCopy->columnWidth(0);
        for (int c = 0; c < columns; c++){
            qDebug() << "adding to width: " << tableCopy->columnWidth(c);
          totalWidth += tableCopy->columnWidth(c);
        }
        qDebug() << tableCopy->verticalHeader()->width() << " //" << totalWidth;
        double totalHeight = tableCopy->horizontalHeader()->height();
        for (int r = 0; r < rows; r++)
          totalHeight += tableCopy->rowHeight(r);
qDebug() << "total height" << totalHeight;
        QTableView tempTable;
        tempTable.setAttribute(Qt::WA_DontShowOnScreen);
        tempTable.setModel(tableCopy->model());
        tempTable.resizeRowsToContents();
        //tempTable.resizeColumnsToContents();
       // tempTable.verticalHeader()->setDefaultSectionSize(totalHeight/(rows*1.0));
        //tempTable.horizontalHeader()->setDefaultSectionSize(totalWidth/(columns*1.0));
        tempTable.setFixedSize(totalWidth, totalHeight);


        //tempTable.horizontalHeader()->setStretchLastSection(true);
        //tempTable.verticalHeader()->setStretchLastSection(true);


        tempTable.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tempTable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QPixmap pix = tempTable.grab(); //like taking a screenshot of the one table
        //QPixmap pix = tableCopy->grab(); //like taking a screenshot of the one table
        int h = painter.window().height()*0.7;
        int w = h *1.2;
        int x = (painter.window().width() / 2) - (w/2);
        int y = (painter.window().height() / 2) - (h/2);
        //tableCopy->resize(w, h);


        painter.drawPixmap(x, y, w, h, pix);
        painter.end();

        msg->close();
    }
}
