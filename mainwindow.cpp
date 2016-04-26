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

void MainWindow::inicializar()
{
    ui->setupUi(this);
    serial=new QSerialPort(this);

    ajustesSerial= new AjustesPuertoSerial;
    ajustesSensores = new AjustesSensores;
    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);

    status = new QLabel;
    ui->statusBar->addWidget(status);
    ui->dockWidget->installEventFilter(this);
    ui->qCustomPlotGrafico->setContextMenuPolicy(Qt::CustomContextMenu);

    QCPPlotTitle *titulo = new QCPPlotTitle(ui->qCustomPlotGrafico);
    titulo->setText("Grafico Angulos Antero-Posterior y Medio Lateral");
    titulo->setFont(QFont("sans", 10, QFont::Normal));
    ui->qCustomPlotGrafico->plotLayout()->insertRow(0);
    ui->qCustomPlotGrafico->plotLayout()->addElement(0, 0,titulo);
}

void MainWindow::conexiones()
{
    connect(ui->actionConfigurar_Serial,SIGNAL(triggered()),ajustesSerial,SLOT(show()));
    connect(ui->actionConfigurar_Sensores,SIGNAL(triggered(bool)),ajustesSensores,SLOT(show()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(leerDatosSerial()));
    connect(this,SIGNAL(emitdata(Dato*)),this,SLOT(slotDatosTiempoReal(Dato*)));
    connect(ui->verticalSliderRangeGraphic,SIGNAL(valueChanged(int)),this,SLOT(RangeGraphic(int)));
    connect(ui->qCustomPlotGrafico,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(ZoomGraphic(QWheelEvent*)));
    connect(ui->qCustomPlotGrafico, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

    connect(ui->actionInicio,SIGNAL(triggered(bool)),this,SLOT(regresarInicio()));
    connect(ui->actionSalir,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionQT,SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));

}

void MainWindow::inicializarGrafico()
{
    limpiarGrafico(ui->qCustomPlotGrafico);

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

    ui->qCustomPlotGrafico->addGraph(); // Para el Grafico del punto Rojo
    ui->qCustomPlotGrafico->graph(0)->setPen(QPen(Qt::red));
    ui->qCustomPlotGrafico->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->qCustomPlotGrafico->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

    //Se configuran los rangos maximos para los ejes X e Y segun el slider.
    const int range=ui->verticalSliderRangeGraphic->value();
    ui->qCustomPlotGrafico->xAxis->setRange(-range,range);
    ui->qCustomPlotGrafico->yAxis->setRange(-range,range);

    ui->qCustomPlotGrafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // Para usar el el Zoom y el Arrastre del grafico.
}

void MainWindow::mostrarMensajeBarraEstado(const QString &message)
{
    status->setText(message);
}

void MainWindow::leerDatosSerial()
{
    if ( cronometro.elapsed()/1000.0 <= ui->spinBoxTiempoPrueba->value()){
        while (serial->canReadLine()){
            const QByteArray serialData = serial->readLine();
            datosLeidosPuertoSerial=QString(serialData);

            QStringList linea=datosLeidosPuertoSerial.split(" ");
            const double AcX=QString(linea.at(0)).toDouble();
            const double AcY=QString(linea.at(1)).toDouble();
            const double AcZ=QString(linea.at(2)).toDouble();
            const double GyX=QString(linea.at(3)).toDouble();
            const double GyY=QString(linea.at(4)).toDouble();
            const double GyZ=QString(linea.at(5)).toDouble();

            cantidadMuestras+=1;

            if(cantidadMuestras==1)//Cuando se agrega el primer dato, se inicia el tiempo.
                cronometro.start();

            const double tiempo=cronometro.elapsed()/1000.0;
            Dato *dato=new Dato(tiempo,AcX,AcY,AcZ,GyX,GyY,GyZ);
            listaMuestras.append(dato);

            //if(cantidadMuestras %ui->spinBoxgraphupdate->value()==0)//Mod
            //    emit emitdata(dato);


            const double porcentaje=(tiempo/ui->spinBoxTiempoPrueba->value())*100+0.1;
            ui->progressBarPrueba->setValue(porcentaje);

            const QString lapso=QString::number(tiempo, 'f', 1);
            ui->lcdNumberTiempoTranscurrido->display(lapso);
            const QString mensaje="Tiempo: "+ lapso + " Muestras:" + QString::number(listaMuestras.size())+ " AcX: "+QString::number(AcX,'f',3)+" AcY: "+QString::number(AcY,'f',3)+" AcZ: "+QString::number(AcZ,'f',3)
                                + " GyX: "+QString::number(GyX,'f',3)+" GyY: "+QString::number(GyY,'f',3)+" GyZ: "+QString::number(GyZ,'f',3);
            mostrarMensajeBarraEstado(mensaje);

            //QTextStream(stdout)<<"Tiempo:"<< timer.elapsed()/1000.0 << " Muestras:"<< samplesList.size() << "  X: "<<data->getAngleX()<<" Y: "<< data->getAngleY() <<endl;
        }
    }
    else{
        QTextStream(stdout)<<"Muestras x Seg: "<<double(cantidadMuestras)/ui->spinBoxTiempoPrueba->value()<<endl;
        serial->close();
        mostrarBotones();
        ui->tabWidgetGrafico_Resultados->setTabEnabled(1,true);

        generarGraficoResultados(ui->qCustomPlotResultados_tab);
    }
}

void MainWindow::mostrarBotones()
{
    ui->pushButtonReiniciarPrueba->show();
    ui->pushButtonResultados->show();
    ui->pushButtonGuardarImagen->show();
    ui->pushButtonGuardarMuestras->show();
    ui->pushButtonConfPrueba->show();
    ui->pushButtonDetenerPrueba->hide();

}

void MainWindow::slotDatosTiempoReal(Dato *data)
{
    lienzo->addData(data->getAcX(), data->getAcY());

    ui->qCustomPlotGrafico->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto rojo.
    ui->qCustomPlotGrafico->graph(0)->addData(data->getAcX(), data->getAcY());
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
    (void) event;//Variable no usada
    QCPRange range=ui->qCustomPlotGrafico->xAxis->range();

    ui->qCustomPlotGrafico->xAxis->setRange(range);
    ui->qCustomPlotGrafico->yAxis->setRange(range);

    ui->verticalSliderRangeGraphic->setValue(range.upper);

}

void MainWindow::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu->addAction("Guardar Imagen", this, SLOT(on_pushButtonGuardarImagen_clicked()));
    menu->addAction("Guardar Muestras", this, SLOT(on_pushButtonGuardarMuestras_clicked()));
    menu->popup(ui->qCustomPlotGrafico->mapToGlobal(pos));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
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
        ui->qCustomPlotGrafico->setGeometry(rect.x(),rect.y()+((h-w)/2),w,w);
}

void MainWindow::abrirPuertoSerial()
{
    cantidadMuestras=0;
    listaMuestras.clear();

    AjustesPuertoSerial::Ajustes cs=ajustesSerial->getAjustes();
    serial->setPortName(cs.portName);
    serial->setBaudRate(cs.baudRate);
    QTextStream(stdout)<<"Baudios: "<< serial->baudRate()<<endl;
    QTextStream(stdout)<<"portName"<< serial->portName()<<endl;
    serial->setStopBits(QSerialPort::OneStop);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)){
        serial->clear();
        QString cadena=ajustesSensores->getAjustes();
        QTextStream(stdout)<<"Cadena de Configuracion: " <<cadena<<endl;
        //serial->dataTerminalReadyChanged(true);
        //serial->requestToSendChanged(true);
        serial->write(cadena.toLocal8Bit());

        //serial->waitForBytesWritten(1000);
        //emit emitEscribirSerial(cadena);
        //serial->waitForBytesWritten(2000);
        cronometro.start();

        inicializarGrafico(); //Se limpian los graficos

        ui->pushButtonDetenerPrueba->show();
        ui->pushButtonConfPrueba->hide();
        ui->pushButtonResultados->hide();
        ui->pushButtonGuardarImagen->hide();
        ui->pushButtonGuardarMuestras->hide();
        ui->tabWidgetGrafico_Resultados->setTabEnabled(1,false);

    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
}

void MainWindow::cerrarPuertoSerial()
{
    if (serial->isOpen()){
        serial->clear();
        serial->close();
    }
}

void MainWindow::regresarInicio()
{
    if(ui->stackedWidget->currentWidget()!=ui->widgetWelcome){
          QMessageBox messageBox(QMessageBox::Question,
                      tr("¿Volver a Pagina Principal?"),
                      tr("¿Seguro que desea volver a Inicio\nTodos los resultados no guardados se perderán?"),
                      QMessageBox::Yes | QMessageBox::No,
                      this);
          messageBox.setButtonText(QMessageBox::Yes, tr("Si"));
          messageBox.setButtonText(QMessageBox::No, tr("No"));

          if (messageBox.exec() == QMessageBox::Yes){
              ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);
              cerrarPuertoSerial();
          }
      }
}

void MainWindow::on_pushButtonIniciarPrueba_clicked()
{
    abrirPuertoSerial();
    ui->stackedWidget->setCurrentWidget(ui->widgetTest);
    ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_grafico);
}

void MainWindow::on_pushButtonReiniciarPrueba_clicked()
{
    cerrarPuertoSerial();
    abrirPuertoSerial();
}

void MainWindow::on_pushButtonDetenerPrueba_clicked()
{
    cerrarPuertoSerial();
    mostrarBotones();
}

void MainWindow::on_pushButtonRegresarInicio_clicked()
{
    if(ui->stackedWidget->currentWidget()!=ui->widgetWelcome){
        QMessageBox messageBox(QMessageBox::Question,
                    tr("¿Volver a Pagina Principal?"),
                    tr("¿Seguro que desea volver a Inicio\nTodos los resultados no guardados se perderán?"),
                    QMessageBox::Yes | QMessageBox::No,
                    this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Si"));
        messageBox.setButtonText(QMessageBox::No, tr("No"));

        if (messageBox.exec() == QMessageBox::Yes){
            ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);
            cerrarPuertoSerial();
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);
}

void MainWindow::on_pushButtonResultados_clicked()
{
    generarGraficoResultados(ui->qCustomPlotResultados);
    ui->stackedWidget->setCurrentWidget(ui->widgetResults);
    const QString name=ui->lineEditNombrPaciente->text();
    if(name=="")
        ui->labelResultsName->setText(tr("Paciente: %1").arg("Sin Nombre"));
    else
        ui->labelResultsName->setText(tr("Paciente: %1").arg(name));
}

void MainWindow::limpiarGrafico(QCustomPlot *grafico){

    if(grafico->plotLayout()->hasElement(0,1))
        grafico->plotLayout()->remove(grafico->plotLayout()->element(0,1));

    grafico->legend->clear();
    grafico->legend->setVisible(false);
    grafico->clearGraphs();
    grafico->clearPlottables();
    grafico->xAxis->setLabel("");
    grafico->yAxis->setLabel("");
    grafico->rescaleAxes();
    grafico->replot();
}

void MainWindow::generarGraficoResultados(QCustomPlot *grafico)
{
    limpiarGrafico(grafico);
    double q1=0, q2=0, q3=0, q4=0;

    foreach (Dato *var, listaMuestras) {//Se recorren las muestras y compara para determinar en que cuadrante estan.
        if(var->getAcX()>0){
            if(var->getAcY()>0)
                q1+=1;
            else
                q3+=1;
        }
        else{
            if(var->getAcY()>0)
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

    QCPBars *cuadrantes = new QCPBars(grafico->xAxis, grafico->yAxis);
    grafico->addPlottable(cuadrantes);
    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    cuadrantes->setName("Porcentaje en cada cuadrante");
    pen.setColor(QColor(255, 131, 0));
    cuadrantes->setPen(pen);
    cuadrantes->setBrush(QColor(255, 131, 0, 50));

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4;
    labels << "Cuadrante 1" << "Cuadrante 2" << "Cuadrante 3" << "Cuadrante 4";
    grafico->xAxis->setAutoTicks(false);
    grafico->xAxis->setAutoTickLabels(false);
    grafico->xAxis->setTickVector(ticks);
    grafico->xAxis->setTickVectorLabels(labels);
    grafico->xAxis->setTickLabelRotation(60);
    grafico->xAxis->setSubTickCount(0);
    grafico->xAxis->setTickLength(0, 4);
    grafico->xAxis->grid()->setVisible(true);
    grafico->xAxis->setRange(0, 5);

    // prepare y axis:
    grafico->yAxis->setRange(0, 100);
    grafico->yAxis->setPadding(5); // a bit more space to the left border
    grafico->yAxis->setLabel("Porcentaje");
    grafico->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    grafico->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    grafico->yAxis->grid()->setSubGridPen(gridPen);

    // Add data:
    QVector<double> quadrantData;
    quadrantData  << q1 << q2 << q3 << q4;
    cuadrantes->setData(ticks, quadrantData);

    // setup legend:
    grafico->legend->setVisible(true);
    grafico->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    grafico->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    grafico->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    grafico->legend->setFont(legendFont);
    grafico->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    grafico->replot();
}

void MainWindow::on_pushButtonPrueba1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
}

void MainWindow::on_pushButtonConfPrueba_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
}

void MainWindow::on_pushButtonGuardarImagen_clicked()
{
    QString filters("Imagen PNG (*.png);;Imagen JPG (*.jpg);;Archivo PDF (*.pdf)");
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, "Guardar Imagen","",filters,&selectedFilter);
    ui->qCustomPlotGrafico->xAxis->setRange(-30,30);
    ui->qCustomPlotGrafico->yAxis->setRange(-30,30);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_resultados){
        if(selectedFilter.contains("PNG"))
            ui->qCustomPlotResultados_tab->savePng(fileName,1000,1000);
        if(selectedFilter.contains("JPG"))
            ui->qCustomPlotResultados_tab->saveJpg(fileName,1000,1000);
        if(selectedFilter.contains("PDF"))
          ui->qCustomPlotResultados_tab->savePdf(fileName,false,1000,1000);

    }
    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_grafico)
    {
        if(selectedFilter.contains("PNG"))
            ui->qCustomPlotGrafico->savePng(fileName,1000,1000);
        if(selectedFilter.contains("JPG"))
            ui->qCustomPlotGrafico->saveJpg(fileName,1000,1000);
        if(selectedFilter.contains("PDF"))
          ui->qCustomPlotGrafico->savePdf(fileName,false,1000,1000);
    }
}

void MainWindow::on_pushButtonGuardarMuestras_clicked()
{
    QString selectedFilter;
    QString filters(tr("CSV (*.csv);;Archivo de Texto (*.txt)"));
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
                    stream <<"Tiempo: " << QString::number(var->getTiempo(),'f',3) << " X: " << QString::number(var->getAcX(),'f',3)
                           << " Y: " << QString::number(var->getAcY(),'f',3) <<  " Z: " << QString::number(var->getAcZ(),'f',3) <<  " X: " << QString::number(var->getGyX(),'f',3) << endl;
                }
                if(selectedFilter.contains("csv")){
                    stream <<QString::number(var->getTiempo(),'f',3) << ";" <<QString::number(var->getAcX(),'f',3)
                           <<";" << QString::number(var->getAcY(),'f',3) << endl;
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

void MainWindow::on_tabWidgetGrafico_Resultados_currentChanged(int index)
{
    if(index==0)
        relacionAspectodelGrafico();//Al cambiar a la pestaña del grafico se reajusta.
}
