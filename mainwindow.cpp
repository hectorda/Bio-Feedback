#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qserialport.h"
#include "qserialportinfo.h"
#include "settingsdialog.h"

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
    settings= new SettingsDialog;
    ui->stackedWidget->setCurrentWidget(ui->widgetHome);
}

void MainWindow::conexiones(){
    //connect(ui->exitButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->actionConfigurar_Serial,SIGNAL(triggered()),settings,SLOT(show()));

}

void MainWindow::on_pushButtonStartTest_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetTest);
}

void MainWindow::on_pushButtonHome_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetHome);
}

void MainWindow::on_pushButtonResults_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetResults);
}
