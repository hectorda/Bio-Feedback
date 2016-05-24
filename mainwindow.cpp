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
    ajustesSerial= new AjustesPuertoSerial(this);
    ajustesSensores = new AjustesSensores(this);
    ajustesGrafico = new AjustesGrafico(this);
    lectorSerial = new Serial(this, new QSerialPort(this)); //Se le envia el objeto en el constructor de la clase Serial
    reportes = new Reportes(this,ui->qCustomPlotResultados,ui->qCustomPlotGraficoAngulos,ui->qCustomPlotGraficoDesplazamientos,ui->qCustomPlotGraficoMuestras,ui->tableWidgetAngulos,ui->tableWidgetDesplazamientos,ui->tableWidgetDatosRaw);
    analisisGraficoAngulos = new AnalisisGrafico(this,reportes);
    analisisGraficoMuestras = new AnalisisGrafico(this,reportes);
    analisisGraficoDesplazamientos = new AnalisisGrafico(this,reportes);

    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);

    status = new QLabel;
    ui->statusBar->addWidget(status);
    ui->dockWidget->installEventFilter(this);
    ui->lineEditRut->setValidator(new QIntValidator(0, 999999999) );

    ui->qCustomPlotGrafico->setContextMenuPolicy(Qt::CustomContextMenu);
    titulo = new QCPPlotTitle(ui->qCustomPlotGrafico);
    ui->qCustomPlotGrafico->plotLayout()->insertRow(0);
    ui->qCustomPlotGrafico->plotLayout()->addElement(0, 0,titulo);

    circuloExterior= new QCPItemEllipse(ui->qCustomPlotGrafico);
    circuloInterior= new QCPItemEllipse(ui->qCustomPlotGrafico);

    ui->tableWidgetDatosRaw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetAngulos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetDesplazamientos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->radioButtonHorizontalAbajo->hide();
    ui->radioButtonHorizontalArriba->hide();

    ui->pushButtonVolverPrueba->hide();

    db = new SQL;
    db->consulta();
}

void MainWindow::conexiones()
{
    //Se envia el dato para graficar en tiempo Real
    connect(this,SIGNAL(emitAnguloGraficoTiempoReal(double,double)),this,SLOT(slotGraficarTiempoReal(double,double)));

    //Conjunto de connects para enviar datos a los graficos de Reportes
    connect(this,SIGNAL(emitAnguloReporte(Angulo*)),reportes,SLOT(agregarDatosGraficoAngulos(Angulo*)));
    connect(this,SIGNAL(emitAnguloReporte(Angulo*)),reportes,SLOT(agregarFilaTablaAngulos(Angulo*)));
    connect(this,SIGNAL(emitDesplazamientoReporte(Desplazamiento*)),reportes,SLOT(agregarDatosGraficoDesplazamientos(Desplazamiento*)));
    connect(this,SIGNAL(emitDesplazamientoReporte(Desplazamiento*)),reportes,SLOT(agregarFilaTablaDesplazamientos(Desplazamiento*)));
    connect(this,SIGNAL(emitRawReporte(Raw*)),reportes,SLOT(agregarDatosGraficoMuestras(Raw*)));
    connect(this,SIGNAL(emitRawReporte(Raw*)),reportes,SLOT(agregarFilaTablaMuestras(Raw*)));
    connect(this,SIGNAL(emitGraficarResultados(QList<Angulo*>)),reportes,SLOT(graficarResultados(QList<Angulo*>)));

    connect(ui->verticalSliderRangeGraphic,SIGNAL(valueChanged(int)),this,SLOT(RangeGraphic(int)));
    connect(ui->qCustomPlotGrafico,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(ZoomGraphic(QWheelEvent*)));
    connect(ui->qCustomPlotGrafico, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
    connect(lectorSerial,SIGNAL(datosLeidos(double,double,double,double,double,double)),this,SLOT(obtenerRaw(double,double,double,double,double,double)));
    connect(ui->lineEditRut,SIGNAL(returnPressed()),ui->pushButtonBuscarPaciente,SLOT(click()));

    connect(ui->pushButtonIniciarPrueba,SIGNAL(clicked()),this,SLOT(configurarArduino()));
    connect(ui->pushButtonDetenerPrueba,SIGNAL(clicked()),this,SLOT(mostrarResultados()));

    connect(ui->pushButtonAnalizarGraficoAngulos,SIGNAL(clicked()),analisisGraficoAngulos,SLOT(show()));
    connect(ui->pushButtonAnalizarGraficoDesplazamientos,SIGNAL(clicked()),analisisGraficoDesplazamientos,SLOT(show()));
    connect(ui->pushButtonAnalizarGraficoMuestras,SIGNAL(clicked()),analisisGraficoMuestras,SLOT(show()));

      //Connect de actions
    connect(ui->actionExportar,SIGNAL(triggered()),this,SLOT(exportar()));
    connect(ui->actionImportar,SIGNAL(triggered()),this,SLOT(importar()));

    connect(ui->actionConfigurar_Serial,SIGNAL(triggered()),ajustesSerial,SLOT(exec()));
    connect(ui->actionConfigurar_Sensores,SIGNAL(triggered(bool)),ajustesSensores,SLOT(exec()));
    connect(ui->actionConfigurar_Grafico,SIGNAL(triggered(bool)),ajustesGrafico,SLOT(exec()));
    connect(ui->actionSQL,SIGNAL(triggered(bool)),db,SLOT(show()));
    connect(ui->actionInicio,SIGNAL(triggered()),this,SLOT(regresarInicio()));
    connect(ui->actionSalir,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionQT,SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));
}

void MainWindow::inicializarGrafico()
{
    limpiarGrafico(ui->qCustomPlotGrafico);
    circuloExterior->topLeft->setCoords(-elementosdelGrafico.RadioExterior,elementosdelGrafico.RadioExterior);
    circuloExterior->bottomRight->setCoords(elementosdelGrafico.RadioExterior,-elementosdelGrafico.RadioExterior);

    circuloExterior->setBrush(QBrush(Qt::yellow));

    if (elementosdelGrafico.Unidad.contains("grados"))
        titulo->setText("Grados Antero-Posterior y Medio Lateral");
    else
        titulo->setText("Centimetros Antero-Posterior y Medio Lateral");

    circuloInterior->topLeft->setCoords(-elementosdelGrafico.RadioInterior,elementosdelGrafico.RadioInterior);
    circuloInterior->bottomRight->setCoords(elementosdelGrafico.RadioInterior,-elementosdelGrafico.RadioInterior);

    generarObjetivos();

    ui->qCustomPlotGrafico->addGraph(); // Para el Grafico del punto
    ui->qCustomPlotGrafico->graph(0)->setPen(QPen(Qt::blue));
    ui->qCustomPlotGrafico->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->qCustomPlotGrafico->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

    lienzo = new QCPCurve(ui->qCustomPlotGrafico->xAxis,ui->qCustomPlotGrafico->yAxis);
    lienzo->setPen(QPen(Qt::blue, 1));

    ui->qCustomPlotGrafico->addPlottable(lienzo);

    ui->qCustomPlotGrafico->xAxis2->setVisible(true);
    ui->qCustomPlotGrafico->yAxis2->setVisible(true);

    //Se configuran los rangos maximos para los ejes X e Y segun el slider.
    const int range=ui->verticalSliderRangeGraphic->value();
    ui->qCustomPlotGrafico->xAxis->setRange(-range,range);
    ui->qCustomPlotGrafico->yAxis->setRange(-range,range);
    ui->qCustomPlotGrafico->xAxis2->setRange(-range,range);
    ui->qCustomPlotGrafico->yAxis2->setRange(-range,range);

    ui->qCustomPlotGrafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // Para usar el el Zoom y el Arrastre del grafico.
}

void MainWindow::actualizarMensajeBarraEstado(const QString &message)
{
    status->setText(message);
}

void MainWindow::mostrarResultados()
{
    QTextStream(stdout)<<"Muestras x Seg: "<< listaMuestras.size()/listaMuestras.last()->getTiempo()<<endl;
    lectorSerial->cerrarPuertoSerial();
    emit emitGraficarResultados(listaAngulos);
    analisisGraficoAngulos->setListaAngulos(listaAngulos);
    analisisGraficoDesplazamientos->setListaDesplazamientos(listaDesplazamientos);
    analisisGraficoMuestras->setListaMuestras(listaMuestras);
    mostrarBotonesPrueba();
    activarTabs();
    ui->centralWidget->adjustSize();
}

void MainWindow::obtenerAngulos(Raw *dato)
{
    const double RAD_TO_DEG=180/M_PI;
    double anguloComplementario1=0,anguloComplementario2=0;
    double angulo1,angulo2;
    double alpha=0.02;
    bool filtroComplementario=true;
    QString orientacion=ui->comboBoxOrientacion->currentText().toLower();

    if(filtroComplementario){

        if(orientacion.contains("vertical"))
        {
            if(ui->radioButtonVerticalAtras->isChecked()){
                //Se calculan los angulos con la IMU vertical.
                angulo1 = qAtan(-dato->getAcX()/qSqrt(qPow(dato->getAcZ(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;
                angulo2 = qAtan(dato->getAcZ()/qSqrt(qPow(dato->getAcX(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;

                //Aplicar el Filtro Complementario
                if(listaAngulos.size()>0){
                    Angulo *lastAngulo=listaAngulos.last();
                    const double dt=(dato->getTiempo()-listaAngulos.last()->getTiempo())/ 1000;
                    anguloComplementario1 = (1-alpha) *(lastAngulo->getAnguloX()+dato->getGyZ()*dt) + alpha*angulo1;
                    anguloComplementario2 = (1-alpha) *(lastAngulo->getAnguloY()+dato->getGyX()*dt) + alpha*angulo2;
                }
                else{
                    anguloComplementario1=angulo1;
                    anguloComplementario2=angulo2;
                }
            }
            if(ui->radioButtonVerticalFrente->isChecked()){

                //Se calculan los angulos con la IMU vertical.
                angulo1 = qAtan(dato->getAcX()/qSqrt(qPow(dato->getAcZ(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;
                angulo2 = qAtan(-dato->getAcZ()/qSqrt(qPow(dato->getAcX(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;

                //Aplicar el Filtro Complementario
                if(listaAngulos.size()>0){
                    Angulo *lastAngulo=listaAngulos.last();
                    const double dt=(dato->getTiempo()-listaAngulos.last()->getTiempo())/ 1000;
                    anguloComplementario1 = (1-alpha) *(lastAngulo->getAnguloX()+dato->getGyZ()*dt) + alpha*angulo1;
                    anguloComplementario2 = (1-alpha) *(lastAngulo->getAnguloY()+dato->getGyX()*dt) + alpha*angulo2;
                }
                else{
                    anguloComplementario1=angulo1;
                    anguloComplementario2=angulo2;
                }
            }
            if(ui->radioButtonVerticalDerecha->isChecked()){

                //Se calculan los angulos con la IMU vertical.
                angulo1 = qAtan(-dato->getAcZ()/qSqrt(qPow(dato->getAcX(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;
                angulo2 = qAtan(-dato->getAcX()/qSqrt(qPow(dato->getAcZ(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;

                //Aplicar el Filtro Complementario
                if(listaAngulos.size()>0){
                    Angulo *lastAngulo=listaAngulos.last();
                    const double dt=(dato->getTiempo()-listaAngulos.last()->getTiempo())/ 1000;
                    anguloComplementario1 = (1-alpha) *(lastAngulo->getAnguloX()+dato->getGyZ()*dt) + alpha*angulo1;
                    anguloComplementario2 = (1-alpha) *(lastAngulo->getAnguloY()+dato->getGyX()*dt) + alpha*angulo2;
                }
                else{
                    anguloComplementario1=angulo1;
                    anguloComplementario2=angulo2;
                }
            }
            if(ui->radioButtonVerticalIzquierda->isChecked()){

                //Se calculan los angulos con la IMU vertical.
                angulo1 = qAtan(dato->getAcZ()/qSqrt(qPow(dato->getAcX(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;
                angulo2 = qAtan(dato->getAcX()/qSqrt(qPow(dato->getAcZ(),2) + qPow(dato->getAcY(),2)))*RAD_TO_DEG;

                //Aplicar el Filtro Complementario
                if(listaAngulos.size()>0){
                    Angulo *lastAngulo=listaAngulos.last();
                    const double dt=(dato->getTiempo()-listaAngulos.last()->getTiempo())/ 1000;
                    anguloComplementario1 = (1-alpha) *(lastAngulo->getAnguloX()+dato->getGyZ()*dt) + alpha*angulo1;
                    anguloComplementario2 = (1-alpha) *(lastAngulo->getAnguloY()+dato->getGyX()*dt) + alpha*angulo2;
                }
                else{
                    anguloComplementario1=angulo1;
                    anguloComplementario2=angulo2;
                }
            }
        }
        else
        {
            if(ui->radioButtonHorizontalArriba->isChecked())
            {
                //Se calculan los angulos con la IMU horizontal.
                angulo1 = qAtan(dato->getAcY()/qSqrt(qPow(dato->getAcX(),2) + qPow(dato->getAcZ(),2)))*RAD_TO_DEG;
                angulo2 = qAtan(-dato->getAcX()/qSqrt(qPow(dato->getAcY(),2) + qPow(dato->getAcZ(),2)))*RAD_TO_DEG;

                //Aplicar el Filtro Complementario
                if(listaAngulos.size()>0){
                    Angulo *lastAngulo=listaAngulos.last();
                    const double dt=(dato->getTiempo()-listaAngulos.last()->getTiempo())/ 1000;
                    anguloComplementario1 = (1-alpha) *(lastAngulo->getAnguloX()+dato->getGyX()*dt) + alpha*angulo1;
                    anguloComplementario2 = (1-alpha) *(lastAngulo->getAnguloY()+dato->getGyY()*dt) + alpha*angulo2;
                }
                else{
                    anguloComplementario1=angulo1;
                    anguloComplementario2=angulo2;
                }
            }
            if(ui->radioButtonHorizontalAbajo->isChecked())
            {
                //Se calculan los angulos con la IMU horizontal.
                angulo1 = qAtan(dato->getAcY()/qSqrt(qPow(dato->getAcX(),2) + qPow(dato->getAcZ(),2)))*RAD_TO_DEG;
                angulo2 = qAtan(dato->getAcX()/qSqrt(qPow(dato->getAcY(),2) + qPow(dato->getAcZ(),2)))*RAD_TO_DEG;

                //Aplicar el Filtro Complementario
                if(listaAngulos.size()>0){
                    Angulo *lastAngulo=listaAngulos.last();
                    const double dt=(dato->getTiempo()-listaAngulos.last()->getTiempo())/ 1000;
                    anguloComplementario1 = (1-alpha) *(lastAngulo->getAnguloX()+dato->getGyX()*dt) + alpha*angulo1;
                    anguloComplementario2 = (1-alpha) *(lastAngulo->getAnguloY()+dato->getGyY()*dt) + alpha*angulo2;
                }
                else{
                    anguloComplementario1=angulo1;
                    anguloComplementario2=angulo2;
                }
            }
        }

        Angulo *angulo=new Angulo(dato->getTiempo(),anguloComplementario1,anguloComplementario2);
        obtenerDesplazamiento(angulo);
        listaAngulos.append(angulo);

        emit emitAnguloReporte(angulo);

        if(listaMuestras.size()%divisorFPS==0 && elementosdelGrafico.Unidad.contains("grados"))//Mod
            emit emitAnguloGraficoTiempoReal(angulo->getAnguloX(),angulo->getAnguloY());
    }
}

void MainWindow::obtenerDesplazamiento(Angulo *angulo)
{
    const double alturadisp=ui->doubleSpinBoxAlturaDispositivo->value();
    const double despX=qSin(qDegreesToRadians(angulo->getAnguloX()));
    const double despY=qSin(qDegreesToRadians(angulo->getAnguloY()));
    Desplazamiento *desplazamiento=new Desplazamiento(angulo->getTiempo(),despX*alturadisp,despY*alturadisp);
    emit emitDesplazamientoReporte(desplazamiento);
    listaDesplazamientos.append(desplazamiento);
    if(listaMuestras.size()%divisorFPS==0 && elementosdelGrafico.Unidad.contains("centimetros"))//Mod
        emit emitAnguloGraficoTiempoReal(desplazamiento->getDesplazamientoX(),desplazamiento->getDesplazamientoY());
}

void MainWindow::mostrarBotonesPrueba()
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
    ui->pushButtonAnalizarGraficoAngulos->show();
    activarSpacerEntreBotones();
}

void MainWindow::ocultarBotonesPrueba()
{
    ui->pushButtonDetenerPrueba->show();
    ui->pushButtonConfPrueba->hide();
    ui->pushButtonGuardarImagen->hide();
    ui->pushButtonGuardarMuestras->hide();
    ui->labelGuardarImagen->hide();
    ui->labelGuardarMuestras->hide();
    ui->pushButtonAnalizarGraficoAngulos->hide();
}

void MainWindow::desactivarTabs()
{
    for (int var = 1; var < ui->tabWidgetGrafico_Resultados->count(); ++var)
        ui->tabWidgetGrafico_Resultados->setTabEnabled(var,false);
}

void MainWindow::activarTabs()
{
    for (int var = 1; var < ui->tabWidgetGrafico_Resultados->count(); ++var)
        ui->tabWidgetGrafico_Resultados->setTabEnabled(var,true);
}

void MainWindow::activarSpacerEntreBotones()
{
    ui->verticalSpacerEntreBotones->changeSize(40,20,QSizePolicy::Ignored,QSizePolicy::Expanding);
}

void MainWindow::desactivarSpacerEntreBotones()
{
    ui->verticalSpacerEntreBotones->changeSize(40,20,QSizePolicy::Ignored,QSizePolicy::Ignored);
}

void MainWindow::generarObjetivos()
{
    if(pruebaNumero==3||pruebaNumero==4)
    {
        const int distanciaCentro=elementosdelGrafico.RadioInterior;
        const int cantidadObjetivos=pruebaNumero==3? 8: 4;
        for (int var = 0; var < cantidadObjetivos; ++var){
            QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
            ui->qCustomPlotGrafico->addItem(objetivo);
            const double angulo=2*var*((M_PI)/cantidadObjetivos); //
            objetivo->topLeft->setCoords(qCos(angulo)*distanciaCentro-elementosdelGrafico.RadioObjetivo,qSin(angulo)*distanciaCentro+elementosdelGrafico.RadioObjetivo);
            objetivo->bottomRight->setCoords(qCos(angulo)*distanciaCentro+elementosdelGrafico.RadioObjetivo,qSin(angulo)*distanciaCentro-elementosdelGrafico.RadioObjetivo);
            objetivo->setBrush(QBrush(Qt::red));
            listaObjetivos.append(objetivo);
        }
        QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
        ui->qCustomPlotGrafico->addItem(objetivo);
        objetivo->topLeft->setCoords(-elementosdelGrafico.RadioObjetivo,elementosdelGrafico.RadioObjetivo);
        objetivo->bottomRight->setCoords(elementosdelGrafico.RadioObjetivo,-elementosdelGrafico.RadioObjetivo);
        objetivo->setBrush(QBrush(Qt::red));
        listaObjetivos.append(objetivo);
    }
    if(pruebaNumero==1 || pruebaNumero==2)
    {
        int cantidadObjetivos=ui->spinBoxCantidadObjetivos->value();

        if(ui->checkBoxObjetivosAleatorios->isChecked()){
            int cantidadintentos=0;
            while(listaObjetivos.size()<cantidadObjetivos && cantidadintentos<10000){
               //para los Random
                const int signox=qrand()%2==1 ? 1: -1;
                const int signoy=qrand()%2==1 ? 1: -1;

                const int randomx=(qrand()%(elementosdelGrafico.RadioExterior-elementosdelGrafico.RadioObjetivo))*signox;
                const int randomy=(qrand()%(elementosdelGrafico.RadioExterior-elementosdelGrafico.RadioObjetivo))*signoy;
                const double ecuacionCircExt=qPow(randomx,2)+qPow(randomy,2);

                if(ecuacionCircExt<=qPow(double(elementosdelGrafico.RadioExterior-elementosdelGrafico.RadioObjetivo),2)){//Si es que no se sale del radio exterior
                    bool noIntersectaOtros=true;
                    foreach (QCPItemEllipse *P, listaObjetivos){//Se analiza si el candidato a agregar no intersecta con otros ya agregados
                        const double perteneceCirc=qSqrt(qPow((randomx - (P->topLeft->coords().x()+elementosdelGrafico.RadioObjetivo)),2)+qPow((randomy - (P->topLeft->coords().y()-elementosdelGrafico.RadioObjetivo)),2));
                        //QTextStream(stdout)<<"x:"<<P->center->toQCPItemPosition()->coords().x()<<" y:"<<P->center->toQCPItemPosition()->coords().y()<<endl;
                        if( perteneceCirc < 2*elementosdelGrafico.RadioObjetivo + 0.5)
                            noIntersectaOtros=false;
                    }
                    if(noIntersectaOtros){
                        QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
                        ui->qCustomPlotGrafico->addItem(objetivo);

                        objetivo->topLeft->setCoords(randomx-elementosdelGrafico.RadioObjetivo,randomy+elementosdelGrafico.RadioObjetivo);
                        objetivo->bottomRight->setCoords(randomx+elementosdelGrafico.RadioObjetivo,randomy-elementosdelGrafico.RadioObjetivo);
                        objetivo->setBrush(QBrush(elementosdelGrafico.colorObjetivoSinMarcar));
                        listaObjetivos.append(objetivo);

                        cantidadintentos=0;
                    }
                    else
                        ++cantidadintentos;
                }
            }
            QTextStream(stdout)<<"Objetivos Puestos"<<listaObjetivos.size()<<endl;
        }
        else
        {
            const int distanciaCentro=elementosdelGrafico.RadioInterior;
            for (int var = 0; var < cantidadObjetivos; ++var){
                QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
                ui->qCustomPlotGrafico->addItem(objetivo);
                const double angulo=2*var*((M_PI)/cantidadObjetivos);
                objetivo->topLeft->setCoords(qCos(angulo)*distanciaCentro-elementosdelGrafico.RadioObjetivo,qSin(angulo)*distanciaCentro+elementosdelGrafico.RadioObjetivo);
                objetivo->bottomRight->setCoords(qCos(angulo)*distanciaCentro+elementosdelGrafico.RadioObjetivo,qSin(angulo)*distanciaCentro-elementosdelGrafico.RadioObjetivo);
                objetivo->setBrush(QBrush(Qt::red));
                listaObjetivos.append(objetivo);
            }
        }
    }
}

QString MainWindow::obtenerOrientacionSensor()
{
    QString cBorientacion = ui->comboBoxOrientacion->currentText().toLower();
    QString orientacion;
    if(cBorientacion.contains("vertical"))
    {
        orientacion="Vertical ";
        if(ui->radioButtonVerticalAtras->isChecked())
            orientacion+="Atras";
        if(ui->radioButtonVerticalFrente->isChecked())
            orientacion+="Frente";
        if(ui->radioButtonVerticalDerecha->isChecked())
            orientacion+="Derecha";
        if(ui->radioButtonVerticalIzquierda->isChecked())
            orientacion+="Izquierda";
    }
    else
    {
        orientacion="Horizontal ";
        if(ui->radioButtonHorizontalArriba->isChecked())
            orientacion+="Arribla";
        if(ui->radioButtonHorizontalAbajo->isChecked())
            orientacion+="Abajo";
    }
    return orientacion;
}

/*
*Se analiza segun el tipo de prueba si el objetivo es marcable, y cual es que actualmente esta parpadenado
*Si se selecciona el modo aleatorios ninguno parpadea.
*/
void MainWindow::marcarObjetivos(const double x,const double y)
{
    if(pruebaNumero== 3 || pruebaNumero== 4)
    {
        if(!listaObjetivos.isEmpty())
        {
            static bool centro=true;
            if(centro)
            {

                QCPItemEllipse *P=listaObjetivos.last();
                parpadeoCirculo(P);

                if(PertenecePuntoAlObjetivo(x,y,P))
                    centro=false;
            }
            else{

                QCPItemEllipse *P=listaObjetivos.at(0);
                parpadeoCirculo(P);

                if(PertenecePuntoAlObjetivo(x,y,P)){
                    listaObjetivos.removeAt(0);
                    ui->lcdNumberObjetivosRestantes->display(listaObjetivos.size());
                    QTextStream(stdout)<<listaObjetivos.size()<<endl;
                    centro=true;
                }
            }
        }
    }
    if(pruebaNumero==1 || pruebaNumero==2)
    {
        if(!ui->checkBoxOrdenObjetivos->isChecked())//Si es que se puede ir a cualquier objetivo
        {
            for (int var = 0; var < listaObjetivos.size(); ++var)//Se recorre la lista de Objetivos y verifica si se pasa por algun objetivo.
            {
                QCPItemEllipse *P=listaObjetivos.at(var);
                if(P->brush()==QBrush(elementosdelGrafico.colorObjetivoSinMarcar)) //Si aun sigue con el color por defecto.
                {
                    if(PertenecePuntoAlObjetivo(x,y,P)){
                        listaObjetivos.removeAt(var);
                        ui->lcdNumberObjetivosRestantes->display(listaObjetivos.size());
                    }
                }
            }
        }
        else
        {
           if(!listaObjetivos.isEmpty())
           {
               QCPItemEllipse *P=listaObjetivos.at(0);
               parpadeoCirculo(P);

               if( PertenecePuntoAlObjetivo(x,y,P)){
                   listaObjetivos.removeAt(0);
                   ui->lcdNumberObjetivosRestantes->display(listaObjetivos.size());
               }
           }
        }
    }

    if(listaObjetivos.isEmpty() && ui->checkBoxDeteneralMarcarObjetivos->isChecked())
        ui->pushButtonDetenerPrueba->click();
}

/*
*Se verifica si el punto dado pertenece a un circulo usando geometria
*En caso pertenecer cambia el color de esta y ademas retorna True
*En caso contrario retorna False.
*/
bool MainWindow::PertenecePuntoAlObjetivo(const double x,const double y,QCPItemEllipse *P){
    const double perteneceCirc=qSqrt(qPow(( x - (P->topLeft->coords().x()+elementosdelGrafico.RadioObjetivo)),2)+qPow(( y - (P->topLeft->coords().y()-elementosdelGrafico.RadioObjetivo)),2));
    if( perteneceCirc < elementosdelGrafico.RadioObjetivo){
        P->setBrush(QBrush(elementosdelGrafico.colorObjetivoMarcado));
        return true;
    }
    return false;
}

/*
*Dado un QCPItemEllipse se calcula el tiempo y se hace parpadear
*repintando la elipse entre dos colores
*segun el modulo del tiempo transcurrido
*/
void MainWindow::parpadeoCirculo(QCPItemEllipse *P)
{
    if(cronometro.elapsed()/1000%2==0)
         P->setBrush(QBrush(Qt::white));
    else
        P->setBrush(QBrush(elementosdelGrafico.colorObjetivoSinMarcar));

}

void MainWindow::exportar()
{
    if(listaAngulos.isEmpty() || listaDesplazamientos.isEmpty())
        QMessageBox::critical(this,"Aun no se realiza prueba","Se debe realizar una prueba antes de Exportar.");
    else{
        QString selectedFilter;
        QString filters(tr("Formato BioFeed-Back (*.bioh)"));
        QString fileName = QFileDialog::getSaveFileName(0, tr("Guardar el Archivo"),"",filters,&selectedFilter);

        if (fileName != ""){
            QFile file(fileName);
            file.remove();
            if (file.open(QIODevice::Append)){
                QTextStream stream(&file);
                const int muestras=listaAngulos.size();
                const double tiempo=listaAngulos.last()->getTiempo();
                const double alturaSensor=ui->doubleSpinBoxAlturaDispositivo->value();
                stream <<"PruebaNumero: "<<pruebaNumero<<endl;
                stream <<"OrientacionSensor: "<<obtenerOrientacionSensor()<<endl;
                stream <<"AlturaPuestaSensor: "<<alturaSensor<<endl;
                stream <<"TiempoPrueba: "<<tiempo<<endl;
                stream <<"MuestrasObtenidas: "<<muestras<<endl;
                for (int var = 0; var < muestras; ++var){
                    Angulo *ang=listaAngulos.at(var);
                    Desplazamiento *desp=listaDesplazamientos.at(var);
                    Raw *raw=listaMuestras.at(var);
                    stream << ang->getTiempo()<<" "<<ang->getAnguloX()<<" "<<ang->getAnguloY()<<" "<<desp->getDesplazamientoX()<<" "<<desp->getDesplazamientoY()<<" "<<
                              raw->getAcX()<<" "<<raw->getAcY()<<" "<<raw->getAcZ()<<" "<<raw->getAcZ()<<" "<<raw->getGyX()<<" "<<raw->getGyY()<<" "<<raw->getGyZ()<<endl;
                }
                file.flush();
                file.close();
            }
            else {
                QMessageBox::critical(0, tr("Error"), tr("No se pudo guardar el archivo"));
                return;
            }
        }
    }
}

void MainWindow::importar(){
    QString selectedFilter;
    QString filters(tr("Formato BioFeed-Back (*.bioh)"));
    QString fileName = QFileDialog::getOpenFileName(0, tr("Guardar el Archivo"),"",filters,&selectedFilter);
    if (fileName != ""){
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)){
            QTextStream stream(&file);
            const int pNumero=stream.readLine().split(" ").last().toInt();
            const QStringList lineaOrientacion=stream.readLine().split(" ");
            const QString orientacion=lineaOrientacion.at(1)+lineaOrientacion.at(2);
            const QStringList lineaAlturaSensor=stream.readLine().split(" ");
            const double alturaSensor=lineaAlturaSensor.last().toDouble();
            const double tiempoPrueba=stream.readLine().split(" ").last().toDouble();
            const int muestras=stream.readLine().split(" ").last().toInt();
            listaMuestras.clear();
            listaAngulos.clear();
            listaDesplazamientos.clear();
            for (int var = 0; var < muestras; ++var){
                QList<double> datos;
                foreach (QString var, stream.readLine().split(" ")) {
                    datos.append(var.toDouble());
                }
                Angulo *ang=new Angulo(datos.at(0),datos.at(1),datos.at(2));
                Desplazamiento *desp=new Desplazamiento(datos.at(0),datos.at(3),datos.at(4));
                Raw *raw=new Raw(datos.at(0),datos.at(5),datos.at(6),datos.at(7),datos.at(8),datos.at(9),datos.at(10));
                emit emitAnguloReporte(ang);
                emit emitDesplazamientoReporte(desp);
                emit emitRawReporte(raw);
                listaAngulos.append(ang);
                listaDesplazamientos.append(desp);
                listaMuestras.append(raw);
            }
            file.flush();
            file.close();
            ui->stackedWidget->setCurrentWidget(ui->widgetTest);
            mostrarResultados();
        }
        else {
            QMessageBox::critical(0, tr("Error"), tr("No se pudo abir el archivo"));
            return;
        }
    }
}

void MainWindow::slotGraficarTiempoReal(const double x,const double y)
{
    marcarObjetivos(x,y); // Se hace parpadear o verifica el objetivo segun las configuraciones de la prueba

    if(ui->checkBoxLimitarGrafico->isChecked())
    {
        const double ecuacionCircExt=qPow(x,2)+qPow(y,2);
        if(ecuacionCircExt<=qPow(double(elementosdelGrafico.RadioExterior),2)){//Si es que no se sale del radio exterior
            lienzo->addData(x,y);
            ui->qCustomPlotGrafico->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto.
            ui->qCustomPlotGrafico->graph(0)->addData(x,y);
        }
        else
        {
            const double pendiente=y/x;
            double inclRecta=qAtan(pendiente)*180/M_PI;

            if(x>0)
            {
                if(y>0)//Cuadrante 1
                    inclRecta=inclRecta;
                else //Cuadrante 4
                    inclRecta+=360;
            }
            else  //Cuadrante 2 y Cuadrante 3
                inclRecta+=180;

            const double aX=elementosdelGrafico.RadioExterior*qCos(qDegreesToRadians(inclRecta));
            const double aY=elementosdelGrafico.RadioExterior*qSin(qDegreesToRadians(inclRecta));
            //QTextStream(stdout)<<"Grados Inclinacion Recta: "<<inclRecta<<" Ax:"<<aX<<" aY:"<<aY<<endl;
            lienzo->addData(aX,aY);
            ui->qCustomPlotGrafico->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto.
            ui->qCustomPlotGrafico->graph(0)->addData(aX,aY);
        }
    }
    else
    {
        lienzo->addData(x , y);
        ui->qCustomPlotGrafico->graph(0)->clearData(); //Se limpian los datos anteriores, para solo mantener el ultimo punto.
        ui->qCustomPlotGrafico->graph(0)->addData(x,y);
    }
    ui->qCustomPlotGrafico->replot(); //Se redibuja el grafico
}

void MainWindow::RangeGraphic(int Range)
{
    ui->qCustomPlotGrafico->xAxis->setRange(-Range,Range);
    ui->qCustomPlotGrafico->yAxis->setRange(-Range,Range);
    ui->qCustomPlotGrafico->xAxis2->setRange(-Range,Range);
    ui->qCustomPlotGrafico->yAxis2->setRange(-Range,Range);
    ui->qCustomPlotGrafico->replot();
}

void MainWindow::ZoomGraphic(QWheelEvent *event)
{
    (void) event;//Variable no usada
    QCPRange range=ui->qCustomPlotGrafico->xAxis->range();

    ui->qCustomPlotGrafico->xAxis->setRange(range);
    ui->qCustomPlotGrafico->yAxis->setRange(range);
    ui->qCustomPlotGrafico->xAxis2->setRange(range);
    ui->qCustomPlotGrafico->yAxis2->setRange(range);

    ui->verticalSliderRangeGraphic->setValue(range.upper);
    ui->qCustomPlotGrafico->replot();
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

void MainWindow::configurarArduino()
{
    if(lectorSerial->abrirPuertoSerial(ajustesSerial->getAjustes()))//Se abre el puerto serial con sus ajustes respectivos
    {
        QTextStream stdout<<"Cadena Ajustes:"<<ajustesSensores->getAjustesSensores()<<endl;
        frecuenciaMuestreo=ajustesSensores->obtenerFrecuenciaMuestreo();

        //Qdialog de ventana de carga configuracion sensores.
        QDialog *QdialogCarga=new QDialog(this,Qt::CustomizeWindowHint|Qt::WindowTitleHint);
        QHBoxLayout* layoutBarraCarga = new QHBoxLayout;
        QLabel *labelCarga= new QLabel(tr("Actualizando configuracion de sensores\nPuerto: %1\nFrecuencia Muestreo: %2 Hz").arg(ajustesSerial->getAjustes().portName).arg(frecuenciaMuestreo));
        layoutBarraCarga->addWidget(labelCarga);
        QLabel *labelQMovie= new QLabel;
        layoutBarraCarga->addWidget(labelQMovie);
        QMovie *movie = new QMovie(":/images/Loading.gif");
        movie->setScaledSize(QSize(50,50));
        labelQMovie->setMovie(movie);
        movie->start();
        QdialogCarga->setLayout(layoutBarraCarga);

        QTimer *timer=new QTimer(this); //Se crea un timer para enviar las configuraciones de los sensores
        timer->setSingleShot(true);

        connect(timer, QTimer::timeout, [=]() { lectorSerial->escribirDatosSerial(ajustesSensores->getAjustesSensores()); });
        connect(timer, QTimer::timeout, [=]() { iniciarPrueba(); });
        connect(timer, QTimer::timeout, [=]() { QdialogCarga->close();});
        connect(timer, QTimer::timeout, [=]() { timer->stop();});
        connect(timer, QTimer::timeout, [=]() { delete timer; delete QdialogCarga; delete movie;});

        timer->start(2500); //Se fija el tiempo de accion en 2.5 seg
        QdialogCarga->exec();
    }
    else
        QMessageBox::warning(this,"Error al conectar","Error Abriendo el Puerto Serial",QMessageBox::Ok);
}

void MainWindow::limpiarListasyOcultarBotones()
{
    //Limpieza de listas y de los elementos de la interfaz
    listaMuestras.clear();
    listaAngulos.clear();
    listaDesplazamientos.clear();
    listaObjetivos.clear();
    reportes->vaciarTablas();
    reportes->vaciarGraficos();
    analisisGraficoAngulos->hide();
    analisisGraficoDesplazamientos->hide();
    analisisGraficoMuestras->hide();
}

void MainWindow::iniciarPrueba()
{
    limpiarListasyOcultarBotones();

    elementosdelGrafico=ajustesGrafico->getAjustes();//Se obtienen los ajustes actuales para el grafico

    if(frecuenciaMuestreo>elementosdelGrafico.FPS)//Si la frecuencia de muestreo es menor a 275 se calculan el divisor en base a la frecuencia elegida sino en 275.
        divisorFPS=frecuenciaMuestreo<275 ? (int)(frecuenciaMuestreo/elementosdelGrafico.FPS):
                                           (int)(275/elementosdelGrafico.FPS);
    else//Si la frecuencia de muestreo  es menor a los fps se grafica en base a la frecuencia
        divisorFPS=1;
    ui->verticalSliderRangeGraphic->setValue(elementosdelGrafico.RadioExterior+5);//Se actualiza el slider del Rango
    inicializarGrafico(); //Se limpian y Reajustan los graficos
    elementosdelGrafico.RadioObjetivo=elementosdelGrafico.RadioObjetivo;

    ui->lcdNumberCantidadObjetivos->display(listaObjetivos.size());
    ui->lcdNumberObjetivosRestantes->display(listaObjetivos.size());

    if (ui->checkBoxTiempoInfinito->isChecked())
        ui->progressBarPrueba->hide();

    ui->pushButtonVolverPrueba->show();
    ui->stackedWidget->setCurrentWidget(ui->widgetTest);
    ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_grafico);
    ui->centralWidget->adjustSize();
    desactivarTabs();
    desactivarSpacerEntreBotones();
    ocultarBotonesPrueba();
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
          lectorSerial->cerrarPuertoSerial();
        }
    }
}

void MainWindow::obtenerRaw(const double AcX, const double AcY, const double AcZ, const double GyX, const double GyY, const double GyZ)
{
    if(listaMuestras.size()==0)//Cuando se agrega el primer dato, se inicia el tiempo.
       cronometro.start(); //Para revalidar que la velocidad esta pasando bien :)

    const double a1=frecuenciaMuestreo<275 ? listaMuestras.size()*(1/frecuenciaMuestreo) : cronometro.elapsed()/1000.0;
    const double tiempoPrueba=ui->checkBoxTiempoInfinito->isChecked() ? qInf() :ui->spinBoxTiempoPrueba->value(); //Se coloca un tiempo infinito o el elegido
    if ( a1 < tiempoPrueba)
    {
        const double tiempo=frecuenciaMuestreo<275 ? (1/frecuenciaMuestreo)*listaMuestras.size(): cronometro.elapsed()/1000.0;
        Raw *dato=new Raw(tiempo,AcX,AcY,AcZ,GyX,GyY,GyZ);
        obtenerAngulos(dato);
        listaMuestras.append(dato);
        emit emitRawReporte(dato);

        if(tiempoPrueba!=qInf())//Si el tiempo es distinto de infinito se calcula el porcentaje
        {
           const int porcentaje=qRound((tiempo/tiempoPrueba)*100.0);
           ui->progressBarPrueba->setValue(porcentaje);
        }

        const QString lapso=QString::number(tiempo, 'f', 1);
        const int elapased=cronometro.elapsed()/1000;

        QString horas = QString::number(qFloor(elapased / 3600));
        QString minutos = QString::number(qFloor ((elapased % 3600) / 60 ));
        QString segundos = QString::number(elapased % 60);

        minutos = minutos.toInt() < 10 ? '0' + minutos : minutos;//Anteponiendo un 0 a los minutos si son menos de 10
        segundos = segundos.toInt() < 10 ? '0' + segundos : segundos;//Anteponiendo un 0 a los segundos si son menos de 10
        QString result = horas + ":" + minutos + ":" + segundos;  // hh:mm::ss

        ui->lcdNumberTiempoTranscurrido->display(result);

        const QString mensaje="Tiempo: "+ lapso + " Muestras:" + QString::number(listaMuestras.size())+ " AcX: "+QString::number(dato->getAcX(),'f',3)+" AcY: "+QString::number(dato->getAcY(),'f',3)+" AcZ: "+QString::number(dato->getAcZ(),'f',3)
                          + " GyX: "+QString::number(dato->getGyX(),'f',3)+" GyY: "+QString::number(dato->getGyY(),'f',3)+" GyZ: "+QString::number(dato->getGyZ(),'f',3);
        actualizarMensajeBarraEstado(mensaje);
    }
    else
    {
        cronometro.invalidate();
        mostrarResultados();
    }
}

void MainWindow::on_pushButtonReiniciarPrueba_clicked()
{
    lectorSerial->cerrarPuertoSerial();
    limpiarGrafico(ui->qCustomPlotGrafico);
    ui->qCustomPlotGrafico->replot();
    configurarArduino();
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
            lectorSerial->cerrarPuertoSerial();
        }
    }
}

void MainWindow::on_pushButtonVolverInicio_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);
}

void MainWindow::on_pushButtonVolverPrueba_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetTest);
}

void MainWindow::limpiarGrafico(QCustomPlot *grafico){

    if(grafico->plotLayout()->hasElement(0,1))
        grafico->plotLayout()->remove(grafico->plotLayout()->element(0,1));

    grafico->legend->clear();
    grafico->legend->setVisible(false);
    grafico->clearGraphs();
    grafico->clearPlottables();
    grafico->clearItems();
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

    ui->labelObjetivosAleatorios->show();
    ui->checkBoxObjetivosAleatorios->show();

    ui->labelCantidadObjetivos->show();
    ui->spinBoxCantidadObjetivos->show();

    ui->progressBarPrueba->show();
    ui->labelTiempoPrueba->show();
    ui->spinBoxTiempoPrueba->show();
}

void MainWindow::on_pushButtonPrueba2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
    pruebaNumero=2;
    ui->labelNombrePrueba->setText("Prueba 2");
    QTextStream stdout <<pruebaNumero<<endl;

    ui->labelCantidadObjetivos->show();
    ui->spinBoxCantidadObjetivos->show();

    ui->labelCantidadObjetivos->show();
    ui->spinBoxCantidadObjetivos->show();

    ui->progressBarPrueba->show();
    ui->labelTiempoPrueba->show();
    ui->spinBoxTiempoPrueba->show();
}

void MainWindow::on_pushButtonPrueba3_clicked()
{
    pruebaNumero=3;
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
    ui->labelNombrePrueba->setText("Prueba 3");

    ui->labelCantidadObjetivos->hide();
    ui->spinBoxCantidadObjetivos->hide();

    ui->labelCantidadObjetivos->hide();
    ui->spinBoxCantidadObjetivos->hide();

    ui->progressBarPrueba->show();
    ui->labelTiempoPrueba->show();
    ui->spinBoxTiempoPrueba->show();
}

void MainWindow::on_pushButtonPrueba4_clicked()
{
    pruebaNumero=4;
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);
    ui->labelNombrePrueba->setText("Prueba 4:Prueba Base");

    ui->labelCantidadObjetivos->show();
    ui->spinBoxCantidadObjetivos->show();

    ui->labelCantidadObjetivos->hide();
    ui->spinBoxCantidadObjetivos->hide();

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
        reportes->guardarImagenGrafico(ui->qCustomPlotGraficoAngulos,1920,1080);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoDespalazamientos)
        reportes->guardarImagenGrafico(ui->qCustomPlotGraficoDesplazamientos,1920,1080);

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

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaDesplazamientos)
        reportes->guardarDesplazamientosEnArchivo(listaDesplazamientos);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoDespalazamientos)
        reportes->guardarDesplazamientosEnArchivo(listaDesplazamientos);

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
    (void) index;
    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_grafico)
    {
        if (elementosdelGrafico.Unidad.contains("grados"))
            ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráfico","Guardar\nDatos\nAngulos");
        else
            ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráfico","Guardar\nDatos\nDesp..");
    }
    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_resultados)
    {
        ui->qCustomPlotResultados->rescaleAxes();
        ui->qCustomPlotResultados->replot();

        ui->pushButtonGuardarImagen->show();
        ui->labelGuardarImagen->setText("Guardar\nGráfico\nResultados");
        ui->labelGuardarImagen->show();

        ui->pushButtonGuardarMuestras->hide();
        ui->labelGuardarMuestras->hide();

        desactivarSpacerEntreBotones();
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaAngulos)
        ocultarMostrarBotonesLabelTabTabla("Guardar\nDatos\nAngulos");


    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaDesplazamientos)
        ocultarMostrarBotonesLabelTabTabla("Guardar\nDatos\nDesp..");

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_TablaMuestras)
        ocultarMostrarBotonesLabelTabTabla("Guardar\nMuestras");


    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoAngulos)
    {
        reportes->replotGraficoAngulos();
        ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráficos\nAngulos","Guardar\nDatos\nAngulos");
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoDespalazamientos)
    {
        reportes->replotGraficoDesplazamientos();
        ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráficos\nDesp..","Guardar\nDatos\nDesp..");
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoMuestras)
    {
        reportes->replotGraficoMuestras();
        ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráficos\nMuestras","Guardar\nMuestras");
    }
}

void MainWindow::ocultarMostrarBotonesLabelTabTabla(const QString &textoLabel)
{
    ui->pushButtonGuardarImagen->hide();
    ui->labelGuardarImagen->hide();

    ui->pushButtonGuardarMuestras->show();
    ui->labelGuardarMuestras->setText(textoLabel);
    ui->labelGuardarMuestras->show();

    desactivarSpacerEntreBotones();
}

void MainWindow::ocultarMostrarBotonesLabelTabGraficos(const QString &textoBoton,const QString &textoLabel)
{
    ui->pushButtonGuardarImagen->show();
    ui->labelGuardarImagen->setText(textoBoton);
    ui->labelGuardarImagen->show();

    ui->pushButtonGuardarMuestras->show();
    ui->labelGuardarMuestras->setText(textoLabel);
    ui->labelGuardarMuestras->show();

    activarSpacerEntreBotones();
}

void MainWindow::on_comboBoxOrientacion_currentTextChanged(const QString &arg1)
{
    if(arg1.toLower().contains("vertical"))
    {
        if (!ui->radioButtonVerticalAtras->isChecked() && !ui->radioButtonVerticalFrente->isChecked() &&
            !ui->radioButtonVerticalIzquierda->isChecked() && !ui->radioButtonVerticalDerecha->isChecked())
            ui->radioButtonVerticalAtras->setChecked(true);

        ui->radioButtonVerticalAtras->show();
        ui->radioButtonVerticalFrente->show();
        ui->radioButtonVerticalDerecha->show();
        ui->radioButtonVerticalIzquierda->show();

        ui->radioButtonHorizontalAbajo->hide();
        ui->radioButtonHorizontalArriba->hide();
    }
    else
    {
        if(!ui->radioButtonHorizontalAbajo->isChecked() && !ui->radioButtonHorizontalArriba->isChecked())
            ui->radioButtonHorizontalArriba->setChecked(true);

        ui->radioButtonHorizontalAbajo->show();
        ui->radioButtonHorizontalArriba->show();

        ui->radioButtonVerticalAtras->hide();
        ui->radioButtonVerticalFrente->hide();
        ui->radioButtonVerticalDerecha->hide();
        ui->radioButtonVerticalIzquierda->hide();
    }
}

void MainWindow::on_lineEditRut_textChanged(const QString &arg1)
{
    (void) arg1;
    QStringList wordList=db->listarRutPacientes();
    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditRut->setCompleter(completer);
}

void MainWindow::on_pushButtonBuscarPaciente_clicked()
{
    const QString rut=ui->lineEditRut->text();
    if (rut.isEmpty())
        QMessageBox::warning(this,"Datos Vacios","Debe Ingresar un Rut a Buscar");
    else
    {
        QStringList datos=db->buscarPacienteporRut(rut);
        if(datos.isEmpty()){
            QMessageBox messageBox(QMessageBox::Question,
                        tr("El Rut no Existe"),
                        tr("El Rut no existe en la base de datos, desea agregarlo?"),
                        QMessageBox::Yes | QMessageBox::No,
                        this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Agregar"));
            messageBox.setButtonText(QMessageBox::No, tr("No"));

            if (messageBox.exec() == QMessageBox::Yes){
               db->show();
            }
        }
        else{
            ui->labelNombrePaciente->setText("Nombre: "+datos.at(0));
            ui->labelApellidoPaciente->setText("Apellido: "+datos.at(1));
            ui->labelEdadPaciente->setText("Edad: "+datos.at(2));
        }
    }
}
