#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    inicializar();
    conexiones();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::inicializar(){
    ui->setupUi(this);
    serial=new QSerialPort(this);
    //ui->menuVer->addAction(ui->dockWidget->toggleViewAction());

    ajustesSerial= new AjustesPuertoSerial;
    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);

    //ui->qCustomPlotGrafico->yAxis->setVisible(false);
    //ui->qCustomPlotGrafico->xAxis->setVisible(false);
    ui->qCustomPlotGrafico->plotLayout()->insertRow(0);
    ui->qCustomPlotGrafico->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->qCustomPlotGrafico, "Grafico Angulos X e Y"));
    status = new QLabel;
    ui->statusBar->addWidget(status);
    ui->dockWidget->installEventFilter(this);
}

void MainWindow::conexiones(){
    connect(ui->pushButtonSalir,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->actionConfigurar_Serial,SIGNAL(triggered()),ajustesSerial,SLOT(show()));
    connect(ui->pushButtonIniciarPrueba,SIGNAL(clicked()),this,SLOT(abrirPuertoSerial()));
    connect(ui->pushButtonReiniciarPrueba,SIGNAL(clicked()),this,SLOT(abrirPuertoSerial()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(leerDatosSerial()));
    connect(this,SIGNAL(emitdata(Dato*)),this,SLOT(slotDatosTiempoReal(Dato*)));
    connect(ui->verticalSliderRangeGraphic,SIGNAL(valueChanged(int)),this,SLOT(RangeGraphic(int)));
    connect(ui->qCustomPlotGrafico,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(ZoomGraphic(QWheelEvent*)));
    connect(ui->qCustomPlotGrafico,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(menuContextualGrafico(QMouseEvent*)));

}

void MainWindow::inicializarGrafico()
{
    ui->qCustomPlotGrafico->clearGraphs(); //Se limpian los graficos agregados
    ui->qCustomPlotGrafico->clearPlottables(); //Se eliminan los elementos graficables

    QCPItemEllipse *circleexterior;
    circleexterior= new QCPItemEllipse(ui->qCustomPlotGrafico);
    int rexterior=20;
    circleexterior->topLeft->setCoords(-rexterior,rexterior);
    circleexterior->bottomRight->setCoords(rexterior,-rexterior);
    circleexterior->setBrush(QBrush(Qt::yellow));

    QCPItemEllipse *circle;
    circle= new QCPItemEllipse(ui->qCustomPlotGrafico);
    int r=10;

    circle->topLeft->setCoords(-r,r);
    circle->bottomRight->setCoords(r,-r);

    lienzo = new QCPCurve(ui->qCustomPlotGrafico->xAxis,ui->qCustomPlotGrafico->yAxis);
    ui->qCustomPlotGrafico->addPlottable(lienzo);

    ui->qCustomPlotGrafico->addGraph(); // Punto rojo
    ui->qCustomPlotGrafico->graph(0)->setPen(QPen(Qt::red));
    ui->qCustomPlotGrafico->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->qCustomPlotGrafico->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->qCustomPlotGrafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    const int range=ui->verticalSliderRangeGraphic->value();
    ui->qCustomPlotGrafico->xAxis->setRange(-range,range);
    ui->qCustomPlotGrafico->yAxis->setRange(-range,range);

}

void MainWindow::mostrarMensajeBarraEstado(const QString &message)
{
    status->setText(message);
}

void MainWindow::leerDatosSerial(){
    if ( temporizador.elapsed()/1000.0 <= ui->spinBoxTiempoPrueba->value()){
        while (serial->canReadLine()){
            const QByteArray serialData = serial->readLine();
            datosLeidosPuertoSerial=QString(serialData);

            QStringList linea=datosLeidosPuertoSerial.split(" ");
            const double AnguloY=QString(linea.at(0)).toDouble();
            const double AnguloX=QString(linea.at(1)).toDouble();

            cantidadMuestras+=1;

//                showStatusMessage(status);
//                emit emitstatustographics(status);
            if(cantidadMuestras==1)//Cuando se agrega el primer dato, se inicia el tiempo.
                temporizador.start();

            Dato *dato=new Dato(AnguloX,AnguloY,temporizador.elapsed()/1000.0);
            listaMuestras.append(dato);

            if(cantidadMuestras %ui->spinBoxgraphupdate->value()==0){//Mod
                emit emitdata(dato);
                //const QString status="Tiempo: "+QString::number(timer.elapsed()/1000.0)+"   Muestras: "+QString::number(samplesNumber);
            }
            const QString lapso=QString::number(temporizador.elapsed()/1000.0, 'f', 2);
            ui->lcdNumberTiempoTranscurrido->display(lapso);
            const QString mensaje="Tiempo: "+ lapso + " Muestras:" + QString::number(listaMuestras.size())+ " X: "+QString::number(dato->getAnguloX(),'f',3)+" Y: "+QString::number(dato->getAnguloY(),'f',3);
            mostrarMensajeBarraEstado(mensaje);
            //QTextStream(stdout)<<"Tiempo:"<< timer.elapsed()/1000.0 << " Muestras:"<< samplesList.size() << "  X: "<<data->getAngleX()<<" Y: "<< data->getAngleY() <<endl;
        }
    }
    else{
        serial->close();
        ui->pushButtonReiniciarPrueba->show();
        ui->pushButtonResultados->show();
        ui->pushButtonGuardarImagen->show();
        ui->pushButtonGuardarMuestras->show();
        //ui->qCustomPlotGrafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    }
}

void MainWindow::escribirDatosSerial()
{
    //serial->write(ui->serialDataLineEdit->text().toLocal8Bit());
}

void MainWindow::slotDatosTiempoReal(Dato *data)
{
    lienzo->addData(data->getAnguloX(), data->getAnguloY());

    ui->qCustomPlotGrafico->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto rojo.
    ui->qCustomPlotGrafico->graph(0)->addData(data->getAnguloX(), data->getAnguloY());
    //ui->qCustomPlotGrafico->graph(0)->rescaleValueAxis(true);

    ui->qCustomPlotGrafico->replot(); //Se redibuja el grafico
}

void MainWindow::RangeGraphic(int Range)
{
    ui->qCustomPlotGrafico->xAxis->setRange(-Range,Range);
    ui->qCustomPlotGrafico->yAxis->setRange(-Range,Range);
    ui->qCustomPlotGrafico->replot();
}

void MainWindow::ZoomGraphic(QWheelEvent *event)
{
    QCPRange range=ui->qCustomPlotGrafico->xAxis->range();

    ui->qCustomPlotGrafico->xAxis->setRange(range);
    ui->qCustomPlotGrafico->yAxis->setRange(range);

    ui->verticalSliderRangeGraphic->setValue(range.upper);
//    ui->horizontalSliderRangeGraphic->setValue(xRange.upper);

}

void MainWindow::menuContextualGrafico(QMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton){
        qDebug() << "Click Derecho"<<event->screenPos().x()<<event->screenPos().y();
//        QPoint *pos=new QPoint(event->screenPos().x(),event->screenPos().y());
//        QPoint globalPos = ui->qCustomPlotGrafico->mapToGlobal(pos);
//            // for QAbstractScrollArea and derived classes you would use:
//            // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

//            QMenu myMenu;
//            myMenu.addAction("Menu Item 1");
//            // ...

//            QAction* selectedItem = myMenu.exec(globalPos);
//            if (selectedItem)
//            {
//                // something was chosen, do stuff
//            }
//            else
//            {
//                // nothing was chosen
//            }
////        QMenu contextMenu(tr("Context menu"), this);
////
////        contextMenu.exec(mapToGlobal(pos));
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Resize && obj == ui->dockWidget)
        relacionAspectodelGrafico();
    return QWidget::eventFilter(obj, event);
}

void MainWindow::relacionAspectodelGrafico()
{
    int w=ui->qCustomPlotGrafico->width();
    int h=ui->qCustomPlotGrafico->height();
    QRect rect=ui->qCustomPlotGrafico->geometry();

    if(w>h)
        ui->qCustomPlotGrafico->setGeometry(rect.x()+((w-h)/2),rect.y(),h,h);
    else
        ui->qCustomPlotGrafico->setGeometry(rect.x(),rect.y(),w,w);
}

void MainWindow::abrirPuertoSerial()
{
    temporizador.start();
    cantidadMuestras=0;
    listaMuestras.clear();

    AjustesPuertoSerial::Ajustes cs=ajustesSerial->getAjustes();
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
         inicializarGrafico(); //Se limpian los graficos
        //serial->dataTerminalReadyChanged(true);
        //serial->requestToSendChanged(true);
        ui->pushButtonReiniciarPrueba->hide();
        ui->pushButtonResultados->hide();
        ui->pushButtonGuardarImagen->hide();
        ui->pushButtonGuardarMuestras->hide();

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

void MainWindow::on_pushButtonIniciarPrueba_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetTest);
}

void MainWindow::on_pushButtonRegresarInicio_clicked()
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

void MainWindow::on_pushButtonResultados_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetResults);
    const QString name=ui->lineEditNombrPaciente->text();
    if(name=="")
        ui->labelResultsName->setText(tr("Paciente: %1").arg("Sin Nombre"));
    else
        ui->labelResultsName->setText(tr("Paciente: %1").arg(name));

    double q1=0, q2=0, q3=0, q4=0;

    foreach (Dato *var, listaMuestras) {//Se recorren las muestras y compara para determinar en que cuadrante estan.
        if(var->getAnguloX()>0){
            if(var->getAnguloY()>0)
                q1+=1;
            else
                q3+=1;
        }
        else{
            if(var->getAnguloY()>0)
                q2+=1;
            else
                q4+=1;
        }
    }

    q1=q1/listaMuestras.size()*100;
    q2=q2/listaMuestras.size()*100;
    q3=q3/listaMuestras.size()*100;
    q4=q4/listaMuestras.size()*100;
    qDebug()<<"1="<<q1<<"% 2="<<q2<<"% 3="<<q3<<"% 4="<<q4<<"%"<<endl;

    QCPBars *quadrants = new QCPBars(ui->qCustomPlotResultados->xAxis, ui->qCustomPlotResultados->yAxis);
    ui->qCustomPlotResultados->addPlottable(quadrants);
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
    ui->qCustomPlotResultados->xAxis->setAutoTicks(false);
    ui->qCustomPlotResultados->xAxis->setAutoTickLabels(false);
    ui->qCustomPlotResultados->xAxis->setTickVector(ticks);
    ui->qCustomPlotResultados->xAxis->setTickVectorLabels(labels);
    ui->qCustomPlotResultados->xAxis->setTickLabelRotation(60);
    ui->qCustomPlotResultados->xAxis->setSubTickCount(0);
    ui->qCustomPlotResultados->xAxis->setTickLength(0, 4);
    ui->qCustomPlotResultados->xAxis->grid()->setVisible(true);
    ui->qCustomPlotResultados->xAxis->setRange(0, 5);

    // prepare y axis:
    ui->qCustomPlotResultados->yAxis->setRange(0, 100);
    ui->qCustomPlotResultados->yAxis->setPadding(5); // a bit more space to the left border
    ui->qCustomPlotResultados->yAxis->setLabel("Porcentaje");
    ui->qCustomPlotResultados->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->qCustomPlotResultados->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->qCustomPlotResultados->yAxis->grid()->setSubGridPen(gridPen);

    // Add data:
    QVector<double> quadrantData;
    quadrantData  << q1 << q2 << q3 << q4;
    quadrants->setData(ticks, quadrantData);

    // setup legend:
    ui->qCustomPlotResultados->legend->setVisible(true);
    ui->qCustomPlotResultados->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->qCustomPlotResultados->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->qCustomPlotResultados->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->qCustomPlotResultados->legend->setFont(legendFont);
    ui->qCustomPlotResultados->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->qCustomPlotResultados->replot();
}

void MainWindow::on_pushButtonPrueba1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
}

void MainWindow::on_pushButtonGuardarImagen_clicked()
{
    QString filters("Imagen PNG (*.png);;Imagen JPG (*.jpg);;Archivo PDF (*.pdf)");
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, "Guardar Imagen","",filters,&selectedFilter);
    //qDebug()<<selectedFilter;
//    ui->qCustomPlotGrafico->xAxis->setRange(-30,30);
//    ui->qCustomPlotGrafico->yAxis->setRange(-30,30);
    if(selectedFilter.contains("PNG"))
        ui->qCustomPlotGrafico->savePng(fileName,1000,1000);
    if(selectedFilter.contains("JPG"))
        ui->qCustomPlotGrafico->saveJpg(fileName,1000,1000);
    if(selectedFilter.contains("PDF"))
      ui->qCustomPlotGrafico->savePdf(fileName,false,1000,1000);
}

void MainWindow::on_pushButtonGuardarMuestras_clicked()
{
    QString selectedFilter;
    QString filters(tr("Tipo de archivo (*.txt);;CSV (*.csv)"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar el Archivo"), "",
                                                    filters,&selectedFilter);

    //qDebug()<<selectedFilter;
    if (fileName != "") {
        QFile file(fileName);
        file.remove();
        if (file.open(QIODevice::Append)) {
            QTextStream stream(&file);
            foreach (Dato *var, listaMuestras) {
                if(selectedFilter.contains("txt")){
                    stream <<"Tiempo: " << QString::number(var->getTiempo(),'f',3) << " X: " << QString::number(var->getAnguloX(),'f',3)
                           << " Y: " << QString::number(var->getAnguloY(),'f',3) << endl;
                }
                if(selectedFilter.contains("csv")){
                    stream <<QString::number(var->getTiempo(),'f',3) << ";" <<QString::number(var->getAnguloX(),'f',3)
                           <<";" << QString::number(var->getAnguloY(),'f',3) << endl;
                }
            }
            file.flush();
            file.close();
        }
        else {
            QMessageBox::critical(this, tr("Error"), tr("No se pudo guardar el archivo"));
            return;
        }
    }
}

void MainWindow::on_dockWidget_topLevelChanged(bool topLevel)
{
   relacionAspectodelGrafico();
   if(topLevel){
       QTextStream(stdout)<<"flotando";
   }
}

