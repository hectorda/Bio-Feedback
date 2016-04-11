#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qserialport.h"
#include "settingsdialog.h"
#include "qmessagebox.h"
#include "data.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial=new QSerialPort(this);

    settings= new SettingsDialog;
    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);
    qccuve = new QCPCurve(ui->qCustomPlotGraphic->xAxis,ui->qCustomPlotGraphic->yAxis);
    ui->qCustomPlotGraphic->addPlottable(qccuve);
    ui->qCustomPlotGraphic->plotLayout()->insertRow(0);
    ui->qCustomPlotGraphic->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->qCustomPlotGraphic, "Grafico Angulos X e Y"));
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
    connect(ui->pushButtonRestartTest,SIGNAL(clicked()),this,SLOT(openSerialPort()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(this,SIGNAL(emitdata(Data*)),this,SLOT(realtimeDataSlot(Data*)));
}

void MainWindow::init_graph()
{
//    QMessageBox::information(this,"Graficos",QString::number(ui->qCustomPlotGraphic->graphCount()));
    ui->qCustomPlotGraphic->clearGraphs();
    qccuve->clearData();
//    ui->qCustomPlotGraphic->removeGraph(2);
//    ui->qCustomPlotGraphic->addGraph(); // blue line
//    ui->qCustomPlotGraphic->graph(0)->setPen(QPen(Qt::blue));
//    ui->qCustomPlotGraphic->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    QCPItemEllipse *circle;
    circle= new QCPItemEllipse(ui->qCustomPlotGraphic);
    int r=10;
    circle->topLeft->setCoords(-r,r);
    circle->bottomRight->setCoords(r,-r);

    QCPItemEllipse *circleexterior;
    circleexterior= new QCPItemEllipse(ui->qCustomPlotGraphic);
    int rexterior=20;
    circleexterior->topLeft->setCoords(-rexterior,rexterior);
    circleexterior->bottomRight->setCoords(rexterior,-rexterior);




    ui->qCustomPlotGraphic->addGraph(); // red dot
    ui->qCustomPlotGraphic->graph(0)->setPen(QPen(Qt::red));
    ui->qCustomPlotGraphic->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->qCustomPlotGraphic->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);



//    // make left and bottom axes transfer their ranges to right and top axes:
//    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
//    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

//    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
//    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
//    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
//    //doPlot();
}


void MainWindow::readData(){
    if ( timer.elapsed()/1000.0 <= ui->spinBoxTime->value()){
        while (serial->canReadLine()){
            const QByteArray serialData = serial->readLine();
            serialReaded=QString(serialData);

            QStringList linea=serialReaded.split(" ");
            const double AngleX=QString(linea.at(0)).toDouble();
            const double AngleY=QString(linea.at(1)).toDouble();

            samplesNumber+=1;

//                showStatusMessage(status);
//                emit emitstatustographics(status);
            if(samplesNumber==1)//Cuando se agrega el primer dato, se inicia el tiempo.
                timer.start();

            Data *data=new Data(AngleX,AngleY,timer.elapsed()/1000.0);
            SampleList.append(data);

                //emit senddata();
            if(samplesNumber %ui->spinBoxgraphupdate->value()==0){//Mod
                ui->lcdNumberTime->display( QString::number(timer.elapsed()/1000.0, 'f', 2) );
                emit emitdata(data);
                //const QString status="Tiempo: "+QString::number(timer.elapsed()/1000.0)+"   Muestras: "+QString::number(samplesNumber);
            }
            QTextStream(stdout)<<"Tiempo:"<< timer.elapsed()/1000.0 << " Muestras:"<< SampleList.size() << " X: "<<data->getAngleX()<<" Y: "<< data->getAngleY() <<endl;

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
        ui->pushButtonRestartTest->show();
        ui->pushButtonResults->show();
        ui->pushButtonSaveImage->show();
//        ui->connectButton->setDisabled(false);
//        ui->stopButton->setDisabled(true);
//        serial->close();
    }
}

void MainWindow::writeData()
{
    //serial->write(ui->serialDataLineEdit->text().toLocal8Bit());
}

void MainWindow::realtimeDataSlot(Data *data)
{
    qccuve->addData(data->getAngleX(), data->getAngleY());

    //ui->graficoAcX->graph(1)->addData(tiempo, AcX);
    // set data of dots:
    //ui->graficoAcX->graph(2)->clearData();
    //ui->graficoAcX->graph(2)->addData(tiempo, AcX+1);
    //ui->graficoAcX->graph(3)->clearData();
    //ui->graficoAcX->graph(3)->addData(tiempo, AcX+2);
    // remove data of lines that's outside visible range:
     ui->qCustomPlotGraphic->graph(0)->clearData();
     ui->qCustomPlotGraphic->graph(0)->addData(data->getAngleX(), data->getAngleY());

    // ui->qCustomPlotGraphic->graph(0)->removeDataBefore(data.time-8);
    //ui->graficoAcX->graph(1)->removeDataBefore(tiempo-8);
    // rescale value (vertical) axis to fit the current data:
    //rafico->graph(0)->rescaleValueAxis();
    ui->qCustomPlotGraphic->graph(0)->rescaleValueAxis(true);



    //Update the display range of your graph

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->qCustomPlotGraphic->xAxis->setRange(-40,40);
    ui->qCustomPlotGraphic->yAxis->setRange(-40,40);
    //ui->qCustomPlotGraphic->xAxis->setRange(data.time, 8, Qt::AlignRight);
    ui->qCustomPlotGraphic->replot();
}

void MainWindow::openSerialPort()
{
    timer.start();
    samplesNumber=0;
    SampleList.clear();
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
        serial->clear();
         init_graph();
        //serial->dataTerminalReadyChanged(true);
        //serial->requestToSendChanged(true);
        ui->pushButtonRestartTest->hide();
        ui->pushButtonResults->hide();
        ui->pushButtonSaveImage->hide();
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
    }/*
    else {
         QMessageBox::information(this,"Cerrar Puerto","El puerto ya estaba cerrado");
    }*/
}


void MainWindow::on_pushButtonStartTest_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetTest);
}

void MainWindow::on_pushButtonHome_clicked()
{

    QMessageBox messageBox(QMessageBox::Question,
                tr("¿Volver a Pagina Principal?"),
                tr("¿Seguro que desea volver a Inicio\nTodos los resultados no guardados se perderán?"),
                QMessageBox::Yes | QMessageBox::No,
                this);
    messageBox.setButtonText(QMessageBox::Yes, tr("Si"));
    messageBox.setButtonText(QMessageBox::No, tr("No"));

    if (messageBox.exec() == QMessageBox::Yes) {
          ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);
          closeSerialPort();
    } else {
          qDebug() << "Yes was *not* clicked";
    }
}

void MainWindow::on_pushButtonResults_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetResults);
    const QString name=ui->lineEditName->text();
    if(name=="")
        ui->labelResultsName->setText(tr("Paciente: %1").arg("Sin Nombre"));
    else
        ui->labelResultsName->setText(tr("Paciente: %1").arg(name));
}

void MainWindow::on_pushButtonTest1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigTest);
}

void MainWindow::on_pushButtonSaveImage_clicked()
{
    qDebug()<<"cicked";
    ui->qCustomPlotGraphic->grab().save("image.png");

}
