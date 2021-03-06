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
    angulo.cpp \
    serial.cpp \
    ajustesgrafico.cpp \
    reportes.cpp \
    sql.cpp \
    qxtspanslider.cpp \
    analisisgrafico.cpp \
    desplazamiento.cpp \
    prueba.cpp \
    Muestra.cpp \
    Kalman.cpp \
    ajustescalculoangulo.cpp \
    acerca.cpp \
    paciente.cpp \
    dialogcarga.cpp \
    objetivo.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    ajustespuertoserial.h \
    ajustessensores.h \
    angulo.h \
    serial.h \
    ajustesgrafico.h \
    reportes.h \
    sql.h \
    qxtspanslider.h \
    qxtspanslider_p.h \
    analisisgrafico.h \
    desplazamiento.h \
    prueba.h \
    Muestra.h \
    Kalman.h \
    ajustescalculoangulo.h \
    acerca.h \
    paciente.h \
    dialogcarga.h \
    objetivo.h

FORMS    += mainwindow.ui \
    ajustespuertoserial.ui \
    ajustessensores.ui \
    ajustesgrafico.ui \
    sql.ui \
    analisisgrafico.ui \
    ajustescalculoangulo.ui \
    acerca.ui \
    dialogcarga.ui


RESOURCES += \
    images.qrc \
    reporte.qrc

QMAKE_CXXFLAGS += -std=gnu++11

DISTFILES +=
