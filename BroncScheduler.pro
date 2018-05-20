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
    helpmessagedialog.cpp

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
    helpmessagedialog.h

FORMS    += mainwindow.ui \
    addclasswidget.ui \
    classrestrictionsmainmenu.ui \
    timerestrictionsdialog.ui \
    exportoptions.ui \
    viewcrndialog.ui \
    selectsemester.ui \
    displaydetailedclassinfo.ui \
    showcurrentrestrictionsdialog.ui \
    helpmessagedialog.ui
