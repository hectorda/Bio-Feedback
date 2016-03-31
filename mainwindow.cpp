#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qserialport.h"
#include "qserialportinfo.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    inicializar();
    conexiones();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inicializar()
{
    serial=new QSerialPort(this);
    settings= new Settings;
}

void MainWindow::conexiones(){
    //connect(ui->exitButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->actionConfigurar_Sensor,SIGNAL(triggered()),settings,SLOT(show()));

}
