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
    status = new QLabel;
    ui->statusBar->addWidget(status);
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
    connect(ui->verticalSliderRangeGraphic,SIGNAL(valueChanged(int)),this,SLOT(yRangeGraphic(int)));
    connect(ui->horizontalSliderRangeGraphic,SIGNAL(valueChanged(int)),this,SLOT(xRangeGraphic(int)));
    connect(ui->qCustomPlotGraphic,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(ZoomGraphic(QWheelEvent*)));
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
    ui->qCustomPlotGraphic->xAxis->setRange(-ui->horizontalSliderRangeGraphic->value(),ui->horizontalSliderRangeGraphic->value());
    ui->qCustomPlotGraphic->yAxis->setRange(-ui->verticalSliderRangeGraphic->value(),ui->verticalSliderRangeGraphic->value());

}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

void MainWindow::readData(){
    if ( timer.elapsed()/1000.0 <= ui->spinBoxTime->value()){
        while (serial->canReadLine()){
            const QByteArray serialData = serial->readLine();
            serialReaded=QString(serialData);

            QStringList linea=serialReaded.split(" ");
            const double AngleY=QString(linea.at(0)).toDouble();
            const double AngleX=QString(linea.at(1)).toDouble();

            samplesNumber+=1;

//                showStatusMessage(status);
//                emit emitstatustographics(status);
            if(samplesNumber==1)//Cuando se agrega el primer dato, se inicia el tiempo.
                timer.start();

            Data *data=new Data(AngleX,AngleY,timer.elapsed()/1000.0);
            samplesList.append(data);

            if(samplesNumber %ui->spinBoxgraphupdate->value()==0){//Mod
                emit emitdata(data);
                //const QString status="Tiempo: "+QString::number(timer.elapsed()/1000.0)+"   Muestras: "+QString::number(samplesNumber);
            }
            ui->lcdNumberTime->display( QString::number(timer.elapsed()/1000.0, 'f', 2) );
            const QString message="Tiempo: "+QString::number(timer.elapsed()/1000.0) + " Muestras:" + QString::number(samplesList.size())+ " X: "+QString::number(data->getAngleX())+" Y: "+QString::number(data->getAngleY());
            showStatusMessage(message);
            //QTextStream(stdout)<<"Tiempo:"<< timer.elapsed()/1000.0 << " Muestras:"<< samplesList.size() << "  X: "<<data->getAngleX()<<" Y: "<< data->getAngleY() <<endl;
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
    lienzo->addData(data->getAngleX(), data->getAngleY());

    ui->qCustomPlotGraphic->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto rojo.
    ui->qCustomPlotGraphic->graph(0)->addData(data->getAngleX(), data->getAngleY());
    //ui->qCustomPlotGraphic->graph(0)->rescaleValueAxis(true);

    ui->qCustomPlotGraphic->replot(); //Se redibuja el grafico
}

void MainWindow::xRangeGraphic(int xRange)
{
    ui->qCustomPlotGraphic->xAxis->setRange(-xRange,xRange);
    ui->qCustomPlotGraphic->replot();
}

void MainWindow::yRangeGraphic(int yRange)
{
    ui->qCustomPlotGraphic->yAxis->setRange(-yRange,yRange);
    ui->qCustomPlotGraphic->replot();
}

void MainWindow::ZoomGraphic(QWheelEvent *event)
{
    QCPRange xRange=ui->qCustomPlotGraphic->xAxis->range();
    QCPRange yRange=ui->qCustomPlotGraphic->yAxis->range();
    ui->qCustomPlotGraphic->xAxis->setRange(xRange);
    ui->qCustomPlotGraphic->yAxis->setRange(yRange);
    ui->verticalSliderRangeGraphic->setValue(yRange.upper);
    ui->horizontalSliderRangeGraphic->setValue(xRange.upper);

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

    double q1=0, q2=0, q3=0, q4=0;

    foreach (Data *var, samplesList) {//Se recorren las muestras y compara para determinar en que cuadrante estan.
        if(var->getAngleX()>0){
            if(var->getAngleY()>0)
                q1+=1;
            else
                q3+=1;
        }
        else{
            if(var->getAngleY()>0)
                q2+=1;
            else
                q4+=1;
        }
    }

    q1=q1/samplesList.size()*100;
    q2=q2/samplesList.size()*100;
    q3=q3/samplesList.size()*100;
    q4=q4/samplesList.size()*100;
    qDebug()<<"1="<<q1<<"% 2="<<q2<<"% 3="<<q3<<"% 4="<<q4<<"%"<<endl;

    QCPBars *quadrants = new QCPBars(ui->qCustomPlotResults->xAxis, ui->qCustomPlotResults->yAxis);
    ui->qCustomPlotResults->addPlottable(quadrants);
    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    quadrants->setName("Porcentaje en cada cuadrante");
    pen.setColor(QColor(255, 131, 0));
    quadrants->setPen(pen);
    quadrants->setBrush(QColor(255, 131, 0, 50));



    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4;
    labels << "Cuadrante 1" << "Cuadrante 2" << "Cuadrante 3" << "Cuadrante 4";
    ui->qCustomPlotResults->xAxis->setAutoTicks(false);
    ui->qCustomPlotResults->xAxis->setAutoTickLabels(false);
    ui->qCustomPlotResults->xAxis->setTickVector(ticks);
    ui->qCustomPlotResults->xAxis->setTickVectorLabels(labels);
    ui->qCustomPlotResults->xAxis->setTickLabelRotation(60);
    ui->qCustomPlotResults->xAxis->setSubTickCount(0);
    ui->qCustomPlotResults->xAxis->setTickLength(0, 4);
    ui->qCustomPlotResults->xAxis->grid()->setVisible(true);
    ui->qCustomPlotResults->xAxis->setRange(0, 5);

    // prepare y axis:
    ui->qCustomPlotResults->yAxis->setRange(0, 100);
    ui->qCustomPlotResults->yAxis->setPadding(5); // a bit more space to the left border
    ui->qCustomPlotResults->yAxis->setLabel("Porcentaje");
    ui->qCustomPlotResults->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->qCustomPlotResults->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->qCustomPlotResults->yAxis->grid()->setSubGridPen(gridPen);

    // Add data:
    QVector<double> quadrantData;
    quadrantData  << q1 << q2 << q3 << q4;
    quadrants->setData(ticks, quadrantData);

    // setup legend:
    ui->qCustomPlotResults->legend->setVisible(true);
    ui->qCustomPlotResults->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->qCustomPlotResults->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->qCustomPlotResults->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->qCustomPlotResults->legend->setFont(legendFont);
    ui->qCustomPlotResults->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->qCustomPlotResults->replot();
}

void MainWindow::on_pushButtonTest1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigTest);
}

void MainWindow::on_pushButtonSaveImage_clicked()
{
    QString filters("Imagen (*.png);;All files (*.*)");
    QString fileName =  QFileDialog::getSaveFileName(0, "Guardar Imagen", QDir::homePath()+"/pictures",filters);
//    ui->qCustomPlotGraphic->xAxis->setRange(-30,30);
//    ui->qCustomPlotGraphic->yAxis->setRange(-30,30);
    ui->qCustomPlotGraphic->savePng(fileName,1000,1000);
}
