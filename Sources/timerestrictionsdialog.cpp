/* Programmer's Name: Matt Crowley
 * Program Title: Bronc Scheduler
 * Class: TimeRestrictionsDialog
 * Description: This class shows the 2 tables of times you can restrict, as well as the fields to block off certain dates.
 *              It allows functionality to click table cell sections, and change the background color.
 */

#include "Headers/timerestrictionsdialog.h"


TimeRestrictionsDialog::TimeRestrictionsDialog(HandleAllClassesParsing *h, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeRestrictionsDialog)
{
    ui->setupUi(this);
    ui->allDaysTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //resize the table so it shows everything
    this->setWindowTitle("Choose Day and Time Restrictions");

    mainParentCopy = h;
    this->restrictionInfo = h->classConstraints; //save restriction info
    this->setParent(parent);

    //now need to check the previous restrictions, and display it on screen
    handlePrevConstraints(); //load constraints chosen previously

    //signals for section headers and buttons at the bottom of the screen
    connect(ui->MWandTRTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(on_sectionClicked(int)));
    connect(ui->allDaysTable->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(on_allDayTableSectionClicked(int)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accepted()));
}

TimeRestrictionsDialog::~TimeRestrictionsDialog()
{
    delete ui;
}


/* Function Name: handlePrevConstraints
 * Description: This function will update the screen so that any previous constraints saved will load fine,
 *              so the user knows what they have already chosen.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void TimeRestrictionsDialog::handlePrevConstraints(){
    QStringList tempSplitStr;
    int col;

    for (int i = 0; i < restrictionInfo.size(); i++){//restrictionInfo contains all needed info, so we need to get info, then populate correct tables
        tempSplitStr = restrictionInfo[i].split(" ");

        //now tempSplitStr[0] has MW or TR...etc search for horizontalHeader info
        if (tempSplitStr[0] == "MW" || tempSplitStr[0] == "TR"){
            if (tempSplitStr[0] == "TR"){
                col = 1; //was TR column
            }
            else{
                col = 0; //MW column
            }
            for (int r = 0; r < ui->MWandTRTable->rowCount(); r++){
                if (ui->MWandTRTable->verticalHeaderItem(r)->text().startsWith(tempSplitStr[1], Qt::CaseInsensitive)){
                    toggleColor(r, col, ui->MWandTRTable, false);
                    break;
                }
            }

        }
        else if (tempSplitStr[0].size() == 1){ //either M or T or W or R or F...
            //first determine which column was chosen
            col = determineColumn(ui->allDaysTable, tempSplitStr[0]);

            //now find the correct row and toggle the color to indicate time is blocked off:
            for (int r = 0; r < ui->allDaysTable->rowCount(); r++){
                if (ui->allDaysTable->verticalHeaderItem(r)->text().startsWith(tempSplitStr[1], Qt::CaseInsensitive)){
                    toggleColor(r, col, ui->allDaysTable, false);
                    break;
                }
            }
        }
        else if (tempSplitStr.size() == 1){ //whole days blocked
            if (tempSplitStr[0] == ui->MonBox->text()){
                ui->MonBox->setChecked(true);
            }
            if (tempSplitStr[0] == ui->TuesBox->text()){
                ui->TuesBox->setChecked(true);
            }
            if (tempSplitStr[0] == ui->WedBox->text()){
                ui->WedBox->setChecked(true);
            }
            if (tempSplitStr[0] == ui->ThursBox->text()){
                ui->ThursBox->setChecked(true);
            }
            if (tempSplitStr[0] == ui->FriBox->text()){
                ui->FriBox->setChecked(true);
            }
        }
    }
}



/* Function Name: determineColumn
 * Description: This function was created to find the column of a table passed in, whose data matches the passed in value.
 *
 *              It searches the headers of "Monday", "Tuesday", etc, until it finds the correct date, then returns that column.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: The table we want to search, and the string of the text we want to find.
*/
int TimeRestrictionsDialog::determineColumn(QTableWidget *table, QString textToFind){
    int col = -1;

    for (int c = 0; c < table->columnCount(); c++){
        if (table->horizontalHeaderItem(c)->text().startsWith(textToFind, Qt::CaseInsensitive)){
            col = c;
            break;
        }
    }
    return col;
}



/* Function Name: toggleColor
 * Description: The function itself changes background color of a selected cell.
 *
 *              This function is called in two different types of places. The first is at initialization, when the user
 *              has already chosen restrictions, and we load them here. This is when the passed in bool is false,
 *              as those values are already in the list of restrictions.
 *
 *              The second place is during normal run time, when the user chooses a time to block. This will then either
 *              add the restriction to the list, or remove it, depending on the current cell color.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: Takes in the row and column of the cell to change, the table we want it done in, and a boolean value that says if we should
 *         add this restriction or remove it from the list. Used when user clicks a cell.
*/
void TimeRestrictionsDialog::toggleColor(int row, int column, QTableWidget* table, bool shouldAddRemove){
    QTableWidgetItem *temp = table->item(row, column);

    if (temp->backgroundColor() == QColor("Invalid") || temp->backgroundColor() == QColor("white")){
        temp->setBackgroundColor(QColor("red"));

        if (shouldAddRemove == true){
            QString classBlocked = table->horizontalHeaderItem(column)->text() + " " + table->verticalHeaderItem(row)->text();
            restrictionInfo.append(classBlocked);
        }
    }
    else if (temp->backgroundColor() == QColor("red")){
        temp->setBackgroundColor(QColor("white"));

        if (shouldAddRemove == true){
            QString s = table->horizontalHeaderItem(column)->text() + " " + table->verticalHeaderItem(row)->text();
            restrictionInfo.removeOne(s);
        }
    }
}


void TimeRestrictionsDialog::on_MWandTRTable_cellClicked(int row, int column)
{
    toggleColor(row, column, ui->MWandTRTable);
}



/* Function Name: on_sectionClicked
 * Description: This function is called when the user clicks the section header, changing all values to opposite background colors.
 *              Only for the MWandTR table. The MTWRF table function is below.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: The row selected (int).
*/
void TimeRestrictionsDialog::on_sectionClicked(int logicalIndex){
    qDebug() << "section clicked!";
    for (int i = 0; i < ui->MWandTRTable->columnCount(); i++)
        toggleColor(logicalIndex, i, ui->MWandTRTable);
}

//this is for the MTWRF day table
void TimeRestrictionsDialog::on_allDayTableSectionClicked(int logicalIndex){
    for (int i = 0; i < ui->allDaysTable->columnCount(); i++)
        toggleColor(logicalIndex, i, ui->allDaysTable);
}


void TimeRestrictionsDialog::on_allDaysTable_cellClicked(int row, int column)
{
    toggleColor(row, column, ui->allDaysTable);
}


/* Function Name: accepted
 * Description: This function is called to save course restrictions, as the user made changes, and decided to save them.
 * Preconditions: NONE
 * Postconditions: NONE
 * Inputs: NONE
*/
void TimeRestrictionsDialog::accepted(){
    //handle whole days being blocked:
    handleDaysBlocked();

    mainParentCopy->classConstraints = restrictionInfo; //error: duplicates class constraints, since it does not delete upon save

    //qDebug() << "Before sorting:";
    //printRestrictionInfo();

    //qDebug() << "After sorting:";
    std::sort(restrictionInfo.begin(), restrictionInfo.end()); //need to sort based on string length, may simplify matters
    //printRestrictionInfo();
}



/* Function Name: handleDaysBlocked
 * Description: This function will determine which of the whole days is blocked off, and then adds them to the restriction
 *              info. Currently not very pretty, but the implementation I have needs this to work.
 * Preconditions: Only called when the user decides to save their changes.
 * Postconditions: NONE
 * Inputs: NONE
*/
void TimeRestrictionsDialog::handleDaysBlocked(){
    if (ui->MonBox->isChecked()){
        if (!restrictionInfo.contains(ui->MonBox->text()))
            restrictionInfo.append(ui->MonBox->text());
    }
    if (ui->TuesBox->isChecked()){
        if (!restrictionInfo.contains(ui->TuesBox->text()))
            restrictionInfo.append(ui->TuesBox->text());
    }
    if (ui->WedBox->isChecked()){
        if (!restrictionInfo.contains(ui->WedBox->text()))
            restrictionInfo.append(ui->WedBox->text());
    }
    if (ui->ThursBox->isChecked()){
        if (!restrictionInfo.contains(ui->ThursBox->text()))
            restrictionInfo.append(ui->ThursBox->text());
    }
    if (ui->FriBox->isChecked()){
        if (!restrictionInfo.contains(ui->FriBox->text()))
            restrictionInfo.append(ui->FriBox->text());
    }
    if (ui->MonBox->isChecked() == false){
        restrictionInfo.removeAll(ui->MonBox->text());
    }
    if (ui->TuesBox->isChecked() == false){
        restrictionInfo.removeAll(ui->TuesBox->text());
    }
    if (ui->WedBox->isChecked() == false){
        restrictionInfo.removeAll(ui->WedBox->text());
    }
    if (ui->ThursBox->isChecked() == false){
        restrictionInfo.removeAll(ui->ThursBox->text());
    }
    if (ui->FriBox->isChecked() == false){
        restrictionInfo.removeAll(ui->FriBox->text());
    }
}

//for debugging purposes
void TimeRestrictionsDialog::printRestrictionInfo(){
    foreach (QString s, restrictionInfo){
        qDebug() << s;
    }
}
