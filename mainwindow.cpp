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
    ajustesCalculoAngulo=new AjustesCalculoAngulo(this);

    lectorSerial = new Serial(this, new QSerialPort(this)); //Se le envia el objeto en el constructor de la clase Serial
    reportes = new Reportes(this,ui->qCustomPlotResultados,ui->qCustomPlotGraficoAngulos,ui->qCustomPlotGraficoDesplazamientosProyeccion,ui->qCustomPlotGraficoDesplazamientosRecorridoCurvo,
                            ui->qCustomPlotGraficoMuestras,ui->tableWidgetAngulos,ui->tableWidgetDesplazamientosProyeccion,ui->tableWidgetDesplazamientosRecorridoCurvo,
                            ui->tableWidgetDatosRaw,ui->textEditReporte);
    analisisGraficoAngulos = new AnalisisGrafico(this,reportes);
    analisisGraficoMuestras = new AnalisisGrafico(this,reportes);
    analisisGraficoDesplazamientoProyeccion = new AnalisisGrafico(this,reportes);
    analisisGraficoDesplazamientoRecorridoCurvo = new AnalisisGrafico(this,reportes);

    ui->stackedWidget->setCurrentWidget(ui->widgetWelcome);
    ui->dockWidget->installEventFilter(this);
    ui->qCustomPlotGrafico->installEventFilter(this);
    ui->lineEditRut->setValidator(new QIntValidator(0, 999999999) );

    ui->qCustomPlotGrafico->setContextMenuPolicy(Qt::CustomContextMenu);
    titulo = new QCPPlotTitle(ui->qCustomPlotGrafico);
    titulo->setFont(QFont("Times", 10, QFont::Bold));
    ui->qCustomPlotGrafico->plotLayout()->insertRow(0);
    ui->qCustomPlotGrafico->plotLayout()->addElement(0, 0,titulo);

    circuloExterior= new QCPItemEllipse(ui->qCustomPlotGrafico);
    circuloInterior= new QCPItemEllipse(ui->qCustomPlotGrafico);

    ui->tableWidgetDatosRaw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetAngulos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetDesplazamientosProyeccion->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetDesplazamientosRecorridoCurvo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->radioButtonHorizontalAbajo->hide();
    ui->radioButtonHorizontalArriba->hide();
    ui->menuVer->addAction(ui->mainToolBar->toggleViewAction());

    ui->qCustomPlotResultados->hide();
    ui->pushButtonVolverPrueba->hide();

    prueba=new Prueba(this);

    db = new SQL(this);

    //Crean instancias de Objetos
    objetoAngulo=new Angulo;
    dialogCarga=new DialogCarga(this);
}

void MainWindow::conexiones()
{
    //Se envia el dato para graficar en tiempo Real
    connect(this,SIGNAL(emitAnguloGraficoTiempoReal(double,double)),this,SLOT(slotGraficarTiempoReal(double,double)));

    //Conjunto de connects para enviar datos a los graficos de Reportes
    connect(this,SIGNAL(emitAnguloReporte(Angulo*)),reportes,SLOT(agregarDatosGraficoAngulos(Angulo*)));
    connect(this,SIGNAL(emitAnguloReporte(Angulo*)),reportes,SLOT(agregarFilaTablaAngulos(Angulo*)));
    connect(this,SIGNAL(emitDesplazamientoReporte(Desplazamiento*)),reportes,SLOT(agregarDatosGraficoDesplazamientoProyeccion(Desplazamiento*)));
    connect(this,SIGNAL(emitDesplazamientoReporte(Desplazamiento*)),reportes,SLOT(agregarDatosGraficoDesplazamientoRecorridoCurvo(Desplazamiento*)));
    connect(this,SIGNAL(emitDesplazamientoReporte(Desplazamiento*)),reportes,SLOT(agregarFilaTablasDesplazamientos(Desplazamiento*)));
    connect(this,SIGNAL(emitRawReporte(Muestra*)),reportes,SLOT(agregarDatosGraficoMuestras(Muestra*)));
    connect(this,SIGNAL(emitRawReporte(Muestra*)),reportes,SLOT(agregarFilaTablaMuestras(Muestra*)));
    connect(this,SIGNAL(emitGraficarResultados(QVector<Angulo*>)),reportes,SLOT(graficarResultados(QVector<Angulo*>)));

    connect(ui->verticalSliderRangeGraphic,SIGNAL(valueChanged(int)),this,SLOT(actualizarRangoGrafico(int)));
    connect(ui->qCustomPlotGrafico, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
    connect(lectorSerial,SIGNAL(datosLeidos(double,double,double,double,double,double)),this,SLOT(obtenerRaw(double,double,double,double,double,double)));
    connect(ui->lineEditRut,SIGNAL(returnPressed()),ui->pushButtonBuscarPaciente,SLOT(click()));

    connect(ui->pushButtonIniciarPrueba,SIGNAL(clicked()),this,SLOT(configurarArduino()));
    connect(ui->pushButtonDetenerPrueba,SIGNAL(clicked()),this,SLOT(mostrarResultados()));

    connect(ui->pushButtonAnalizarGraficoAngulos,SIGNAL(clicked()),analisisGraficoAngulos,SLOT(show()));
    connect(ui->pushButtonAnalizarGraficoDesplazamientosProyeccion,SIGNAL(clicked()),analisisGraficoDesplazamientoProyeccion,SLOT(show()));
    connect(ui->pushButtonAnalizarGraficoDesplazamientosRecorridoCurvo,SIGNAL(clicked()),analisisGraficoDesplazamientoRecorridoCurvo,SLOT(show()));
    connect(ui->pushButtonAnalizarGraficoMuestras,SIGNAL(clicked()),analisisGraficoMuestras,SLOT(show()));
    connect(ui->pushButtonGuardarReportePDF,QPushButton::clicked,[=]{ reportes->guardarInformeReportePDF(); });


    //Conect de Botones de Prueba
    connect(ui->pushButtonPrueba1,SIGNAL(clicked()),this,SLOT(irAConfigurarPrueba()));
    connect(ui->pushButtonPrueba2,SIGNAL(clicked()),this,SLOT(irAConfigurarPrueba()));
    connect(ui->pushButtonPrueba3,SIGNAL(clicked()),this,SLOT(irAConfigurarPrueba()));
    connect(ui->pushButtonPrueba4,SIGNAL(clicked()),this,SLOT(irAConfigurarPrueba()));
    connect(ui->pushButtonOtro,SIGNAL(clicked()),this,SLOT(irAConfigurarPrueba()));


    //Connect de actions
    connect(ui->actionExportar,QAction::triggered,[=](){ prueba->exportar(); });
    connect(ui->actionImportar,QAction::triggered,[=](){
        if(prueba->importar()){
            reportes->setDatosTablaAngulos(prueba->listaAngulos);
            reportes->setDatosGraficoAngulos(prueba->listaAngulos);
            reportes->setDatosTablasDesplazamientos(prueba->listaDesplazamientos);
            reportes->setDatosGraficosDezplazamientos(prueba->listaDesplazamientos);
            reportes->setDatosTablaMuestras(prueba->listaMuestras);
            reportes->setDatosGraficoMuestras(prueba->listaMuestras);
            mostrarResultados();
            ui->stackedWidget->setCurrentWidget(ui->widgetPruebayResultados);
            ui->tabWidgetGrafico_Resultados->setTabEnabled(0,false);
            ui->tabWidgetGrafico_Resultados->currentChanged(1);
            ui->actionGraficoPrincipal->blockSignals(true);
        }
    });

    connect(ui->actionConfigurar_Serial,SIGNAL(triggered()),ajustesSerial,SLOT(exec()));
    connect(ui->actionConfigurar_Sensores,SIGNAL(triggered(bool)),ajustesSensores,SLOT(exec()));
    connect(ui->actionConfigurar_Grafico,SIGNAL(triggered(bool)),ajustesGrafico,SLOT(exec()));
    connect(ui->actionConfigurar_Angulo,SIGNAL(triggered(bool)),ajustesCalculoAngulo,SLOT(exec()));
    connect(ui->actionSQL,SIGNAL(triggered(bool)),db,SLOT(show()));
    connect(ui->actionAgregar_Paciente,QAction::triggered,[=]{ db->abrirTabAgregarPaciente("");});
    connect(ui->actionInicio,SIGNAL(triggered()),this,SLOT(regresarInicio()));
    connect(ui->actionSalir,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionAcerca,QAction::triggered,[=]{ Acerca *acerca=new Acerca(this); acerca->exec();});
    connect(ui->actionQT,SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));
}

void MainWindow::inicializarGrafico()
{
    limpiarGrafico(ui->qCustomPlotGrafico);
    if(prueba->getNumeroPrueba()!=-1){
        circuloExterior->topLeft->setCoords(-prueba->getAjustesGrafico().RadioExterior,prueba->getAjustesGrafico().RadioExterior);
        circuloExterior->bottomRight->setCoords(prueba->getAjustesGrafico().RadioExterior,-prueba->getAjustesGrafico().RadioExterior);

        circuloExterior->setVisible(true);
        circuloExterior->setBrush(QBrush(Qt::yellow));

        if (prueba->getAjustesGrafico().Unidad.contains("grados"))
            titulo->setText("Grados Antero-Posterior y Medio Lateral");
        else{
            if(prueba->getAjustesGrafico().CalculoDesplazamiento.contains("proyeccion"))
                titulo->setText("Centímetros Antero-Posterior y Medio Lateral (Proyeccion)");
            else
                titulo->setText("Centímetros Antero-Posterior y Medio Lateral (Recorrido Curvo)");
        }

        circuloInterior->setVisible(true);
        circuloInterior->topLeft->setCoords(-prueba->getAjustesGrafico().RadioInterior,prueba->getAjustesGrafico().RadioInterior);
        circuloInterior->bottomRight->setCoords(prueba->getAjustesGrafico().RadioInterior,-prueba->getAjustesGrafico().RadioInterior);

        generarObjetivos();

        ui->qCustomPlotGrafico->addGraph(); // Para el Grafico del punto
        ui->qCustomPlotGrafico->graph(0)->setPen(QPen(Qt::blue));
        ui->qCustomPlotGrafico->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->qCustomPlotGrafico->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

        lienzo = new QCPCurve(ui->qCustomPlotGrafico->xAxis,ui->qCustomPlotGrafico->yAxis);
        lienzo->setPen(QPen(Qt::blue, 1));

        ui->qCustomPlotGrafico->addPlottable(lienzo);

        //Se configuran los rangos maximos para los ejes X e Y segun el slider.
        const int range=ui->verticalSliderRangeGraphic->value();
        ui->qCustomPlotGrafico->xAxis->setRange(-range,range);
        ui->qCustomPlotGrafico->yAxis->setRange(-range,range);
        ui->qCustomPlotGrafico->xAxis2->setRange(-range,range);
        ui->qCustomPlotGrafico->yAxis2->setRange(-range,range);
        ui->qCustomPlotGrafico->setInteractions(QCP::iRangeDrag); // Para usar el el Zoom y el Arrastre del grafico.
    }
    else
    {
        if (prueba->getAjustesGrafico().Unidad.contains("grados")){
            if(ui->comboBoxTipoGrafico->currentText().toLower().contains("medio-lateral"))
                titulo->setText("Tiempo Ángulo Medio-Lateral");
            if(ui->comboBoxTipoGrafico->currentText().toLower().contains("antero-posterior"))
                titulo->setText("Tiempo Ángulo Antero-Posterior");
        }
        else{
            if(ui->comboBoxTipoGrafico->currentText().toLower().contains("medio-lateral"))
                titulo->setText("Tiempo Desplazamiento Medio-Lateral");
            if(ui->comboBoxTipoGrafico->currentText().toLower().contains("antero-posterior"))
                titulo->setText("Tiempo Desplazamiento Antero-Posterior");
        }

        circuloExterior->setVisible(false);
        circuloInterior->setVisible(false);
        ui->qCustomPlotGrafico->addGraph(); // blue line
        ui->qCustomPlotGrafico->graph(0)->setPen(QPen(Qt::blue));
        ui->qCustomPlotGrafico->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
        ui->qCustomPlotGrafico->graph(0)->setAntialiasedFill(false);

        ui->qCustomPlotGrafico->addGraph(); // blue dot
        ui->qCustomPlotGrafico->graph(1)->setPen(QPen(Qt::blue));
        ui->qCustomPlotGrafico->graph(1)->setLineStyle(QCPGraph::lsNone);
        ui->qCustomPlotGrafico->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
        ui->qCustomPlotGrafico->setInteractions(QCP::iRangeZoom|QCP::iRangeDrag); // Para usar el el Zoom y el Arrastre del grafico.
    }
}

/*
 *Actualiza el Mensaje de la Barra de Estado.
 */
void MainWindow::actualizarMensajeBarraEstado(const QString &message)
{
    ui->statusBar->showMessage(message);
}

/*
* Se realizan las acciones Post-Prueba para la generación de reportes y herramientas
* de analisis de los graficos, junto con el informe de Resultados
*/
void MainWindow::mostrarResultados()
{
    if(!prueba->listaMuestras.isEmpty()){
        prueba->setCantidadMuestras(prueba->listaMuestras.size());
        prueba->setTiempoTotal(prueba->listaMuestras.last()->getTiempo());
        lectorSerial->cerrarPuertoSerial();

        emit emitGraficarResultados(prueba->listaAngulos);

        analisisGraficoAngulos->setListaAngulos(prueba->listaAngulos);
        analisisGraficoDesplazamientoProyeccion->setListaDesplazamientosProyeccion(prueba->listaDesplazamientos);
        analisisGraficoDesplazamientoRecorridoCurvo->setListaDesplazamientosRecorridoCurvo(prueba->listaDesplazamientos);
        analisisGraficoMuestras->setListaMuestras(prueba->listaMuestras);
        if(prueba->getPaciente().isEmpty())
        {
            Paciente pac;
            pac.setRut("S/N");
            pac.setNombre("S/N");
            pac.setApellido("S/N");
            pac.setEdad("S/N");
            pac.setAltura("S/N");
            prueba->setPaciente(pac);
        }
        llenarInformeReporte();
        activarTabs();
        conectarActionsParaIrATabs();
    }
    else
        QMessageBox::critical(this,"A ocurrido un problema","A ocurrido un problema y no se realizaron mediciones\nverifique que el dispositivo esta conectado y correctamente configurado");

    mostrarBotonesPrueba();
    activarActions();
    ui->centralWidget->adjustSize();
}

/*
* Se utiliza la plantilla de Reporte dentro de los Resources
* Para completar el Informe que se despliega en resultados.
*/
void MainWindow::llenarInformeReporte()
{
    reportes->inicializarInformeReporte();
    reportes->agregarDatosInformeReportePlainText(":rutP",prueba->getPaciente().getRut());
    reportes->agregarDatosInformeReportePlainText(":nombreP",prueba->getPaciente().getNombre());
    reportes->agregarDatosInformeReportePlainText(":apellidoP",prueba->getPaciente().getApellido());
    reportes->agregarDatosInformeReportePlainText(":edadP" ,prueba->getPaciente().getEdad());
    reportes->agregarDatosInformeReportePlainText(":alturaP" ,prueba->getPaciente().getAltura());

    reportes->agregarDatosInformeReportePlainText(":numeroP",QString::number(prueba->getNumeroPrueba()));
    reportes->agregarDatosInformeReportePlainText(":tiempoPrueba",QString::number(prueba->getTiempoTotal()));
    reportes->agregarDatosInformeReporteImagen(":graficobarras",ui->qCustomPlotResultados->toPixmap(400,400).toImage());
    ui->qCustomPlotGraficoDesplazamientosProyeccion->rescaleAxes();
    reportes->agregarDatosInformeReporteImagen(":graficodesp",ui->qCustomPlotGraficoDesplazamientosProyeccion->toPixmap(400,400).toImage());
    reportes->agregarDatosInformeReporteImagen(":analisisdesplazamiento",analisisGraficoDesplazamientoProyeccion->obtenerImagenTablaEstadisticos());
    reportes->agregarDatosInformeReporteImagen("...",analisisGraficoDesplazamientoRecorridoCurvo->obtenerImagenTablaEstadisticos());

    ui->textEditReporte->moveCursor(QTextCursor::Start);
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

/*
* Se Desactivan las tabs a excepcion de la correspondiente
* Al gráfico Principal.
*/
void MainWindow::desactivarTabs()
{
    for (int var = 1; var < ui->tabWidgetGrafico_Resultados->count(); ++var)
        ui->tabWidgetGrafico_Resultados->setTabEnabled(var,false);
}
/*
* Se Reactivan las tabs.
*/
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

/*
* Función para generar una lista de Objetivos que será almacenada
* dentro de la listaObjetivos de el Objeto Prueba utilizado.
* Se utilizarán los datos tomados de la interfaz y almacenados en el objeto prueba.
*/
void MainWindow::generarObjetivos()
{
    const int pruebaNumero=prueba->getNumeroPrueba();
    if(pruebaNumero==3||pruebaNumero==4)
    {
        const int distanciaCentro=prueba->getAjustesGrafico().RadioInterior;
        const int cantidadObjetivos=pruebaNumero==3? 8: 4;
        for (int var = 0; var < cantidadObjetivos; ++var){
            QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
            ui->qCustomPlotGrafico->addItem(objetivo);
            const double angulo=2*var*((M_PI)/cantidadObjetivos); //
            objetivo->topLeft->setCoords(qCos(angulo)*distanciaCentro-prueba->getAjustesGrafico().RadioObjetivo,qSin(angulo)*distanciaCentro+prueba->getAjustesGrafico().RadioObjetivo);
            objetivo->bottomRight->setCoords(qCos(angulo)*distanciaCentro+prueba->getAjustesGrafico().RadioObjetivo,qSin(angulo)*distanciaCentro-prueba->getAjustesGrafico().RadioObjetivo);
            objetivo->setBrush(QBrush(Qt::red));
            prueba->listaObjetivos.append(objetivo);
        }
        QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
        ui->qCustomPlotGrafico->addItem(objetivo);
        objetivo->topLeft->setCoords(-prueba->getAjustesGrafico().RadioObjetivo,prueba->getAjustesGrafico().RadioObjetivo);
        objetivo->bottomRight->setCoords(prueba->getAjustesGrafico().RadioObjetivo,-prueba->getAjustesGrafico().RadioObjetivo);
        objetivo->setBrush(QBrush(Qt::red));
        prueba->listaObjetivos.append(objetivo);
    }
    if(pruebaNumero==1 || pruebaNumero==2)
    {
       const int cantidadObjetivos=prueba->getCantidadObjetivos();

        if(prueba->getAleatorios()){
            int cantidadintentos=0;
            while(prueba->listaObjetivos.size()<cantidadObjetivos && cantidadintentos<10000){
               //para los Random
                const int signox=qrand()%2==1 ? 1: -1;
                const int signoy=qrand()%2==1 ? 1: -1;

                const int randomx=(qrand()%(prueba->getAjustesGrafico().RadioExterior-prueba->getAjustesGrafico().RadioObjetivo))*signox;
                const int randomy=(qrand()%(prueba->getAjustesGrafico().RadioExterior-prueba->getAjustesGrafico().RadioObjetivo))*signoy;
                const double ecuacionCircExt=qPow(randomx,2)+qPow(randomy,2);

                if(ecuacionCircExt<=qPow(double(prueba->getAjustesGrafico().RadioExterior-prueba->getAjustesGrafico().RadioObjetivo),2)){//Si es que no se sale del radio exterior
                    bool noIntersectaOtros=true;
                    foreach (QCPItemEllipse *P, prueba->listaObjetivos){//Se analiza si el candidato a agregar no intersecta con otros ya agregados
                        const double perteneceCirc=qSqrt(qPow((randomx - (P->topLeft->coords().x()+prueba->getAjustesGrafico().RadioObjetivo)),2)+qPow((randomy - (P->topLeft->coords().y()-prueba->getAjustesGrafico().RadioObjetivo)),2));
                        //QTextStream(stdout)<<"x:"<<P->center->toQCPItemPosition()->coords().x()<<" y:"<<P->center->toQCPItemPosition()->coords().y()<<endl;
                        if( perteneceCirc < 2*prueba->getAjustesGrafico().RadioObjetivo + 0.5)
                            noIntersectaOtros=false;
                    }
                    if(noIntersectaOtros){
                        QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
                        ui->qCustomPlotGrafico->addItem(objetivo);

                        objetivo->topLeft->setCoords(randomx-prueba->getAjustesGrafico().RadioObjetivo,randomy+prueba->getAjustesGrafico().RadioObjetivo);
                        objetivo->bottomRight->setCoords(randomx+prueba->getAjustesGrafico().RadioObjetivo,randomy-prueba->getAjustesGrafico().RadioObjetivo);
                        objetivo->setBrush(QBrush(prueba->getAjustesGrafico().colorObjetivoSinMarcar));
                        prueba->listaObjetivos.append(objetivo);

                        cantidadintentos=0;
                    }
                    else
                        ++cantidadintentos;
                }
            }
        }
        else
        {
            const int distanciaCentro=prueba->getAjustesGrafico().RadioInterior;
            for (int var = 0; var < cantidadObjetivos; ++var){
                QCPItemEllipse *objetivo=new QCPItemEllipse(ui->qCustomPlotGrafico);
                ui->qCustomPlotGrafico->addItem(objetivo);
                const double angulo=2*var*((M_PI)/cantidadObjetivos);
                objetivo->topLeft->setCoords(qCos(angulo)*distanciaCentro-prueba->getAjustesGrafico().RadioObjetivo,qSin(angulo)*distanciaCentro+prueba->getAjustesGrafico().RadioObjetivo);
                objetivo->bottomRight->setCoords(qCos(angulo)*distanciaCentro+prueba->getAjustesGrafico().RadioObjetivo,qSin(angulo)*distanciaCentro-prueba->getAjustesGrafico().RadioObjetivo);
                objetivo->setBrush(QBrush(Qt::red));
                prueba->listaObjetivos.append(objetivo);
            }
        }
    }
}

/*
* A partir de la interfaz se obtiene una cadena que representa la orientacion del sensor
* utilizando el comboBox y el radiobutton usado.
*/
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
            orientacion+="Arriba";
        if(ui->radioButtonHorizontalAbajo->isChecked())
            orientacion+="Abajo";
    }
    return orientacion;
}

/*
* Se analiza segun el tipo de prueba si el objetivo es marcable, y cual es que actualmente esta parpadenado
* Si se selecciona el modo aleatorios ninguno parpadea.
*/
void MainWindow::marcarObjetivos(const double x,const double y)
{
    const int pruebaNumero=prueba->getNumeroPrueba();
    if(!prueba->listaObjetivos.isEmpty()){
        if(pruebaNumero== 3 || pruebaNumero== 4)
        {
            if(!prueba->listaObjetivos.isEmpty())
            {
                static bool centro=true;
                if(centro)
                {
                    QCPItemEllipse *P=prueba->listaObjetivos.last();
                    parpadeoCirculo(P);

                    if(PertenecePuntoAlObjetivo(x,y,P))
                        centro=false;
                }
                else{

                    QCPItemEllipse *P=prueba->listaObjetivos.at(0);
                    parpadeoCirculo(P);

                    if(PertenecePuntoAlObjetivo(x,y,P)){
                        prueba->listaObjetivos.removeAt(0);
                        ui->lcdNumberObjetivosRestantes->display(prueba->listaObjetivos.size());
                        QTextStream(stdout)<<prueba->listaObjetivos.size()<<endl;
                        centro=true;
                    }
                }
            }
        }
        if(pruebaNumero==1 || pruebaNumero==2)
        {
            if(!prueba->getObjetivosEnOrden())//Si es que se puede ir a cualquier objetivo
            {
                for (int var = 0; var < prueba->listaObjetivos.size(); ++var)//Se recorre la prueba->lista de Objetivos y verifica si se pasa por algun objetivo.
                {
                    QCPItemEllipse *P=prueba->listaObjetivos.at(var);
                    if(P->brush()==QBrush(prueba->getAjustesGrafico().colorObjetivoSinMarcar)) //Si aun sigue con el color por defecto.
                    {
                        if(PertenecePuntoAlObjetivo(x,y,P)){
                            prueba->listaObjetivos.removeAt(var);
                            ui->lcdNumberObjetivosRestantes->display(prueba->listaObjetivos.size());
                        }
                    }
                }
            }
            else
            {
               if(!prueba->listaObjetivos.isEmpty())
               {
                   QCPItemEllipse *P=prueba->listaObjetivos.at(0);
                   parpadeoCirculo(P);

                   if( PertenecePuntoAlObjetivo(x,y,P)){
                       prueba->listaObjetivos.removeAt(0);
                       ui->lcdNumberObjetivosRestantes->display(prueba->listaObjetivos.size());
                   }
               }
            }
        }
    }
    //Si no es la Prueba -1 y se escogio la opción de Detener al Marcar.
    if(prueba->getNumeroPrueba()!=-1 && prueba->getCantidadObjetivos()>0 && prueba->listaObjetivos.isEmpty() && prueba->getDetenerAlMarcarTodos())
        ui->pushButtonDetenerPrueba->click();
}

/*
* Se verifica si el punto dado pertenece a un circulo usando geometria
* En caso pertenecer cambia el color de esta y ademas retorna True
* En caso contrario retorna False.
*/
bool MainWindow::PertenecePuntoAlObjetivo(const double x,const double y,QCPItemEllipse *P){
    const double perteneceCirc=qSqrt(qPow(( x - (P->topLeft->coords().x()+prueba->getAjustesGrafico().RadioObjetivo)),2)+qPow(( y - (P->topLeft->coords().y()-prueba->getAjustesGrafico().RadioObjetivo)),2));
    if( perteneceCirc < prueba->getAjustesGrafico().RadioObjetivo){
        P->setBrush(QBrush(prueba->getAjustesGrafico().colorObjetivoMarcado));
        return true;
    }
    return false;
}

/*
* Dado un QCPItemEllipse se calcula el tiempo y se hace parpadear
* repintando la elipse entre dos colores
* según el módulo del tiempo transcurrido
*/
void MainWindow::parpadeoCirculo(QCPItemEllipse *P)
{
    if(cronometro.elapsed()/1000%2==0)
         P->setBrush(QBrush(Qt::white));
    else
        P->setBrush(QBrush(prueba->getAjustesGrafico().colorObjetivoSinMarcar));
}

/*
* SLOT para graficar según un nuevo dato sea leído por el Sensor,
* junto con realizar las acciones en los objetivos
*/
void MainWindow::slotGraficarTiempoReal(const double x,const double y)
{
    if(prueba->getNumeroPrueba()!=-1){
        marcarObjetivos(x,y); //Se hace parpadear o verifica el objetivo segun las configuraciones de la prueba

        if(ui->checkBoxLimitarGrafico->isChecked())
        {
            const double ecuacionCircExt=qPow(x,2)+qPow(y,2);
            if(ecuacionCircExt<=qPow(double(prueba->getAjustesGrafico().RadioExterior),2)){//Si es que no se sale del radio exterior
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

                const double aX=prueba->getAjustesGrafico().RadioExterior*qCos(qDegreesToRadians(inclRecta));
                const double aY=prueba->getAjustesGrafico().RadioExterior*qSin(qDegreesToRadians(inclRecta));
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
    }
    else{
        ui->qCustomPlotGrafico->graph(0)->addData(x, y);
        ui->qCustomPlotGrafico->graph(1)->clearData();
        ui->qCustomPlotGrafico->graph(1)->addData(x, y);
        ui->qCustomPlotGrafico->graph(1)->rescaleValueAxis(true);
        ui->qCustomPlotGrafico->xAxis->setRange(x+0.25, 8, Qt::AlignRight);
    }
    ui->qCustomPlotGrafico->replot(); //Se redibuja el grafico
}
/*
* Se actualizan los rangos del gráfico
* Se redibuja el gráfico.
*/
void MainWindow::actualizarRangoGrafico(int Range)
{
    ui->qCustomPlotGrafico->xAxis->setRange(-Range,Range);
    ui->qCustomPlotGrafico->yAxis->setRange(-Range,Range);
    ui->qCustomPlotGrafico->xAxis2->setRange(-Range,Range);
    ui->qCustomPlotGrafico->yAxis2->setRange(-Range,Range);
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

/*
* Filtro de Eventos segun Objeto, se asigna en installEventFilter
* Si es el QCP se reescribe el evento Zoom
* Si es el DockWidget Mantiene el aspecto cuadrado
*/
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Resize && obj == ui->dockWidget)
        relacionAspectodelGrafico();

    if(event->type()==QEvent::Wheel && obj == ui->qCustomPlotGrafico && prueba->getNumeroPrueba()!=-1){
        QWheelEvent *wheelEvent = (QWheelEvent *)event;
        QCPRange range=ui->qCustomPlotGrafico->xAxis->range();
        range+=(wheelEvent->delta()/120);
        ui->verticalSliderRangeGraphic->setValue(range.upper); //Acticar el Slot
        ui->qCustomPlotGrafico->replot();
    }
    return QWidget::eventFilter(obj, event);
}

/*
* Para mantener el Grafico Principal con ejes de igual tamaño
* con el fin de que siga estando un circulo.
*/
void MainWindow::relacionAspectodelGrafico()
{
    if(prueba->getNumeroPrueba()!=-1){
        const int w=ui->qCustomPlotGrafico->width();
        const int h=ui->qCustomPlotGrafico->height();
        const QRect rect=ui->qCustomPlotGrafico->geometry();
        if(w>h)
            ui->qCustomPlotGrafico->setGeometry(rect.x()+((w-h)/2),rect.y(),h,h);
        else
            ui->qCustomPlotGrafico->setGeometry(rect.x(),rect.y()+((h-w)/2),w,w);
    }
}

/*
* Se intenta configurar Micro-controlador Arduino además de mostrar un
* QDialog que ameniza la carga junto con darle tiempo de realizar la escritura Serial.
*/
void MainWindow::configurarArduino()
{
    const AjustesPuertoSerial::Ajustes aSerial=ajustesSerial->getAjustes();
    if(lectorSerial->abrirPuertoSerial(aSerial))//Se abre el puerto serial con sus ajustes respectivos
    {
        prueba->setAjustesPuertoSerial(aSerial);
        prueba->setCadenaConfiguracion(ajustesSensores->getAjustesSensores());
        prueba->setFrecuenciaMuestreo(ajustesSensores->obtenerFrecuenciaMuestreo());
        const double tiempo=3000;
        QString texto=QString("Actualizando configuracion de sensores\nPuerto: %1\nFrecuencia Muestreo: %2 Hz").arg(ajustesSerial->getAjustes().portName).arg(prueba->getFrecuenciaMuestreo());
        dialogCarga->setTextoCarga(texto);
        dialogCarga->iniciar(tiempo);
        QTimer *timer=new QTimer(this); //Se crea un timer para enviar las configuraciones de los sensores
        timer->setSingleShot(true);

        connect(timer, QTimer::timeout, [=]() { lectorSerial->escribirDatosSerial(prueba->getCadenaConfiguracion()); });
        connect(timer, QTimer::timeout, [=]() { dialogCarga->close();});
        connect(timer, QTimer::timeout, [=]() { iniciarPrueba(); });
        connect(timer, QTimer::timeout, [=]() { timer->stop();});
        connect(timer, QTimer::timeout, [=]() { delete timer; dialogCarga->pararMovie();});

        timer->start(tiempo); //Se fija el tiempo de accion en 3 seg
        dialogCarga->exec();
    }
    else
        QMessageBox::warning(this,"Error al conectar","Error Abriendo el Puerto Serial",QMessageBox::Ok);
}

/*
* Limpieza de prueba->listas y de los elementos de la interfaz
*/
void MainWindow::limpiarListasyOcultarBotones()
{
    prueba->limpiarListas();
    reportes->vaciarTablas();
    reportes->vaciarGraficos();
    reportes->vaciarInformeReporte();
    analisisGraficoAngulos->hide();
    analisisGraficoDesplazamientoProyeccion->hide();
    analisisGraficoDesplazamientoRecorridoCurvo->hide();
    analisisGraficoMuestras->hide();
}

/*
* Se bloquean todas las señales de los QAction para envitar interrupciones
*/
void MainWindow::desactivarActions()
{
    QList<QAction *> actionList = this->findChildren<QAction *>();
    foreach (QAction *action, actionList)
        action->blockSignals(true);
}

/*
* Se re-activan las señales de los QAction
*/
void MainWindow::activarActions()
{
    QList<QAction *> actionList = this->findChildren<QAction *>();
    foreach (QAction *action, actionList)
        action->blockSignals(false);
}

void MainWindow::conectarActionsParaIrATabs()
{
    connect(ui->actionGraficoPrincipal,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_grafico); });

    connect(ui->actionReporte,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_resultados); });

    connect(ui->actionTablaAngulos,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_tablaAngulos); });
    connect(ui->actionGraficoAngulos,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_GraficoAngulos); });

    connect(ui->actionTablaDesplazamientoProyeccion,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_tablaDesplazamientosProyeccion); });
    connect(ui->actionGraficoDesplazamientoProyeccion,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_GraficoDesplazamientosProyeccion); });

    connect(ui->actionTablaDesplazamientoRecorridoCurvo,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_tablaDesplazamientosRecorridoCurvo); });
    connect(ui->actionGraficoDesplazamientoRecorridoCurvo,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_GraficoDesplazamientosRecorridoCurvo); });

    connect(ui->actionTablaMuestras,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_TablaMuestras); });
    connect(ui->actionGraficoMuestras,QAction::triggered,[=](){ ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_GraficoMuestras); });
}

void MainWindow::iniciarPrueba()
{
    calibrado=false;
    limpiarListasyOcultarBotones();
    desactivarActions();

    almacenarAjustesInterfazenObjetoPrueba();

    ui->verticalSliderRangeGraphic->setValue(prueba->getAjustesGrafico().RadioExterior+5);//Se actualiza el slider del Rango

    inicializarGrafico(); //Se limpian y Reajustan los graficos

    ui->lcdNumberCantidadObjetivos->display(prueba->listaObjetivos.size());
    ui->lcdNumberObjetivosRestantes->display(prueba->listaObjetivos.size());

    if (prueba->getTiempoPrueba()==qInf())
        ui->progressBarPrueba->hide();

    ui->pushButtonVolverPrueba->show();
    ui->stackedWidget->setCurrentWidget(ui->widgetPruebayResultados);

    if (prueba->getAjustesGrafico().Unidad.contains("grados"))
        ui->labelGuardarMuestras->setText("Guardar\nDatos\nÁngulos");
    else
        ui->labelGuardarMuestras->setText("Guardar\nDatos\nDesp..");

    ui->tabWidgetGrafico_Resultados->setTabEnabled(0,true);

    ui->tabWidgetGrafico_Resultados->setCurrentWidget(ui->tab_grafico);
    ui->centralWidget->adjustSize();
    desactivarTabs();
    desactivarSpacerEntreBotones();
    ocultarBotonesPrueba();
}

void MainWindow::almacenarAjustesInterfazenObjetoPrueba()
{
    const double tPrueba=ui->checkBoxTiempoInfinito->isChecked()?qInf():ui->spinBoxTiempoPrueba->value();
    prueba->setTiempoPrueba(tPrueba); //Se coloca un tiempo infinito o el elegido
    prueba->setAjustesGrafico(ajustesGrafico->getAjustes());//Se obtienen los ajustes actuales para el grafico
    prueba->setOrientacion(obtenerOrientacionSensor());
    prueba->setAlturaDispositivo(ui->doubleSpinBoxAlturaDispositivo->value());
    prueba->setDivisorFPS(); //Se calcula el divisor de FPS
    prueba->setCantidadObjetivos(ui->spinBoxCantidadObjetivos->value());

    //Almacenando los Checkboxs
    prueba->setAleatorios(ui->checkBoxObjetivosAleatorios->isChecked());
    prueba->setDetenerAlMarcarTodos(ui->checkBoxDeteneralMarcarObjetivos->isChecked());
    prueba->setLimitarGrafico(ui->checkBoxLimitarGrafico->isChecked());
    prueba->setObjetivosEnOrden(ui->checkBoxOrdenObjetivos->isChecked());
}

/*
* Se pregunta si se desea volver a Inicio.
*/
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

void MainWindow::calibrar(const double AcX,const double AcY, const double AcZ, const double GyX, const double GyY, const double GyZ)
{
    if(!cronometro.isValid()){
        cronometro.start();
        const double tiempo=ajustesCalculoAngulo->tiempoCalibracion;
        QString texto="Calibrando sensores espere: "+QString::number(tiempo)+" seg.";
        dialogCarga->setTextoCarga(texto);
        dialogCarga->iniciar(tiempo);
        dialogCarga->exec();
    }

    double tiempo=cronometro.elapsed()/1000.0;
    Muestra *dato=new Muestra(tiempo,AcX,AcY,AcZ,GyX,GyY,GyZ);
    const QString orientacion=prueba->getOrientacion().toLower();
    const QString filtroAngulo=ajustesCalculoAngulo->filtro.toLower();
    if(!filtroAngulo.contains("sin filtro")){
        if(!prueba->listaAngulos.isEmpty()){
            Angulo *anguloAnterior=prueba->listaAngulos.last();
            if(filtroAngulo.contains("kalman"))
                objetoAngulo->calcularAnguloFiltroKalman(orientacion,dato,anguloAnterior);
            if(filtroAngulo.contains("complementario")){
                const double alpha=ajustesCalculoAngulo->alpha;
                objetoAngulo->calcularAnguloFiltroComplementario(orientacion,dato,anguloAnterior,alpha);
            }
        }
        else{
            objetoAngulo->calcularAngulo(orientacion,dato);
            Angulo *angulo=new Angulo(objetoAngulo->getTiempo(),objetoAngulo->getAngulo1(),objetoAngulo->getAngulo2());
            prueba->listaAngulos.append(angulo);
        }
    }
    else
        calibrado=true;

    if(cronometro.elapsed()/1000.0>ajustesCalculoAngulo->tiempoCalibracion){
        calibrado=true;
        prueba->listaAngulos.clear();
        dialogCarga->close();
        dialogCarga->pararMovie();
    }
}

/*
* Se obtiene la Informacion proveniete del Sensor
* Ademas se obtiene el Angulo y Desplazamiento
* Junto con enviar a los Elementos Graficos de Reporte y
* Grafico en Tiempo Real el dato que corresponda.
*/
void MainWindow::obtenerRaw(const double AcX, const double AcY, const double AcZ, const double GyX, const double GyY, const double GyZ)
{
    if(calibrado){
        if(prueba->listaMuestras.isEmpty())//Cuando se agrega el primer dato, se inicia el tiempo.
           cronometro.start(); //Para revalidar que la velocidad esta pasando bien :)

        const double tiempo=prueba->getFrecuenciaMuestreo()<275 ? prueba->listaMuestras.size()*(1/prueba->getFrecuenciaMuestreo()) :
                                                              cronometro.elapsed()/1000.0;
        if ( tiempo < prueba->getTiempoPrueba())
        {
            //Calculo y de Angulos y Desplazamiento
            Desplazamiento *desplazamiento=new Desplazamiento;
            Muestra *dato=new Muestra(tiempo,AcX,AcY,AcZ,GyX,GyY,GyZ);
            const QString orientacion=prueba->getOrientacion().toLower();
            const double alpha=ajustesCalculoAngulo->alpha;
            const QString filtroAngulo=ajustesCalculoAngulo->filtro.toLower();

            if(filtroAngulo.contains("sin filtro"))
                objetoAngulo->calcularAngulo(orientacion,dato);
            else{
                if(!prueba->listaAngulos.isEmpty()){
                    Angulo *anguloAnterior=prueba->listaAngulos.last();
                    if(filtroAngulo.contains("kalman"))
                        objetoAngulo->calcularAnguloFiltroKalman(orientacion,dato, anguloAnterior);
                    if(filtroAngulo.contains("complementario"))
                        objetoAngulo->calcularAnguloFiltroComplementario(orientacion,dato, anguloAnterior,alpha);
                }
                else{
                    objetoAngulo->calcularAngulo(orientacion,dato);
                    if(filtroAngulo.contains("kalman"))
                        objetoAngulo->setAnguloInicialKalman(objetoAngulo->getAngulo1(),objetoAngulo->getAngulo2());
                }
            }

    //        if(!prueba->listaAngulos.isEmpty()){
    //            Angulo *anguloAnterior=prueba->listaAngulos.last();
    //             objetoAngulo->calcularAngulo(orientacion,dato);
    //             QTextStream stdout <<objetoAngulo->getTiempo()<<" "<<objetoAngulo->getAnguloX()<<" "<<objetoAngulo->getAnguloY()<<" ";
    //            //if(filtroAngulo.contains("kalman"))
    //                objetoAngulo->calcularAnguloFiltroKalman(orientacion,dato, anguloAnterior);
    //            QTextStream stdout <<objetoAngulo->getAnguloX()<<" "<<objetoAngulo->getAnguloY()<<" ";
    //                //if(filtroAngulo.contains("complementario"))
    //                objetoAngulo->calcularAnguloFiltroComplementario(orientacion,dato, anguloAnterior,alpha);
    //                QTextStream stdout <<objetoAngulo->getAnguloX()<<" "<<objetoAngulo->getAnguloY()<<endl;
    //        }
    //        else{
    //            objetoAngulo->calcularAngulo(orientacion,dato);
    //            if(filtroAngulo.contains("kalman"))
    //                objetoAngulo->setAnguloInicialKalman(objetoAngulo->getAnguloX(),objetoAngulo->getAnguloY());
    //        }

            Angulo *angulo=new Angulo(objetoAngulo->getTiempo(),objetoAngulo->getAngulo1(),objetoAngulo->getAngulo2());
            desplazamiento->calcularDesplazamiento(angulo,prueba->getAlturaDispositivo());
            prueba->listaAngulos.append(angulo);
            prueba->listaDesplazamientos.append(desplazamiento);
            prueba->listaMuestras.append(dato);
            emit emitRawReporte(dato);
            emit emitDesplazamientoReporte(desplazamiento);
            emit emitAnguloReporte(angulo);

            //Comienza actualizacion elementos de la interfaz
            //Se pregunta y envia el dato para graficar
            if(prueba->listaMuestras.size() % prueba->getDivisorFPS()==0)
                seleccionarDatoAGraficar(angulo,desplazamiento);

            if(prueba->getTiempoPrueba()!=qInf())//Si el tiempo es distinto de infinito se calcula el porcentaje
            {
               const int porcentaje=qRound((tiempo*100.0)/prueba->getTiempoPrueba());
               ui->progressBarPrueba->setValue(porcentaje);
            }

            const QString lapso=QString::number(tiempo, 'f', 1);

            QString horas = QString::number(qFloor(tiempo / 3600));
            QString minutos = QString::number(qFloor ((qRound(tiempo)% 3600) / 60 ));
            QString segundos = QString::number(qRound(tiempo) % 60);

            minutos = minutos.toInt() < 10 ? '0' + minutos : minutos;//Anteponiendo un 0 a los minutos si son menos de 10
            segundos = segundos.toInt() < 10 ? '0' + segundos : segundos;//Anteponiendo un 0 a los segundos si son menos de 10
            QString result = horas + ":" + minutos + ":" + segundos;  // hh:mm::ss

            ui->lcdNumberTiempoTranscurrido->display(result);

            const QString mensaje="Tiempo: "+ lapso + " Muestras:" + QString::number(prueba->listaMuestras.size())+ " AcX: "+QString::number(dato->getAcX(),'f',3)+" AcY: "+QString::number(dato->getAcY(),'f',3)+" AcZ: "+QString::number(dato->getAcZ(),'f',3)
                              + " GyX: "+QString::number(dato->getGyX(),'f',3)+" GyY: "+QString::number(dato->getGyY(),'f',3)+" GyZ: "+QString::number(dato->getGyZ(),'f',3);
            actualizarMensajeBarraEstado(mensaje);
        }
        else//Si se agoto el tiempo de la preueba
        {
            cronometro.invalidate();
            mostrarResultados();
            ui->actionGraficoPrincipal->blockSignals(false);
        }
    }
    else
        calibrar(AcX,AcY,AcZ,GyX,GyY,GyZ);
}

void MainWindow::seleccionarDatoAGraficar(Angulo *angulo,Desplazamiento *desplazamiento)
{
    if(prueba->getNumeroPrueba()!=-1){
        if(prueba->getAjustesGrafico().Unidad.contains("grados"))
            emit emitAnguloGraficoTiempoReal(angulo->getAngulo1(),angulo->getAngulo2());
        else{
            if(prueba->getAjustesGrafico().CalculoDesplazamiento.contains("proyeccion"))
                emit emitAnguloGraficoTiempoReal(desplazamiento->getDesplazamientoProyeccion().Desplazamiento1,desplazamiento->getDesplazamientoProyeccion().Desplazamiento2);
            if(prueba->getAjustesGrafico().CalculoDesplazamiento.contains("curvo"))
                emit emitAnguloGraficoTiempoReal(desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento1,desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento2);
        }
    }
    else{
        if(prueba->getAjustesGrafico().Unidad.contains("grados"))
        {
            if(ui->comboBoxTipoGrafico->currentText().toLower().contains("medio-lateral"))
                emitAnguloGraficoTiempoReal(angulo->getTiempo(),angulo->getAngulo1());
            if(ui->comboBoxTipoGrafico->currentText().toLower().contains("antero-posterior"))
                emitAnguloGraficoTiempoReal(angulo->getTiempo(),angulo->getAngulo2());
        }
        else{
            if(prueba->getAjustesGrafico().CalculoDesplazamiento.contains("proyeccion"))
            {
                if(ui->comboBoxTipoGrafico->currentText().toLower().contains("medio-lateral"))
                    emitAnguloGraficoTiempoReal(desplazamiento->getTiempo(),desplazamiento->getDesplazamientoProyeccion().Desplazamiento1);
                if(ui->comboBoxTipoGrafico->currentText().toLower().contains("antero-posterior"))
                    emitAnguloGraficoTiempoReal(desplazamiento->getTiempo(),desplazamiento->getDesplazamientoProyeccion().Desplazamiento2);
            }
            if(prueba->getAjustesGrafico().CalculoDesplazamiento.contains("curvo"))
            {
                if(ui->comboBoxTipoGrafico->currentText().toLower().contains("medio-lateral"))
                    emitAnguloGraficoTiempoReal(desplazamiento->getTiempo(),desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento1);
                if(ui->comboBoxTipoGrafico->currentText().toLower().contains("antero-posterior"))
                    emitAnguloGraficoTiempoReal(desplazamiento->getTiempo(),desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento2);

            }
        }
    }
}

/*
* Llamadas de funciones de reiniciar Prueba
* Se cierra el puerto, se limpian los graficos y se reconfigura Arduino
*/
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
    ui->stackedWidget->setCurrentWidget(ui->widgetPruebayResultados);
}

void MainWindow::limpiarGrafico(QCustomPlot *grafico){

    if(grafico->plotLayout()->hasElement(0,1))
        grafico->plotLayout()->remove(grafico->plotLayout()->element(0,1));

    grafico->legend->clear();
    grafico->legend->setVisible(false);
    grafico->clearFocus();
    grafico->clearGraphs();
    grafico->clearPlottables();
    grafico->clearItems();
    grafico->xAxis->setLabel("");
    grafico->yAxis->setLabel("");
    grafico->rescaleAxes();
    grafico->replot();
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
        //reportes->guardarImagenGrafico(ui->qCustomPlotResultados,1000,1000);
        reportes->guardarInformeReportePDF();

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoAngulos)
        reportes->guardarImagenGrafico(ui->qCustomPlotGraficoAngulos,1920,1080);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoDesplazamientosProyeccion)
        reportes->guardarImagenGrafico(ui->qCustomPlotGraficoDesplazamientosProyeccion,1920,1080);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoDesplazamientosRecorridoCurvo)
        reportes->guardarImagenGrafico(ui->qCustomPlotGraficoDesplazamientosRecorridoCurvo,1920,1080);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoMuestras)
        reportes->guardarImagenGrafico(ui->qCustomPlotGraficoMuestras,1920,1080);
}

void MainWindow::on_pushButtonGuardarMuestras_clicked()//Guardar en archivo la informacion de muestras o angulos.
{    
    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_grafico){
        if(prueba->getAjustesGrafico().Unidad.contains("grados"))
            reportes->guardarAngulosEnArchivo(prueba->listaAngulos);
        else
            reportes->guardarDesplazamientosEnArchivo(prueba->listaDesplazamientos,prueba->getAjustesGrafico().CalculoDesplazamiento);
    }
    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaAngulos)
        reportes->guardarAngulosEnArchivo(prueba->listaAngulos);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoAngulos)
        reportes->guardarAngulosEnArchivo(prueba->listaAngulos);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaDesplazamientosProyeccion)
        reportes->guardarDesplazamientosEnArchivo(prueba->listaDesplazamientos,"proyeccion");

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaDesplazamientosRecorridoCurvo)
        reportes->guardarDesplazamientosEnArchivo(prueba->listaDesplazamientos,"curvo");

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoDesplazamientosProyeccion)
        reportes->guardarDesplazamientosEnArchivo(prueba->listaDesplazamientos,"proyeccion");

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoDesplazamientosRecorridoCurvo)
        reportes->guardarDesplazamientosEnArchivo(prueba->listaDesplazamientos,"curvo");

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_TablaMuestras)
        reportes->guardarMuestrasEnArchivo(prueba->listaMuestras);

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoMuestras)
        reportes->guardarMuestrasEnArchivo(prueba->listaMuestras);
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
        relacionAspectodelGrafico();
        if (prueba->getAjustesGrafico().Unidad.contains("grados"))
            ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráfico","Guardar\nDatos\nAngulos");
        else
            ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráfico","Guardar\nDatos\nDesp..");
    }
    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_resultados)
    {
        ui->qCustomPlotResultados->rescaleAxes();
        ui->qCustomPlotResultados->replot();

        ui->pushButtonGuardarImagen->show();
        ui->labelGuardarImagen->setText("Guardar\nReporte");
        ui->labelGuardarImagen->show();

        ui->pushButtonGuardarMuestras->hide();
        ui->labelGuardarMuestras->hide();

        desactivarSpacerEntreBotones();
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaAngulos)
        ocultarMostrarBotonesLabelTabTabla("Guardar\nDatos\nAngulos");

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaDesplazamientosProyeccion)
        ocultarMostrarBotonesLabelTabTabla("Guardar\nDatos\nDesp..\nProyección");

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_tablaDesplazamientosRecorridoCurvo)
        ocultarMostrarBotonesLabelTabTabla("Guardar\nDatos\nDesp..\nRecorrido\nCurvo");

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_TablaMuestras)
        ocultarMostrarBotonesLabelTabTabla("Guardar\nMuestras");

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoAngulos)
    {
        reportes->replotGraficoAngulos();
        ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráficos\nAngulos","Guardar\nDatos\nAngulos");
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoDesplazamientosProyeccion)
    {
        reportes->replotGraficoDesplazamientoProyeccion();
        ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráficos\nDesp..\nProyección","Guardar\nDatos\nDesp..\nProyección");
    }

    if(ui->tabWidgetGrafico_Resultados->currentWidget()==ui->tab_GraficoDesplazamientosRecorridoCurvo)
    {
        reportes->replotGraficoDesplazamientoRecorridoCurvo();
        ocultarMostrarBotonesLabelTabGraficos("Guardar\nGráficos\nDesp..\nRecorrido\nCurvo","Guardar\nDatos\nDesp..\nRecorrido\nCurvo");
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
        QMessageBox::warning(this,"Datos vacíos","Debe ingresar un Rut a Buscar");
    else
    {
        Paciente paciente=db->buscarPacienteporRut(rut);
        if(paciente.isEmpty()){
            QMessageBox messageBox(QMessageBox::Question,
                        tr("Rut no encontrado"),
                        tr("El Rut: %1 no existe en la base de datos, desea agregarlo?").arg(rut),
                        QMessageBox::Yes | QMessageBox::No,
                        this);
            messageBox.setButtonText(QMessageBox::Yes, tr("Agregar"));
            messageBox.setButtonText(QMessageBox::No, tr("No"));

            if (messageBox.exec() == QMessageBox::Yes)
               db->abrirTabAgregarPaciente(rut);
        }
        else{
            ui->labelNombrePaciente->setText("Nombre: "+paciente.getNombre());
            ui->labelApellidoPaciente->setText("Apellido: "+paciente.getApellido());
            ui->labelEdadPaciente->setText("Edad: "+paciente.getEdad());
            ui->labelSexoPaciente->setText("Sexo: "+paciente.getSexo());
            ui->labelPesoPaciente->setText("Peso: "+paciente.getPeso());
            ui->labelAlturaPaciente->setText("Altura: "+paciente.getAltura());

            prueba->setPaciente(paciente);
        }
    }
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    (void) arg1;
    if(ui->stackedWidget->currentWidget()==ui->widgetConfigurarPrueba)
    {
        if(!prueba->getPaciente().getRut().isEmpty())
            ui->labelPaciente->setText("Paciente: " + prueba->getPaciente().getRut());
        else
            ui->labelPaciente->setText("Paciente: Anónimo");
    }
}

void MainWindow::irAConfigurarPrueba()
{
    mostarElementosConfigurarPrueba();
    QString clicked = sender()->objectName();
    ui->stackedWidget->setCurrentWidget(ui->widgetConfigurarPrueba);

    if(!prueba->getPaciente().isEmpty()){
        const double alturaSugerida=prueba->getPaciente().getAltura().toInt()*0.55;
        ui->labelAlturaSugerida->setText(tr("Altura dispositivo sugerida (55%): %1").arg(alturaSugerida));
        ui->doubleSpinBoxAlturaDispositivo->setValue(alturaSugerida);
    }
    else
        ui->labelAlturaSugerida->hide();

    if(clicked=="pushButtonPrueba1"){
        prueba->setNumeroPrueba(1);
        ui->labelNombrePrueba->setText("Modo Libre");
        ui->groupBoxAjustesPruebaPersonalizada->hide();
    }
    if(clicked=="pushButtonPrueba2"){
        prueba->setNumeroPrueba(2);
        ui->labelNombrePrueba->setText("Prueba 2");
        ui->groupBoxAjustesPruebaPersonalizada->hide();
    }
    if(clicked=="pushButtonPrueba3"){
        prueba->setNumeroPrueba(3);
        ui->labelNombrePrueba->setText("Prueba 3");
        ui->groupBoxAjustesPruebaPersonalizada->hide();

        ui->labelObjetivosAleatorios->hide();
        ui->checkBoxObjetivosAleatorios->hide();

        ui->labelOrdenObjetivos->hide();
        ui->checkBoxOrdenObjetivos->hide();
    }
    if(clicked=="pushButtonPrueba4"){
        prueba->setNumeroPrueba(4);
        ui->labelNombrePrueba->setText("Prueba 4:Prueba Base");
        ui->groupBoxAjustesPruebaPersonalizada->hide();

        ui->labelCantidadObjetivos->hide();
        ui->spinBoxCantidadObjetivos->hide();

        ui->labelObjetivosAleatorios->hide();
        ui->checkBoxObjetivosAleatorios->hide();

        ui->labelOrdenObjetivos->hide();
        ui->checkBoxOrdenObjetivos->hide();
    }
    if(clicked=="pushButtonOtro"){
        prueba->setNumeroPrueba(-1);
        ui->groupBoxAjustesObjetivos->hide();
        ui->labelLimitarGrafico->hide();
        ui->checkBoxLimitarGrafico->hide();
        ui->verticalSliderRangeGraphic->hide();

        ui->lcdNumberCantidadObjetivos->hide();
        ui->labelObjetivos->hide();
        ui->lcdNumberObjetivosRestantes->hide();
        ui->labelObjetivosRestantes->hide();
    }
}

void MainWindow::mostarElementosConfigurarPrueba()
{
    ui->labelAlturaSugerida->show();
    ui->groupBoxAjustesPruebaPersonalizada->show();
    ui->groupBoxAjustesObjetivos->show();

    ui->labelLimitarGrafico->show();
    ui->checkBoxLimitarGrafico->show();

    ui->labelCantidadObjetivos->show();
    ui->spinBoxCantidadObjetivos->show();

    ui->labelObjetivosAleatorios->show();
    ui->checkBoxObjetivosAleatorios->show();

    ui->labelOrdenObjetivos->show();
    ui->checkBoxOrdenObjetivos->show();

    ui->verticalSliderRangeGraphic->show();

    ui->lcdNumberCantidadObjetivos->show();
    ui->labelObjetivos->show();
    ui->lcdNumberObjetivosRestantes->show();
    ui->labelObjetivosRestantes->show();
}
