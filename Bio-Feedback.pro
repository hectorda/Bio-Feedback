#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T15:51:52
#
#-------------------------------------------------

QT       += core gui serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bio-Feedback
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    dato.cpp \
    ajustespuertoserial.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    dato.h \
    ajustespuertoserial.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \


RESOURCES += \
    images.qrc

DISTFILES +=
