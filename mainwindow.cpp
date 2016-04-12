#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial=new QSerialPort(this);

    settings= new SettingsDialog;
    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);

//    ui->qCustomPlotGraphic->xAxis->setVisible(false);
//    ui->qCustomPlotGraphic->yAxis->setVisible(false);
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
    ui->qCustomPlotGraphic->clearGraphs(); //Se limpian los graficos agregados
    ui->qCustomPlotGraphic->clearPlottables(); //Se eliminan los elementos graficables

    QCPItemEllipse *circleexterior;
    circleexterior= new QCPItemEllipse(ui->qCustomPlotGraphic);
    int rexterior=20;
    circleexterior->topLeft->setCoords(-rexterior,rexterior);
    circleexterior->bottomRight->setCoords(rexterior,-rexterior);
    circleexterior->setBrush(QBrush(Qt::yellow));

    QCPItemEllipse *circle;
    circle= new QCPItemEllipse(ui->qCustomPlotGraphic);
    int r=10;

    circle->topLeft->setCoords(-r,r);
    circle->bottomRight->setCoords(r,-r);

    lienzo = new QCPCurve(ui->qCustomPlotGraphic->xAxis,ui->qCustomPlotGraphic->yAxis);
    ui->qCustomPlotGraphic->addPlottable(lienzo);

    ui->qCustomPlotGraphic->addGraph(); // Punto rojo
    ui->qCustomPlotGraphic->graph(0)->setPen(QPen(Qt::red));
    ui->qCustomPlotGraphic->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->qCustomPlotGraphic->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->qCustomPlotGraphic->setInteractions(false);
    ui->qCustomPlotGraphic->xAxis->setRange(-40,40);
    ui->qCustomPlotGraphic->yAxis->setRange(-40,40);
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
            samplesList.append(data);

            if(samplesNumber %ui->spinBoxgraphupdate->value()==0){//Mod
                ui->lcdNumberTime->display( QString::number(timer.elapsed()/1000.0, 'f', 2) );
                emit emitdata(data);
                //const QString status="Tiempo: "+QString::number(timer.elapsed()/1000.0)+"   Muestras: "+QString::number(samplesNumber);
            }
            QTextStream(stdout)<<"Tiempo:"<< timer.elapsed()/1000.0 << " Muestras:"<< samplesList.size() << " X: "<<data->getAngleX()<<" Y: "<< data->getAngleY() <<endl;
        }
    }
    else{
        serial->close();
        ui->pushButtonRestartTest->show();
        ui->pushButtonResults->show();
        ui->pushButtonSaveImage->show();
        ui->qCustomPlotGraphic->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    }
}

void MainWindow::writeData()
{
    //serial->write(ui->serialDataLineEdit->text().toLocal8Bit());
}

void MainWindow::realtimeDataSlot(Data *data)
{
    lienzo->addData(data->getAngleY(), data->getAngleX());

    ui->qCustomPlotGraphic->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto rojo.
    ui->qCustomPlotGraphic->graph(0)->addData(data->getAngleY(), data->getAngleX());
    ui->qCustomPlotGraphic->graph(0)->rescaleValueAxis(true);

    ui->qCustomPlotGraphic->replot(); //Se redibuja el grafico
}

void MainWindow::openSerialPort()
{
    timer.start();
    samplesNumber=0;
    samplesList.clear();

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
         init_graph(); //Se limpian los graficos
        //serial->dataTerminalReadyChanged(true);
        //serial->requestToSendChanged(true);
        ui->pushButtonRestartTest->hide();
        ui->pushButtonResults->hide();
        ui->pushButtonSaveImage->hide();

        QMessageBox::information(this,"Puerto Abierto","El puerto se ha abierto");
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen()){
        serial->close();
        QMessageBox::information(this,"Cerrar Puerto","Puerto Cerrado");
        QTextStream(stdout)<<"Cerrado";
    }
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

    QString filters("Imagen (*.png);;All files (*.*)");
    QString defaultFilter("Text files (*.txt)");
    QString fileName =  QFileDialog::getSaveFileName(0, "Guardar Imagen", QDir::homePath()+"/pictures",filters);

    ui->qCustomPlotGraphic->grab().save(fileName);

}
