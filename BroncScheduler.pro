#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T09:40:54
#
#-------------------------------------------------

QT       += core gui\
            webkit \
            webkitwidgets \
            printsupport \
            network \
            sql \
            svg \
            xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BroncScheduler
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    selectsemester.cpp \
    coursesofferedlist.cpp \
    courseinfo.cpp \
    handleallclassesparsing.cpp \
    addclasswidget.cpp \
    classrestrictionsmainmenu.cpp \
    timerestrictionsdialog.cpp \
    eliminaterestrictions.cpp \
    buildscheduleslist.cpp \
    exportoptions.cpp \
    viewcrndialog.cpp \
    displaydetailedclassinfo.cpp \
    showcurrentrestrictionsdialog.cpp \
    helpmessagedialog.cpp \
    Sources/addclasswidget.cpp \
    Sources/buildscheduleslist.cpp \
    Sources/classrestrictionsmainmenu.cpp \
    Sources/courseinfo.cpp \
    Sources/coursesofferedlist.cpp \
    Sources/displaydetailedclassinfo.cpp \
    Sources/eliminaterestrictions.cpp \
    Sources/exportoptions.cpp \
    Sources/handleallclassesparsing.cpp \
    Sources/helpmessagedialog.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp \
    Sources/selectsemester.cpp \
    Sources/showcurrentrestrictionsdialog.cpp \
    Sources/timerestrictionsdialog.cpp \
    Sources/viewcrndialog.cpp \
    Sources/addclasswidget.cpp \
    Sources/buildscheduleslist.cpp \
    Sources/classrestrictionsmainmenu.cpp \
    Sources/courseinfo.cpp \
    Sources/coursesofferedlist.cpp \
    Sources/displaydetailedclassinfo.cpp \
    Sources/eliminaterestrictions.cpp \
    Sources/exportoptions.cpp \
    Sources/handleallclassesparsing.cpp \
    Sources/helpmessagedialog.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp \
    Sources/selectsemester.cpp \
    Sources/showcurrentrestrictionsdialog.cpp \
    Sources/timerestrictionsdialog.cpp \
    Sources/viewcrndialog.cpp

HEADERS  += mainwindow.h \
    handleallclassesparsing.h \
    selectsemester.h \
    coursesofferedlist.h \
    courseinfo.h \
    addclasswidget.h \
    classrestrictionsmainmenu.h \
    timerestrictionsdialog.h \
    eliminaterestrictions.h \
    buildscheduleslist.h \
    exportoptions.h \
    viewcrndialog.h \
    displaydetailedclassinfo.h \
    showcurrentrestrictionsdialog.h \
    helpmessagedialog.h \
    Headers/addclasswidget.h \
    Headers/buildscheduleslist.h \
    Headers/classrestrictionsmainmenu.h \
    Headers/courseinfo.h \
    Headers/coursesofferedlist.h \
    Headers/displaydetailedclassinfo.h \
    Headers/eliminaterestrictions.h \
    Headers/exportoptions.h \
    Headers/handleallclassesparsing.h \
    Headers/helpmessagedialog.h \
    Headers/mainwindow.h \
    Headers/selectsemester.h \
    Headers/showcurrentrestrictionsdialog.h \
    Headers/timerestrictionsdialog.h \
    Headers/viewcrndialog.h

FORMS    += mainwindow.ui \
    addclasswidget.ui \
    classrestrictionsmainmenu.ui \
    timerestrictionsdialog.ui \
    exportoptions.ui \
    viewcrndialog.ui \
    selectsemester.ui \
    displaydetailedclassinfo.ui \
    showcurrentrestrictionsdialog.ui \
    helpmessagedialog.ui \
    Forms/addclasswidget.ui \
    Forms/classrestrictionsmainmenu.ui \
    Forms/creatingClassListDialog.ui \
    Forms/displaydetailedclassinfo.ui \
    Forms/exportoptions.ui \
    Forms/helpmessagedialog.ui \
    Forms/mainwindow.ui \
    Forms/selectsemester.ui \
    Forms/showcurrentrestrictionsdialog.ui \
    Forms/timerestrictionsdialog.ui \
    Forms/viewcrndialog.ui
