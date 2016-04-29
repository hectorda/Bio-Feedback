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

    QHeaderView* header = ui->tableWidgetDatosRaw->horizontalHeader(); //Para que el header de la table se ajuste
    header->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::conexiones()
{
    connect(ui->actionConfigurar_Serial,SIGNAL(triggered()),ajustesSerial,SLOT(show()));
    connect(ui->actionConfigurar_Sensores,SIGNAL(triggered(bool)),ajustesSensores,SLOT(show()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(leerDatosSerial()));
    connect(this,SIGNAL(emitAngulo(Angulo*)),this,SLOT(slotGraficarTiempoReal(Angulo*)));
    connect(ui->verticalSliderRangeGraphic,SIGNAL(valueChanged(int)),this,SLOT(RangeGraphic(int)));
    connect(ui->qCustomPlotGrafico,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(ZoomGraphic(QWheelEvent*)));
    connect(ui->qCustomPlotGrafico, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

    connect(ui->actionInicio,SIGNAL(triggered()),this,SLOT(regresarInicio()));
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

    int rObjetivo=1;

    foreach (QPoint *P, listaObjetivos) {
        QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
        objetivo->topLeft->setCoords(P->x()-rObjetivo,P->y()+rObjetivo);
        objetivo->bottomRight->setCoords(P->x()+rObjetivo,P->y()-rObjetivo);
        objetivo->setBrush(QBrush(Qt::red));
    }

    lienzo = new QCPCurve(ui->qCustomPlotGrafico->xAxis,ui->qCustomPlotGrafico->yAxis);
    ui->qCustomPlotGrafico->addPlottable(lienzo);

    ui->qCustomPlotGrafico->addGraph(); // Para el Grafico del punto
    ui->qCustomPlotGrafico->graph(0)->setPen(QPen(Qt::blue));
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

            if(linea.size()>6)
                QTextStream(stdout)<<"key: "<<linea.at(6)<<endl;

            cantidadMuestras+=1;

            if(cantidadMuestras==1)//Cuando se agrega el primer dato, se inicia el tiempo.
                cronometro.start();

            const double tiempo=cronometro.elapsed()/1000.0;
            Raw *dato=new Raw(tiempo,AcX,AcY,AcZ,GyX,GyY,GyZ);
            if (cantidadMuestras>1)
                obtenerAngulos(dato);

            listaMuestras.append(dato);
            const double porcentaje=(tiempo/ui->spinBoxTiempoPrueba->value())*100+0.1;
            ui->progressBarPrueba->setValue(porcentaje);

            const QString lapso=QString::number(tiempo, 'f', 1);
            ui->lcdNumberTiempoTranscurrido->display(lapso);
            const QString mensaje="Tiempo: "+ lapso + " Muestras:" + QString::number(listaMuestras.size())+ " AcX: "+QString::number(AcX,'f',3)+" AcY: "+QString::number(AcY,'f',3)+" AcZ: "+QString::number(AcZ,'f',3)
                                + " GyX: "+QString::number(GyX,'f',3)+" GyY: "+QString::number(GyY,'f',3)+" GyZ: "+QString::number(GyZ,'f',3);
            mostrarMensajeBarraEstado(mensaje);
            //if(cantidadMuestras %ui->spinBoxgraphupdate->value()==0)//Mod
            //QTextStream(stdout)<<mensaje;

        }
    }
    else{
        QTextStream(stdout)<<"Muestras x Seg: "<<double(cantidadMuestras)/ui->spinBoxTiempoPrueba->value()<<endl;
        serial->close();
        mostrarBotones();
        activarTabs();
        generarGraficoResultados(ui->qCustomPlotResultados_tab);
        generarTablaRaw();
        generarGraficosRaw();
    }
}

void MainWindow::obtenerAngulos(Raw *dato)
{
    const double RAD_TO_DEG=180/M_PI;
    //Se calculan los angulos con la IMU vertical.
    const double angulo1 = qAtan(dato->getAcZ()/qSqrt(qPow(dato->getAcX(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;
    const double angulo2 = qAtan(dato->getAcX()/qSqrt(qPow(dato->getAcZ(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;
    double dt=dato->getTiempo()-listaMuestras.last()->getTiempo();
    //Aplicar el Filtro Complementario
    anguloComplementario1 = 0.98 *(anguloComplementario1+dato->getGyX()*dt) + 0.02*angulo1;
    anguloComplementario2 = 0.98 *(anguloComplementario2+dato->getGyZ()*dt) + 0.02*angulo2;

    if(cantidadMuestras %ui->spinBoxgraphupdate->value()==0)//Mod
        emit emitAngulo(new Angulo(dato->getTiempo(),-anguloComplementario2,anguloComplementario1));
        //emit emitdata(new Dato(dato->getTiempo(),Angle_compl[0],Angle_compl[1],0,0,0,0));

    //QTextStream(stdout)<<"Angulo normal X:"<<QString::number(angulo1,'f',2)<<" Angulo normal Y:"<<QString::number(angulo2,'f',2) <<" Angulo Compl X:"<<QString::number(anguloComplementario1,'f',2)<<" Angulo Compl Y:"<<QString::number(anguloComplementario2,'f',2);

}

void MainWindow::mostrarBotones()
{
    ui->pushButtonReiniciarPrueba->show();
    ui->pushButtonGuardarImagen->show();
    ui->pushButtonGuardarMuestras->show();
    ui->pushButtonConfPrueba->show();
    ui->pushButtonDetenerPrueba->hide();
    ui->pushButtonGuardarImagen->show();
    ui->labelGuardarImagen->show();
    ui->pushButtonGuardarMuestras->show();
    ui->labelGuardarMuestras->show();
    activarSpacerEntreBotones();
}

void MainWindow::ocultarBotones()
{
    ui->pushButtonDetenerPrueba->show();
    ui->pushButtonConfPrueba->hide();
    ui->pushButtonGuardarImagen->hide();
    ui->pushButtonGuardarMuestras->hide();
    ui->labelGuardarImagen->hide();
    ui->labelGuardarMuestras->hide();
}

void MainWindow::desactivarTabs()
{
    ui->tabWidgetGrafico_Resultados->setTabEnabled(1,false);
    ui->tabWidgetGrafico_Resultados->setTabEnabled(2,false);
    ui->tabWidgetGrafico_Resultados->setTabEnabled(3,false);
}

void MainWindow::activarTabs()
{
    ui->tabWidgetGrafico_Resultados->setTabEnabled(1,true);
    ui->tabWidgetGrafico_Resultados->setTabEnabled(2,true);
    ui->tabWidgetGrafico_Resultados->setTabEnabled(3,true);
}

void MainWindow::activarSpacerEntreBotones()
{
    ui->verticalSpacerEntreBotones->changeSize(40,20,QSizePolicy::Ignored,QSizePolicy::Expanding);
    ui->centralWidget->adjustSize();
}

void MainWindow::desactivarSpacerEntreBotones()
{
    ui->verticalSpacerEntreBotones->changeSize(40,20,QSizePolicy::Ignored,QSizePolicy::Ignored);
}

void MainWindow::generarObjetivos(int rexterior=20,int rObjetivo=1,int distanciaCentro=5)
{
    listaObjetivos.clear();
    int cantidadObjetivos=ui->spinBoxCantidadObjetivos->value();

    if(ui->checkBoxObjetivosAleatorios->isChecked()){
        while(listaObjetivos.size()<cantidadObjetivos){
           //para los Random
            const int signox=qrand()%2==1 ? 1: -1;
            const int signoy=qrand()%2==1 ? 1: -1;

            const int randomx=(qrand()%(rexterior-rObjetivo))*signox;
            const int randomy=(qrand()%(rexterior-rObjetivo))*signoy;
            const double ecuacionCircExt=qPow(randomx,2)+qPow(randomy,2);

            if(ecuacionCircExt<=qPow((rexterior-rObjetivo),2)){
                bool noIntersectaOtros=true;
                foreach (QPoint *P, listaObjetivos){
                    const double perteneceCirc=qSqrt(qPow((randomx - P->x()),2)+qPow((randomy - P->y()),2));
                    if( perteneceCirc < 2*rObjetivo + 0.5)
                        noIntersectaOtros=false;
                }
                if(noIntersectaOtros){
//                    QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
//                    objetivo->topLeft->setCoords(randomx-rObjetivo,randomy+rObjetivo);
//                    objetivo->bottomRight->setCoords(randomx+rObjetivo,randomy-rObjetivo);
//                    objetivo->setBrush(QBrush(Qt::red));
                    listaObjetivos.append(new QPoint(randomx,randomy));
                }
            }
        }
    }
    else{
        for (int var = 0; var < cantidadObjetivos; ++var){
//            QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
            const double angulo=var*((2*M_PI)/cantidadObjetivos); //
//            objetivo->topLeft->setCoords(qCos(angulo)*distanciaCentro-rObjetivo,qSin(angulo)*distanciaCentro+rObjetivo);
//            objetivo->bottomRight->setCoords(qCos(angulo)*distanciaCentro+rObjetivo,qSin(angulo)*distanciaCentro-rObjetivo);
//            objetivo->setBrush(QBrush(Qt::red));
            listaObjetivos.append(new QPoint(qCos(angulo)*distanciaCentro,qSin(angulo)*distanciaCentro));
        }
    }
}

void MainWindow::generarTablaRaw()
{
    if (!ui->tableWidgetDatosRaw)
       return;
    while (ui->tableWidgetDatosRaw->rowCount() > 0)
    {
        ui->tableWidgetDatosRaw->removeRow(0);
    }

    for (int var = 0; var < cantidadMuestras; ++var) {
        const int currentRow = ui->tableWidgetDatosRaw->rowCount();
        ui->tableWidgetDatosRaw->setRowCount(currentRow + 1);
        ui->tableWidgetDatosRaw->setItem(var,0,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getTiempo())));
        ui->tableWidgetDatosRaw->setItem(var,1,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getAcX())));
        ui->tableWidgetDatosRaw->setItem(var,2,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getAcY())));
        ui->tableWidgetDatosRaw->setItem(var,3,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getAcZ())));
        ui->tableWidgetDatosRaw->setItem(var,4,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getGyX())));
        ui->tableWidgetDatosRaw->setItem(var,5,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getGyY())));
        ui->tableWidgetDatosRaw->setItem(var,6,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getGyZ())));
    }

}

void MainWindow::generarGraficosRaw()
{
    QVector<double> Tiempo;
    QVector<double> DatosAcX;
    QVector<double> DatosAcY;
    QVector<double> DatosAcZ;
    QVector<double> DatosGyX;
    QVector<double> DatosGyY;
    QVector<double> DatosGyZ;
    foreach (Raw *var, listaMuestras) {
        Tiempo.append(var->getTiempo());
        DatosAcX.append(var->getAcX());
        DatosAcY.append(var->getAcY());
        DatosAcZ.append(var->getAcZ());
        DatosGyX.append(var->getGyX());
        DatosGyY.append(var->getGyY());
        DatosGyZ.append(var->getGyZ());
    }
    limpiarGrafico(ui->qCustomPlotGraficoAcX);
    ui->qCustomPlotGraficoAcX->addGraph();
    ui->qCustomPlotGraficoAcX->graph(0)->setData(Tiempo, DatosAcX);
    ui->qCustomPlotGraficoAcX->xAxis->setLabel("Tiempo");
    ui->qCustomPlotGraficoAcX->yAxis->setLabel("Aceleracion X");
    ui->qCustomPlotGraficoAcX->xAxis->setRange(0, ui->spinBoxTiempoPrueba->value());
    ui->qCustomPlotGraficoAcX->yAxis->setRange(-1, 1);
    ui->qCustomPlotGraficoAcX->replot();
    ui->qCustomPlotGraficoAcX->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    limpiarGrafico(ui->qCustomPlotGraficoAcY);
    ui->qCustomPlotGraficoAcY->addGraph();
    ui->qCustomPlotGraficoAcY->graph(0)->setData(Tiempo, DatosAcY);
    ui->qCustomPlotGraficoAcY->xAxis->setLabel("Tiempo");
    ui->qCustomPlotGraficoAcY->yAxis->setLabel("Aceleracion Y");
    ui->qCustomPlotGraficoAcY->xAxis->setRange(0, ui->spinBoxTiempoPrueba->value());
    ui->qCustomPlotGraficoAcY->yAxis->setRange(-1, 1);
    ui->qCustomPlotGraficoAcY->replot();
    ui->qCustomPlotGraficoAcY->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    limpiarGrafico(ui->qCustomPlotGraficoAcZ);
    ui->qCustomPlotGraficoAcZ->addGraph();
    ui->qCustomPlotGraficoAcZ->graph(0)->setData(Tiempo, DatosAcZ);
    ui->qCustomPlotGraficoAcZ->xAxis->setLabel("Tiempo");
    ui->qCustomPlotGraficoAcZ->yAxis->setLabel("Aceleracion Z");
    ui->qCustomPlotGraficoAcZ->xAxis->setRange(0, ui->spinBoxTiempoPrueba->value());
    ui->qCustomPlotGraficoAcZ->yAxis->setRange(-1, 1);
    ui->qCustomPlotGraficoAcZ->replot();
    ui->qCustomPlotGraficoAcZ->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    limpiarGrafico(ui->qCustomPlotGraficoGyX);
    ui->qCustomPlotGraficoGyX->addGraph();
    ui->qCustomPlotGraficoGyX->graph(0)->setData(Tiempo, DatosGyX);
    ui->qCustomPlotGraficoGyX->xAxis->setLabel("Tiempo");
    ui->qCustomPlotGraficoGyX->yAxis->setLabel("Rotacion X");
    ui->qCustomPlotGraficoGyX->xAxis->setRange(0, ui->spinBoxTiempoPrueba->value());
    ui->qCustomPlotGraficoGyX->yAxis->setRange(-250, 250);
    ui->qCustomPlotGraficoGyX->replot();
    ui->qCustomPlotGraficoGyX->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    limpiarGrafico(ui->qCustomPlotGraficoGyY);
    ui->qCustomPlotGraficoGyY->addGraph();
    ui->qCustomPlotGraficoGyY->graph(0)->setData(Tiempo, DatosGyY);
    ui->qCustomPlotGraficoGyY->xAxis->setLabel("Tiempo");
    ui->qCustomPlotGraficoGyY->yAxis->setLabel("Rotacion Y");
    ui->qCustomPlotGraficoGyY->xAxis->setRange(0, ui->spinBoxTiempoPrueba->value());
    ui->qCustomPlotGraficoGyY->yAxis->setRange(-250, 250);
    ui->qCustomPlotGraficoGyY->replot();
    ui->qCustomPlotGraficoGyY->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    limpiarGrafico(ui->qCustomPlotGraficoGyZ);
    ui->qCustomPlotGraficoGyZ->addGraph();
    ui->qCustomPlotGraficoGyZ->graph(0)->setData(Tiempo, DatosGyZ);
    ui->qCustomPlotGraficoGyZ->xAxis->setLabel("Tiempo");
    ui->qCustomPlotGraficoGyZ->yAxis->setLabel("Rotacion Z");
    ui->qCustomPlotGraficoGyZ->xAxis->setRange(0, ui->spinBoxTiempoPrueba->value());
    ui->qCustomPlotGraficoGyZ->yAxis->setRange(-250, 250);
    ui->qCustomPlotGraficoGyZ->replot();
    ui->qCustomPlotGraficoGyZ->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
}

void MainWindow::slotGraficarTiempoReal(Angulo *angulo)
{
    lienzo->addData(angulo->getAnguloX(), angulo->getAnguloY());

    ui->qCustomPlotGrafico->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto rojo.
    ui->qCustomPlotGrafico->graph(0)->addData(angulo->getAnguloX(), angulo->getAnguloY());
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
        serial->write("v0"+cadena.toLocal8Bit());

        cronometro.start();
        generarObjetivos();
        inicializarGrafico(); //Se limpian los graficos

        desactivarTabs();
        desactivarSpacerEntreBotones();
        ocultarBotones();


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

void MainWindow::preguntarRegresarInicio()
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

void MainWindow::on_pushButtonVolverInicio_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);
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

    foreach (Raw *var, listaMuestras) {//Se recorren las muestras y compara para determinar en que cuadrante estan.
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
            foreach (Raw *var, listaMuestras) {
                if(selectedFilter.contains("txt")){
                    stream <<"Tiempo: " << QString::number(var->getTiempo(),'f',3) << " X: " << QString::number(var->getAcX(),'f',3)
                           << " Y: " << QString::number(var->getAcY(),'f',3) <<  " Z: " << QString::number(var->getAcZ(),'f',3) <<  " X: " << QString::number(var->getGyX(),'f',3) << endl;
                }
                if(selectedFilter.contains("csv")){
                    stream <<QString::number(var->getTiempo(),'f',3) << ";" <<QString::number(var->getAcX(),'f',3)
                           <<";" << QString::number(var->getAcY(),'f',3) <<";" << QString::number(var->getAcZ(),'f',3)
                           <<";" << QString::number(var->getGyX(),'f',3) <<";" << QString::number(var->getGyY(),'f',3)
                           <<";" << QString::number(var->getGyZ(),'f',3) << endl;
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

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_grafico)
    {
        ui->pushButtonGuardarImagen->show();
        ui->labelGuardarImagen->setText("Guardar\nGráfico");
        ui->labelGuardarImagen->show();

        ui->pushButtonGuardarMuestras->show();
        ui->labelGuardarMuestras->setText("Guardar\nAngulos");
        ui->labelGuardarMuestras->show();

        activarSpacerEntreBotones();
    }
    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_resultados)
    {
        ui->pushButtonGuardarImagen->show();
        ui->labelGuardarImagen->setText("Guardar\nGrafico");
        ui->labelGuardarImagen->show();

        ui->pushButtonGuardarMuestras->hide();
        ui->labelGuardarMuestras->hide();
        desactivarSpacerEntreBotones();

    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_muestras)
    {
        ui->pushButtonGuardarImagen->hide();
        ui->labelGuardarImagen->hide();

        ui->pushButtonGuardarMuestras->show();
        ui->labelGuardarMuestras->setText("Guardar\nMuestras");
        ui->labelGuardarMuestras->show();

        desactivarSpacerEntreBotones();
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_graficosRaw)
    {
        ui->pushButtonGuardarImagen->show();
        ui->labelGuardarImagen->setText("Guardar\nGraficos Raw");
        ui->labelGuardarImagen->show();

        ui->pushButtonGuardarMuestras->show();
        ui->labelGuardarMuestras->setText("Guardar\nMuestras");
        ui->labelGuardarMuestras->show();

        activarSpacerEntreBotones();
    }

}
