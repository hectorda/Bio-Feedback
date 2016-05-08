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
    ajustesGrafico = new AjustesGrafico;
    reportes = new Reportes;

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

    ui->tableWidgetDatosRaw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetAngulos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    db = new SQL;
    db->consulta();
}

void MainWindow::conexiones()
{
    connect(serial, SIGNAL(readyRead()), this, SLOT(leerDatosSerial()));
    connect(this,SIGNAL(emitAngulo(Angulo*)),this,SLOT(slotGraficarTiempoReal(Angulo*)));
    connect(ui->verticalSliderRangeGraphic,SIGNAL(valueChanged(int)),this,SLOT(RangeGraphic(int)));
    connect(ui->qCustomPlotGrafico,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(ZoomGraphic(QWheelEvent*)));
    connect(ui->qCustomPlotGrafico, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

    //Connect de actions
    connect(ui->actionConfigurar_Serial,SIGNAL(triggered()),ajustesSerial,SLOT(show()));
    connect(ui->actionConfigurar_Sensores,SIGNAL(triggered(bool)),ajustesSensores,SLOT(show()));
    connect(ui->actionConfigurar_Grafico,SIGNAL(triggered(bool)),ajustesGrafico,SLOT(show()));

    connect(ui->actionSQL,SIGNAL(triggered(bool)),db,SLOT(show()));

    connect(ui->actionInicio,SIGNAL(triggered()),this,SLOT(regresarInicio()));
    connect(ui->actionSalir,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionQT,SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));
}

void MainWindow::inicializarGrafico()
{
    limpiarGrafico(ui->qCustomPlotGrafico);
    //ui->qCustomPlotGrafico->plotLayout()->clear();

    QCPItemEllipse *circuloExterior;
    circuloExterior= new QCPItemEllipse(ui->qCustomPlotGrafico);
    circuloExterior->topLeft->setCoords(-radios.RadioExterior,radios.RadioExterior);
    circuloExterior->bottomRight->setCoords(radios.RadioExterior,-radios.RadioExterior);
    circuloExterior->setBrush(QBrush(Qt::yellow));

    QCPItemEllipse *circuloInterior;
    circuloInterior= new QCPItemEllipse(ui->qCustomPlotGrafico);

    circuloInterior->topLeft->setCoords(-radios.RadioInterior,radios.RadioInterior);
    circuloInterior->bottomRight->setCoords(radios.RadioInterior,-radios.RadioInterior);

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

void MainWindow::actualizarMensajeBarraEstado(const QString &message)
{
    status->setText(message);
}

void MainWindow::leerDatosSerial()
{
    const double tiempoPrueba=pruebaNumero==1 ? qInf() :ui->spinBoxTiempoPrueba->value();
    if ( cronometro.elapsed()/1000.0 <=tiempoPrueba){

        while (serial->canReadLine()){
            const double tiempo=cronometro.elapsed()/1000.0;

            Raw raw=lecturaSerial->leerDatosSerial(serial,tiempo);
            Raw *dato=new Raw(raw.getTiempo(),raw.getAcX(),raw.getAcY(),raw.getAcZ(),raw.getGyX(),raw.getGyY(),raw.getGyZ());

            obtenerAngulos(dato);

            listaMuestras.append(dato);

            if(listaMuestras.size()==1)//Cuando se agrega el primer dato, se inicia el tiempo.
                cronometro.start();

            const double porcentaje=(tiempo/ui->spinBoxTiempoPrueba->value())*100+0.1;
            ui->progressBarPrueba->setValue(porcentaje);

            const QString lapso=QString::number(tiempo, 'f', 1);
            ui->lcdNumberTiempoTranscurrido->display(lapso);

            const QString mensaje="Tiempo: "+ lapso + " Muestras:" + QString::number(listaMuestras.size())+ " AcX: "+QString::number(dato->getAcX(),'f',3)+" AcY: "+QString::number(dato->getAcY(),'f',3)+" AcZ: "+QString::number(dato->getAcZ(),'f',3)
                                + " GyX: "+QString::number(dato->getGyX(),'f',3)+" GyY: "+QString::number(dato->getGyY(),'f',3)+" GyZ: "+QString::number(dato->getGyZ(),'f',3);
            actualizarMensajeBarraEstado(mensaje);
        }
    }
    else{
        mostrarResultados();
    }
}

void MainWindow::mostrarResultados()
{
    QTextStream(stdout)<<"Muestras x Seg: "<<double(listaMuestras.size())/ui->spinBoxTiempoPrueba->value()<<endl;
    lecturaSerial->cerrarPuertoSerial(serial);
    reportes->graficarResultados(ui->qCustomPlotResultados,listaAngulos);
    reportes->tablaAngulos(ui->tableWidgetAngulos,listaAngulos);
    reportes->tablaMuestras(ui->tableWidgetDatosRaw,listaMuestras);
    reportes->graficarAngulos(ui->qCustomPlotGraficosAngulos,listaAngulos);
    reportes->graficarMuestras(ui->qCustomPlotGraficoMuestras,listaMuestras);

    mostrarBotones();
    activarTabs();
}

void MainWindow::obtenerAngulos(Raw *dato)
{
    const double RAD_TO_DEG=180/M_PI;

    Angulo *angulo;
    const bool IMUVertical = ui->radioButtonIMUVertical->isChecked();
    const bool IMUHorizontal = ui->radioButtonIMUHorizonal->isChecked();
    if(IMUVertical)
    {
        //Se calculan los angulos con la IMU vertical.
        const double angulo1 = qAtan(dato->getAcX()/qSqrt(qPow(dato->getAcZ(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;
        const double angulo2 = qAtan(dato->getAcZ()/qSqrt(qPow(dato->getAcX(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;

        //Aplicar el Filtro Complementario
        if(listaAngulos.size()>0){
            const double dt=(dato->getTiempo()-listaAngulos.last()->getTiempo())/ 1000;
            anguloComplementario1 = 0.98 *(anguloComplementario1+dato->getGyZ()*dt) + 0.02*angulo1;
            anguloComplementario2 = 0.98 *(anguloComplementario2+dato->getGyX()*dt) + 0.02*angulo2;
        }
        else{
            anguloComplementario1=angulo1;
            anguloComplementario2=angulo2;
        }
        angulo=new Angulo(dato->getTiempo(),anguloComplementario1,anguloComplementario2);
    }

    if(IMUHorizontal)
    {
        //Se calculan los angulos con la IMU horizontal.
        const double angulo1 = qAtan(dato->getAcY()/qSqrt(qPow(dato->getAcX(),2) + qPow(dato->getAcZ(),2)))*RAD_TO_DEG;
        const double angulo2 = qAtan(dato->getAcX()/qSqrt(qPow(dato->getAcY(),2) + qPow(dato->getAcZ(),2)))*RAD_TO_DEG;

        //Aplicar el Filtro Complementario
        if(listaAngulos.size()>0){
            const double dt=(dato->getTiempo()-listaAngulos.last()->getTiempo())/ 1000;
            anguloComplementario1 = 0.98 *(anguloComplementario1+dato->getGyX()*dt) + 0.02*angulo1;
            anguloComplementario2 = 0.98 *(anguloComplementario2+dato->getGyY()*dt) + 0.02*angulo2;
        }
        else{
            anguloComplementario1=angulo1;
            anguloComplementario2=angulo2;
        }
        angulo=new Angulo(dato->getTiempo(),anguloComplementario1,anguloComplementario2);
    }

    listaAngulos.append(angulo);
    if(listaAngulos.size() %ui->spinBoxgraphupdate->value()==0)//Mod
        emit emitAngulo(angulo);
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
    ui->tabWidgetGrafico_Resultados->setTabEnabled(4,false);
    ui->tabWidgetGrafico_Resultados->setTabEnabled(5,false);
}

void MainWindow::activarTabs()
{
    ui->tabWidgetGrafico_Resultados->setTabEnabled(1,true);
    ui->tabWidgetGrafico_Resultados->setTabEnabled(2,true);
    ui->tabWidgetGrafico_Resultados->setTabEnabled(3,true);
    ui->tabWidgetGrafico_Resultados->setTabEnabled(4,true);
    ui->tabWidgetGrafico_Resultados->setTabEnabled(5,true);
}

void MainWindow::activarSpacerEntreBotones()
{
    ui->verticalSpacerEntreBotones->changeSize(40,20,QSizePolicy::Ignored,QSizePolicy::Expanding);
    //ui->centralWidget->adjustSize();
}

void MainWindow::desactivarSpacerEntreBotones()
{
    ui->verticalSpacerEntreBotones->changeSize(40,20,QSizePolicy::Ignored,QSizePolicy::Ignored);
}

void MainWindow::generarObjetivos(const int distanciaCentro=5)
{
    listaObjetivos.clear();
    int cantidadObjetivos=ui->spinBoxCantidadObjetivos->value();

    if(ui->checkBoxObjetivosAleatorios->isChecked()){
        int cantidadintentos=0;
        while(listaObjetivos.size()<cantidadObjetivos && cantidadintentos<10000){
           //para los Random
            const int signox=qrand()%2==1 ? 1: -1;
            const int signoy=qrand()%2==1 ? 1: -1;

            const int randomx=(qrand()%(radios.RadioExterior-radios.RadioObjetivo))*signox;
            const int randomy=(qrand()%(radios.RadioExterior-radios.RadioObjetivo))*signoy;
            const double ecuacionCircExt=qPow(randomx,2)+qPow(randomy,2);

            if(ecuacionCircExt<=qPow(double(radios.RadioExterior-radios.RadioObjetivo),2)){//Si es que no se sale del radio exterior
                bool noIntersectaOtros=true;
                foreach (QCPItemEllipse *P, listaObjetivos){//Se analiza si el candidato a agregar no intersecta con otros ya agregados
                    const double perteneceCirc=qSqrt(qPow((randomx - (P->topLeft->coords().x()+radios.RadioObjetivo)),2)+qPow((randomy - (P->topLeft->coords().y()-radios.RadioObjetivo)),2));
                    //QTextStream(stdout)<<"x:"<<P->center->toQCPItemPosition()->coords().x()<<" y:"<<P->center->toQCPItemPosition()->coords().y()<<endl;
                    if( perteneceCirc < 2*radios.RadioObjetivo + 0.5)
                        noIntersectaOtros=false;
                }
                if(noIntersectaOtros){
                    QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);

                    objetivo->topLeft->setCoords(randomx-radios.RadioObjetivo,randomy+radios.RadioObjetivo);
                    objetivo->bottomRight->setCoords(randomx+radios.RadioObjetivo,randomy-radios.RadioObjetivo);
                    objetivo->setBrush(QBrush(Qt::red));
                    listaObjetivos.append(objetivo);
                    cantidadintentos=0;
                }
                else
                    ++cantidadintentos;
            }
        }
        QTextStream(stdout)<<"Objetivos Puestos"<<listaObjetivos.size()<<endl;
    }
    else{
        for (int var = 0; var < cantidadObjetivos; ++var){
            QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
            const double angulo=2*var*((M_PI)/cantidadObjetivos); //
            objetivo->topLeft->setCoords(qCos(angulo)*distanciaCentro-radios.RadioObjetivo,qSin(angulo)*distanciaCentro+radios.RadioObjetivo);
            objetivo->bottomRight->setCoords(qCos(angulo)*distanciaCentro+radios.RadioObjetivo,qSin(angulo)*distanciaCentro-radios.RadioObjetivo);
            objetivo->setBrush(QBrush(Qt::red));
            listaObjetivos.append(objetivo);
        }
    }
}

void MainWindow::slotGraficarTiempoReal(Angulo *angulo)
{
    for (int var = 0; var < listaObjetivos.size(); ++var){ //Se recorre la lista de Objetivos y verifica si se pasa por algun objetivo.
        QCPItemEllipse *P=listaObjetivos.at(var);
        if(P->brush()==QBrush(Qt::red)){ //Si aun sigue con el color por defecto.
            const double perteneceCirc=qSqrt(qPow((angulo->getAnguloX() - (P->topLeft->coords().x()+radios.RadioObjetivo)),2)+qPow((angulo->getAnguloY() - (P->topLeft->coords().y()-radios.RadioObjetivo)),2));
            if( perteneceCirc < radios.RadioObjetivo){
                P->setBrush(QBrush(Qt::green));
                listaObjetivos.removeAt(var);
                ui->lcdNumberObjetivosRestantes->display(listaObjetivos.size());
                QTextStream(stdout)<<listaObjetivos.size()<<endl;
            }
        }
    }

    if(ui->checkBoxLimitarGrafico->isChecked())
    {
        const double ecuacionCircExt=qPow(angulo->getAnguloX(),2)+qPow(angulo->getAnguloY(),2);
        if(ecuacionCircExt<=qPow(double(radios.RadioExterior),2)){//Si es que no se sale del radio exterior
            lienzo->addData(angulo->getAnguloX(), angulo->getAnguloY());
            ui->qCustomPlotGrafico->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto.
            ui->qCustomPlotGrafico->graph(0)->addData(angulo->getAnguloX(), angulo->getAnguloY());
            //ui->qCustomPlotGrafico->graph(0)->rescaleValueAxis(true);
        }
        else{
            const double pendiente=angulo->getAnguloY()/angulo->getAnguloX();
            double inclRecta=qAtan(pendiente)*180/M_PI;

            if(angulo->getAnguloX()>0){
                if(angulo->getAnguloY()>0)//Cuadrante 1
                    inclRecta=inclRecta;
                else //Cuadrante 4
                    inclRecta+=360;
            }
            else  //Cuadrante 2 y Cuadrante 3
                inclRecta+=180;

            const double aX=radios.RadioExterior*qCos(qDegreesToRadians(inclRecta));
            const double aY=radios.RadioExterior*qSin(qDegreesToRadians(inclRecta));
            //QTextStream(stdout)<<"Grados Inclinacion Recta: "<<inclRecta<<" Ax:"<<aX<<" aY:"<<aY<<endl;
            lienzo->addData(aX, aY);
            ui->qCustomPlotGrafico->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto.
            ui->qCustomPlotGrafico->graph(0)->addData(aX, aY);

        }
    }
    else{
        lienzo->addData(angulo->getAnguloX(), angulo->getAnguloY());
        ui->qCustomPlotGrafico->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto.
        ui->qCustomPlotGrafico->graph(0)->addData(angulo->getAnguloX(), angulo->getAnguloY());
    }

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
    const int w=ui->qCustomPlotGrafico->width();
    const int h=ui->qCustomPlotGrafico->height();
    const QRect rect=ui->qCustomPlotGrafico->geometry();

    if(w>h)
        ui->qCustomPlotGrafico->setGeometry(rect.x()+((w-h)/2),rect.y(),h,h);
    else
        ui->qCustomPlotGrafico->setGeometry(rect.x(),rect.y()+((h-w)/2),w,w);
}

void MainWindow::iniciarPrueba()
{
    listaMuestras.clear();
    listaAngulos.clear();
    lecturaSerial->abrirPuertoSerial(serial,ajustesSerial->getAjustes(),ajustesSensores->getAjustes());
    cronometro.start();
    radios=ajustesGrafico->getAjustes();
    ui->verticalSliderRangeGraphic->setValue(radios.RadioExterior+5);//Se actualiza el slider
    inicializarGrafico(); //Se limpian los graficos
    radios.RadioObjetivo=radios.RadioObjetivo;
    generarObjetivos();
    ui->lcdNumberCantidadObjetivos->display(listaObjetivos.size());
    ui->lcdNumberObjetivosRestantes->display(listaObjetivos.size());
    desactivarTabs();
    desactivarSpacerEntreBotones();
    ocultarBotones();
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
              lecturaSerial->cerrarPuertoSerial(serial);
          }
    }
}

void MainWindow::on_pushButtonIniciarPrueba_clicked()
{
    iniciarPrueba();
    ui->stackedWidget->setCurrentWidget(ui->widgetTest);
    ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_grafico);
}

void MainWindow::on_pushButtonReiniciarPrueba_clicked()
{
    lecturaSerial->cerrarPuertoSerial(serial);
    iniciarPrueba();
}

void MainWindow::on_pushButtonDetenerPrueba_clicked()
{    
    mostrarResultados();
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
            lecturaSerial->cerrarPuertoSerial(serial);
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

void MainWindow::on_pushButtonPrueba1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
    pruebaNumero=1;
    ui->labelNombrePrueba->setText("Modo Libre");
    QTextStream stdout <<pruebaNumero<<endl;
    ui->progressBarPrueba->hide();
    ui->labelTiempoPrueba->hide();
    ui->spinBoxTiempoPrueba->hide();
}

void MainWindow::on_pushButtonPrueba2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
    pruebaNumero=2;
    ui->labelNombrePrueba->setText("Prueba 2");
    QTextStream stdout <<pruebaNumero<<endl;
    ui->progressBarPrueba->show();
    ui->labelTiempoPrueba->show();
    ui->spinBoxTiempoPrueba->show();
}

void MainWindow::on_pushButtonPrueba3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
    ui->labelNombrePrueba->setText("Prueba 3");
    ui->progressBarPrueba->show();
    ui->labelTiempoPrueba->show();
    ui->spinBoxTiempoPrueba->show();
}

void MainWindow::on_pushButtonPrueba4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
    ui->labelNombrePrueba->setText("Prueba 4");
    ui->progressBarPrueba->show();
    ui->labelTiempoPrueba->show();
    ui->spinBoxTiempoPrueba->show();
}

void MainWindow::on_pushButtonConfPrueba_clicked() //Permite volver a configurar la prueba
{
     ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
}

void MainWindow::on_pushButtonGuardarImagen_clicked()//Guardar la Imagen de los Graficos
{
    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_grafico)
        reportes->guardarImagenGrafico(ui->qCustomPlotGrafico,1000,1000);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_resultados)
        reportes->guardarImagenGrafico(ui->qCustomPlotResultados,1000,1000);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoAngulos)
        reportes->guardarImagenGrafico(ui->qCustomPlotGraficosAngulos,1920,1080);


    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoMuestras)
        reportes->guardarImagenGrafico(ui->qCustomPlotGraficoMuestras,1920,1080);
}

void MainWindow::on_pushButtonGuardarMuestras_clicked()//Guardar en archivo la informacion de muestras o angulos.
{    
    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_grafico)
        reportes->guardarAngulosEnArchivo(listaAngulos);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaAngulos)
        reportes->guardarAngulosEnArchivo(listaAngulos);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoAngulos)
        reportes->guardarAngulosEnArchivo(listaAngulos);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_TablaMuestras)
        reportes->guardarMuestrasEnArchivo(listaMuestras);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoMuestras)
        reportes->guardarMuestrasEnArchivo(listaMuestras);

}

void MainWindow::on_dockWidget_topLevelChanged(bool topLevel)//Si el grafico esta flotando o dejo de flotar.
{
    (void) topLevel;
    relacionAspectodelGrafico();
}

void MainWindow::on_tabWidgetGrafico_Resultados_currentChanged(int index)
{
    if(index==0)
        ui->centralWidget->adjustSize();

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
        ui->labelGuardarImagen->setText("Guardar\nGrafico\nResultados");
        ui->labelGuardarImagen->show();

        ui->pushButtonGuardarMuestras->hide();
        ui->labelGuardarMuestras->hide();

        desactivarSpacerEntreBotones();
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaAngulos)
    {
        ui->pushButtonGuardarImagen->hide();
        ui->labelGuardarImagen->hide();

        ui->pushButtonGuardarMuestras->show();
        ui->labelGuardarMuestras->setText("Guardar\nDatos\nAngulos");
        ui->labelGuardarMuestras->show();

        desactivarSpacerEntreBotones();
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_TablaMuestras)
    {
        ui->pushButtonGuardarImagen->hide();
        ui->labelGuardarImagen->hide();

        ui->pushButtonGuardarMuestras->show();
        ui->labelGuardarMuestras->setText("Guardar\nMuestras");
        ui->labelGuardarMuestras->show();

        desactivarSpacerEntreBotones();
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoAngulos)
    {
        ui->pushButtonGuardarImagen->show();
        ui->labelGuardarImagen->setText("Guardar\nGraficos\nAngulos");
        ui->labelGuardarImagen->show();

        ui->pushButtonGuardarMuestras->show();
        ui->labelGuardarMuestras->setText("Guardar\nDatos\nAngulos");
        ui->labelGuardarMuestras->show();

        activarSpacerEntreBotones();
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoMuestras)
    {
        ui->pushButtonGuardarImagen->show();
        ui->labelGuardarImagen->setText("Guardar\nGraficos\nMuestras");
        ui->labelGuardarImagen->show();

        ui->pushButtonGuardarMuestras->show();
        ui->labelGuardarMuestras->setText("Guardar\nMuestras");
        ui->labelGuardarMuestras->show();

        activarSpacerEntreBotones();
    }

}

