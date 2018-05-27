#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T09:40:54
#
#-------------------------------------------------

QT       += core gui\
            webenginewidgets \
            printsupport \
            network \
            sql \
            svg \
            xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BroncScheduler
TEMPLATE = app

SOURCES += Sources/addclasswidget.cpp \
    Sources/buildscheduleslist.cpp \
    Sources/classrestrictionsmainmenu.cpp \
    Sources/displaydetailedclassinfo.cpp \
    Sources/eliminaterestrictions.cpp \
    Sources/exportoptions.cpp \
    Sources/helpmessagedialog.cpp \
    Sources/showcurrentrestrictionsdialog.cpp \
    Sources/timerestrictionsdialog.cpp \
    Sources/viewcrndialog.cpp

HEADERS  += Headers/addclasswidget.h \
    Headers/buildscheduleslist.h \
    Headers/classrestrictionsmainmenu.h \
    Headers/displaydetailedclassinfo.h \
    Headers/eliminaterestrictions.h \
    Headers/exportoptions.h \
    Headers/helpmessagedialog.h \
    Headers/showcurrentrestrictionsdialog.h \
    Headers/timerestrictionsdialog.h \
    Headers/viewcrndialog.h

FORMS    += Forms/addclasswidget.ui \
    Forms/classrestrictionsmainmenu.ui \
    Forms/displaydetailedclassinfo.ui \
    Forms/exportoptions.ui \
    Forms/helpmessagedialog.ui \
    Forms/selectsemester.ui \
    Forms/showcurrentrestrictionsdialog.ui \
    Forms/timerestrictionsdialog.ui \
    Forms/viewcrndialog.ui \
    Forms/mainwindow.ui
