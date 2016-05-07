#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T15:51:52
#
#-------------------------------------------------

QT       += core gui serialport sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bio-Feedback
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    ajustespuertoserial.cpp \
    ajustessensores.cpp \
    raw.cpp \
    angulo.cpp \
    filtro_kalman.cpp \
    serial.cpp \
    ajustesgrafico.cpp \
    reportes.cpp \
    sql.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    ajustespuertoserial.h \
    ajustessensores.h \
    raw.h \
    angulo.h \
    filtro_kalman.h \
    serial.h \
    ajustesgrafico.h \
    reportes.h \
    sql.h

FORMS    += mainwindow.ui \
    ajustespuertoserial.ui \
    ajustessensores.ui \
    ajustesgrafico.ui \
    sql.ui


RESOURCES += \
    images.qrc

DISTFILES +=
