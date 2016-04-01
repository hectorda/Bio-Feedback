#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qserialport.h"
#include "settingsdialog.h"
#include "qmessagebox.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial=new QSerialPort(this);

    settings= new SettingsDialog;
    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);
    init_Connections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_Connections(){
    connect(ui->pushButtonExit,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->actionConfigurar_Serial,SIGNAL(triggered()),settings,SLOT(show()));
    connect(ui->pushButtonStartTest,SIGNAL(clicked()),this,SLOT(openSerialPort()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

void MainWindow::readData(){

    if ( timer.elapsed()/1000.0 <= (double)ui->spinBoxTime->value()){
        while (serial->canReadLine()){
            const QByteArray serialData = serial->readLine();
            serialReaded=QString(serialData);

            QStringList linea=serialReaded.split(" ");
                samplesNumber+=1;
                const QString status="Tiempo: "+QString::number(timer.elapsed()/1000.0)+"   Muestras: "+QString::number(samplesNumber);
                QTextStream(stdout)<<serialReaded<<status<<endl;
//                showStatusMessage(status);
//                emit emitstatustographics(status);
                if(samplesNumber==1)//Cuando se agrega el primer dato, se inicia el tiempo.
                    timer.start();

//                listaTiempos.append(timer.elapsed()/1000.0);
//                datos.append(linea);
//                emit emitlinea(linea);
//                if(samplesNumber % ui->frecgraph->value()==0)//Cada x datos se grafica
//                    emit emitdato(linea,timer.elapsed()/1000.0);



        }
    }
    else{
        serial->close();
        QTextStream(stdout)<<"Cerrado";
//        ui->connectButton->setDisabled(false);
//        ui->stopButton->setDisabled(true);
//        serial->close();
    }
}

void MainWindow::writeData()
{
    //serial->write(ui->serialDataLineEdit->text().toLocal8Bit());
}

void MainWindow::openSerialPort()
{
    timer.start();
    samplesNumber=0;
//    datos.clear();        //Limpieza de las listas
//    listaTiempos.clear();
    SettingsDialog::Settings cs=settings->getCurrentSettings();
    serial->setPortName(cs.portName);
    serial->setBaudRate(cs.baudRate);
    QTextStream(stdout)<<"Baudios: "<< serial->baudRate();
    QTextStream(stdout)<<"portName"<< serial->portName();
    serial->setStopBits(QSerialPort::OneStop);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)){
        //serial->dataTerminalReadyChanged(true);
        //serial->requestToSendChanged(true);
//        ui->connectButton->setDisabled(true);
//        ui->stopButton->setDisabled(false);
//        emit vamosagraficar(this->GetGraphicsCheckboxs());
        QMessageBox::information(this,"Puerto Abierto","El puerto se ha abierto");
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
}

void MainWindow::closeSerialPort()
{
    //datos.clear();
    samplesNumber=0;
    //listaTiempos.clear();
    if (serial->isOpen()){
        serial->close();
        QMessageBox::information(this,"Cerrar Puerto","Puerto Cerrado");
        QTextStream(stdout)<<"Cerrado";
        //ui->connectButton->setDisabled(false);
    }
    else {
         QMessageBox::information(this,"Cerrar Puerto","El puerto ya estaba cerrado");
    }
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
