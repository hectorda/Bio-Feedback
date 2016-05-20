#include "reportes.h"

Reportes::Reportes(QObject *parent) : QObject(parent)
{
    this->presicion = 3;
}

Reportes::Reportes(QObject *parent, QCustomPlot *graficoResultados, QCustomPlot *graficoAngulos, QCustomPlot *graficoMuestras, QTableWidget *tablaAngulos, QTableWidget *tablaMuestras) : QObject(parent)
{
    this->presicion = 4;
    this->graficoResultados = graficoResultados;
    this->graficoAngulos = graficoAngulos;
    this->graficoMuestras = graficoMuestras;
    this->tablaAngulos = tablaAngulos;
    this->tablaMuestras = tablaMuestras;
    inicializarGraficoResultados();
    inicializarGraficoAngulos();
    inicializarGraficoMuestras();
}

void Reportes::vaciarTablas()
{
    tablaAngulos->clearContents();
    tablaAngulos->setRowCount(0);
    tablaMuestras->clearContents();
    tablaMuestras->setRowCount(0);
}

void Reportes::vaciarGraficos()
{
    cuadrantes->clearData();

    vaciarGraficoAngulos();

    graficoAcX->clearData();
    graficoAcY->clearData();
    graficoAcZ->clearData();
    graficoGyX->clearData();
    graficoGyY->clearData();
    graficoGyZ->clearData();
}

void Reportes::vaciarGraficoAngulos()
{
    graficoAnguloX->clearData();
    graficoAnguloY->clearData();
}

void Reportes::replotGraficoAngulos()
{
    graficoAnguloX->rescaleAxes();
    graficoAnguloY->rescaleAxes();
    graficoAngulos->replot();
}

void Reportes::inicializarGraficoResultados()
{
    cuadrantes = new QCPBars(graficoResultados->xAxis, graficoResultados->yAxis);
    graficoResultados->addPlottable(cuadrantes);
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
    graficoResultados->xAxis->setAutoTicks(false);
    graficoResultados->xAxis->setAutoTickLabels(false);
    graficoResultados->xAxis->setTickVector(ticks);
    graficoResultados->xAxis->setTickVectorLabels(labels);
    graficoResultados->xAxis->setTickLabelRotation(60);
    graficoResultados->xAxis->setSubTickCount(0);
    graficoResultados->xAxis->setTickLength(0, 4);
    graficoResultados->xAxis->grid()->setVisible(true);
    graficoResultados->xAxis->setRange(0, 5);

    // prepare y axis:
    graficoResultados->yAxis->setRange(0, 100);
    graficoResultados->yAxis->setPadding(5); // a bit more space to the left border
    graficoResultados->yAxis->setLabel("Porcentaje");
    graficoResultados->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    graficoResultados->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    graficoResultados->yAxis->grid()->setSubGridPen(gridPen);

    // setup legend:
    graficoResultados->legend->setVisible(true);
    graficoResultados->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    graficoResultados->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    graficoResultados->legend->setBorderPen(legendPen);
//    QFont legendFont = font();
//    legendFont.setPointSize(10);
//    graficoResultados->legend->setFont(legendFont);
    graficoResultados->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    //graficoResultados->replot();

}

void Reportes::inicializarGraficoAngulos()
{
    graficoAngulos->plotLayout()->clear();
    graficoAngulos->clearItems();
    graficoAngulos->clearGraphs();

    //Elementos del grafico
    QCPAxisRect *topAxisRect = new QCPAxisRect(graficoAngulos);
    QCPAxisRect *bottomAxisRect = new QCPAxisRect(graficoAngulos);

    bottomAxisRect->axis(QCPAxis::atLeft)->setRange(0,2);
    bottomAxisRect->axis(QCPAxis::atBottom)->setRange(0,2);

    QCPPlotTitle *tituloX=new QCPPlotTitle(graficoAngulos,"Grafico Angulo X vs Tiempo");
    QCPPlotTitle *tituloY=new QCPPlotTitle(graficoAngulos,"Grafico Angulo Y vs Tiempo");

    //Se posicionan los layouts
    graficoAngulos->plotLayout()->addElement(0, 0, tituloX);
    graficoAngulos->plotLayout()->addElement(1, 0, topAxisRect);

    graficoAngulos->plotLayout()->addElement(2, 0, tituloY);
    graficoAngulos->plotLayout()->addElement(3, 0, bottomAxisRect);

     //create and configure plottables:
    graficoAnguloX = graficoAngulos->addGraph(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));
    graficoAnguloY = graficoAngulos->addGraph(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));

    graficoAngulos->addLayer("name", 0 , QCustomPlot::limAbove);

    lineaIzq1=new QCPItemLine(graficoAngulos);
    graficoAngulos->addItem(lineaIzq1);
    lineaIzq1->setClipAxisRect(topAxisRect);
    lineaIzq1->setPen(QPen(Qt::red));
    lineaIzq1->start->setAxes(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));
    lineaIzq1->end->setAxes(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));

    lineaIzq2=new QCPItemLine(graficoAngulos);
    graficoAngulos->addItem(lineaIzq2);
    lineaIzq2->setClipAxisRect(bottomAxisRect);
    lineaIzq2->setPen(QPen(Qt::red));
    lineaIzq2->start->setAxes(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));
    lineaIzq2->end->setAxes(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));

    lineaDer1=new QCPItemLine(graficoAngulos);
    graficoAngulos->addItem(lineaDer1);
    lineaDer1->setClipAxisRect(topAxisRect);
    lineaDer1->setPen(QPen(Qt::red));
    lineaDer1->start->setAxes(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));
    lineaDer1->end->setAxes(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));

    lineaDer2=new QCPItemLine(graficoAngulos);
    graficoAngulos->addItem(lineaDer2);
    lineaDer2->setClipAxisRect(bottomAxisRect);
    lineaDer2->setPen(QPen(Qt::red));
    lineaDer2->start->setAxes(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));
    lineaDer2->end->setAxes(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));

    //Colores de la Line
    graficoAnguloX->setPen(QPen(QColor(71, 71, 194), 2));
    graficoAnguloY->setPen(QPen(QColor(153, 102, 51), 2));

    //Labels de los ejes
    topAxisRect->axis(QCPAxis::atLeft)->setLabel("Angulo (grados)");
    topAxisRect->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");
    bottomAxisRect->axis(QCPAxis::atLeft)->setLabel("Angulo (grados)");
    bottomAxisRect->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");

    //Se rescalan los ejes para el autoajuste
    graficoAnguloX->rescaleAxes();
    graficoAnguloY->rescaleAxes();

    graficoAngulos->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
//    grafico->replot(); //Se redibuja para actualizar para actualizar la vista

}

void Reportes::inicializarGraficoMuestras()
{
    graficoMuestras->plotLayout()->clear(); // let's start from scratch and remove the default axis rect
    graficoMuestras->clearItems();
    graficoMuestras->clearGraphs();

    //Se crean los LAYOUTS principales
    QCPLayoutGrid *layoutAcelerometro = new QCPLayoutGrid;
    QCPLayoutGrid *layoutGiroscopio = new QCPLayoutGrid;

    //Se crean los titulos
    QCPPlotTitle *tituloAcelerometro = new QCPPlotTitle(graficoMuestras, "Gráficos Aceleración X Y Z vs Tiempo");
    QCPPlotTitle *tituloGiroscopio = new QCPPlotTitle(graficoMuestras, "Gráficos Velocidad Angular X Y Z vs Tiempo");

    //Se añaden los layouts principales y los titulos
    graficoMuestras->plotLayout()->addElement(0, 0, tituloAcelerometro);
    graficoMuestras->plotLayout()->addElement(1, 0, layoutAcelerometro);
    graficoMuestras->plotLayout()->addElement(2, 0, tituloGiroscopio);
    graficoMuestras->plotLayout()->addElement(3, 0, layoutGiroscopio);

    // Se crean los sublayouts del acelerometro
    QCPAxisRect *leftAxisRectAcelerometro = new QCPAxisRect(graficoMuestras);
    QCPAxisRect *centerAxisRectAcelerometro = new QCPAxisRect(graficoMuestras);
    QCPAxisRect *rightAxisRectAcelerometro = new QCPAxisRect(graficoMuestras);


    //Se agregan al layout principal
    layoutAcelerometro->addElement(0, 0, leftAxisRectAcelerometro);
    layoutAcelerometro->addElement(0, 1, centerAxisRectAcelerometro);
    layoutAcelerometro->addElement(0, 2, rightAxisRectAcelerometro);

    //Se crean los sublayouts del giroscopio
    QCPAxisRect *leftAxisRectGiroscopio = new QCPAxisRect(graficoMuestras);
    QCPAxisRect *centerAxisRectGiroscopio = new QCPAxisRect(graficoMuestras);
    QCPAxisRect *rightAxisRectGiroscopio = new QCPAxisRect(graficoMuestras);

    //Se agregan al layout principal
    layoutGiroscopio->addElement(0, 0, leftAxisRectGiroscopio);
    layoutGiroscopio->addElement(0, 1, centerAxisRectGiroscopio);
    layoutGiroscopio->addElement(0, 2, rightAxisRectGiroscopio);

    // create and configure plottables: Acelerometro
    graficoAcX = graficoMuestras->addGraph(leftAxisRectAcelerometro->axis(QCPAxis::atBottom), leftAxisRectAcelerometro->axis(QCPAxis::atLeft));
    graficoAcY = graficoMuestras->addGraph(centerAxisRectAcelerometro->axis(QCPAxis::atBottom), centerAxisRectAcelerometro->axis(QCPAxis::atLeft));
    graficoAcZ = graficoMuestras->addGraph(rightAxisRectAcelerometro->axis(QCPAxis::atBottom), rightAxisRectAcelerometro->axis(QCPAxis::atLeft));

    // create and configure plottables: Giroscopio
    graficoGyX = graficoMuestras->addGraph(leftAxisRectGiroscopio->axis(QCPAxis::atBottom), leftAxisRectGiroscopio->axis(QCPAxis::atLeft));
    graficoGyY = graficoMuestras->addGraph(centerAxisRectGiroscopio->axis(QCPAxis::atBottom), centerAxisRectGiroscopio->axis(QCPAxis::atLeft));
    graficoGyZ = graficoMuestras->addGraph(rightAxisRectGiroscopio->axis(QCPAxis::atBottom), rightAxisRectGiroscopio->axis(QCPAxis::atLeft));

    //Estilo del lapiz para la linea
    graficoAcX->setPen(QPen(Qt::blue));
    graficoAcY->setPen(QPen(Qt::blue));
    graficoAcZ->setPen(QPen(Qt::blue));

    graficoGyX->setPen(QPen(Qt::red));
    graficoGyY->setPen(QPen(Qt::red));
    graficoGyZ->setPen(QPen(Qt::red));

//    // rescale axes according to graph's data:
//    graficoAcX->rescaleAxes();
//    graficoAcY->rescaleAxes();
//    graficoAcZ->rescaleAxes();

//    graficoGyX->rescaleAxes();
//    graficoGyY->rescaleAxes();
//    graficoGyZ->rescaleAxes();

    //Labels de los ejes: Acelerometro
    leftAxisRectAcelerometro->axis(QCPAxis::atLeft)->setLabel("Aceleracion Eje X (G)");
    leftAxisRectAcelerometro->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");

    centerAxisRectAcelerometro->axis(QCPAxis::atLeft)->setLabel("Aceleracion Eje Y (G)");
    centerAxisRectAcelerometro->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");

    rightAxisRectAcelerometro->axis(QCPAxis::atLeft)->setLabel("Aceleracion Eje Z (G)");
    rightAxisRectAcelerometro->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");

    //Labels de los ejes: Giroscopio
    leftAxisRectGiroscopio->axis(QCPAxis::atLeft)->setLabel("Velocidad Angular Eje X (º/segundos)");
    leftAxisRectGiroscopio->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");

    centerAxisRectGiroscopio->axis(QCPAxis::atLeft)->setLabel("Velocidad Angular Eje Y (º/segundos)");
    centerAxisRectGiroscopio->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");

    rightAxisRectGiroscopio->axis(QCPAxis::atLeft)->setLabel("Velocaidad Angular Eje Z (º/segundos)");
    rightAxisRectGiroscopio->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");

    //Agregamos interactividad
    graficoMuestras->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    //grafico->replot(); //Se redibuja para actualizar la vista

}

void Reportes::agregarDatosGraficoAngulos(Angulo *angulo)
{
    //Se agregan los datos al grafico de Angulos
    graficoAnguloX->addData(angulo->getTiempo() , angulo->getAnguloX());
    graficoAnguloY->addData(angulo->getTiempo() , angulo->getAnguloY());
}

void Reportes::agregarDatosGraficoMuestras(Raw *datos)
{
    //Se rellenar los datos de los graficos de las Muestras
    graficoAcX->addData(datos->getTiempo(), datos->getAcX());
    graficoAcY->addData(datos->getTiempo(), datos->getAcY());
    graficoAcZ->addData(datos->getTiempo(), datos->getAcZ());

    graficoGyX->addData(datos->getTiempo(), datos->getGyX());
    graficoGyY->addData(datos->getTiempo(), datos->getGyZ());
    graficoGyZ->addData(datos->getTiempo(), datos->getGyZ());

}

void Reportes::graficarResultados(QList<Angulo*> listaAngulos)
{
    // Add data:
    QVector<double> quadrantData;
    QVector<double> ticks;

    double q1=0, q2=0, q3=0, q4=0;

    ticks << 1 << 2 << 3 << 4;

    foreach (Angulo *var, listaAngulos) {//Se recorren las muestras y compara para determinar en que cuadrante estan.
        if(var->getAnguloX()>0){
            if(var->getAnguloY()>0)//Cuadrante 1
                q1+=1;
            else //Cuadrante 4
                q4+=1;
        }
        else{
            if(var->getAnguloY()>0) //Cuadrante 2
                q2+=1;
            else //Cuadrante 3
                q3+=1;
        }
    }

    q1=q1/listaAngulos.size()*100;
    q2=q2/listaAngulos.size()*100;
    q3=q3/listaAngulos.size()*100;
    q4=q4/listaAngulos.size()*100;

    quadrantData  << q1 << q2 << q3 << q4;

    cuadrantes->setData(ticks, quadrantData);
}

void Reportes::moverLineaIzquierdaAngulos(const int newValue)
{
    lineaIzq1->end->setCoords(newValue/100.0,5000);
    lineaIzq1->start->setCoords(newValue/100.0,-5000);
    lineaIzq2->end->setCoords(newValue/100.0,5000);
    lineaIzq2->start->setCoords(newValue/100.0,-5000);
    graficoAngulos->replot();
}

void Reportes::moverLineaDerechaAngulos(const int newValue)
{
    lineaDer1->end->setCoords(newValue/100.0,5000);
    lineaDer1->start->setCoords(newValue/100.0,-5000);
    lineaDer2->end->setCoords(newValue/100.0,5000);
    lineaDer2->start->setCoords(newValue/100.0,-5000);
    graficoAngulos->replot();
}

void Reportes::analizarGraficosAngulos(QWidget *parent, double tiempoPrueba, QList<Angulo *> listaAngulos)
{
    /*
    const int rangoHorizontal=5000;
    QCPItemLine *lineaLowerGraficoAnguloX = new QCPItemLine(graficoAnguloX);
    graficoAnguloX->addItem(lineaLowerGraficoAnguloX);
    lineaLowerGraficoAnguloX->setPen(QPen(Qt::red));
    lineaLowerGraficoAnguloX->start->setCoords(0,-rangoHorizontal);
    lineaLowerGraficoAnguloX->end->setCoords(0,rangoHorizontal);

    QCPItemLine *lineaLowerGraficoAnguloY = new QCPItemLine(graficoAnguloY);
    graficoAnguloY->addItem(lineaLowerGraficoAnguloY);
    lineaLowerGraficoAnguloY->setPen(QPen(Qt::red));
    lineaLowerGraficoAnguloY->start->setCoords(0,-rangoHorizontal);
    lineaLowerGraficoAnguloY->end->setCoords(0,rangoHorizontal);

    QCPItemLine *lineaUpperGraficoAnguloX = new QCPItemLine(graficoAnguloX);
    graficoAnguloX->addItem(lineaUpperGraficoAnguloX);
    lineaUpperGraficoAnguloX->setPen(QPen(Qt::red));
    lineaUpperGraficoAnguloX->start->setCoords(tiempoPrueba,-rangoHorizontal);
    lineaUpperGraficoAnguloX->end->setCoords(tiempoPrueba,rangoHorizontal);

    QCPItemLine *lineaUpperGraficoAnguloY = new QCPItemLine(graficoAnguloY);
    graficoAnguloY->addItem(lineaUpperGraficoAnguloY);
    lineaUpperGraficoAnguloY->setPen(QPen(Qt::red));
    lineaUpperGraficoAnguloY->start->setCoords(tiempoPrueba,-rangoHorizontal);
    lineaUpperGraficoAnguloY->end->setCoords(tiempoPrueba,rangoHorizontal);

    graficoAnguloX->replot();
    graficoAnguloY->replot();

    //Qdialog de ventana de carga configuracion sensores.
    QDialogAnalisis= new QDialog(parent);
    QDialogAnalisis->setWindowFlags(QDialogAnalisis->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QHBoxLayout* layoutBarraCarga = new QHBoxLayout;
    QLabel *labelCarga= new QLabel("SLIDERS!!");
    layoutBarraCarga->addWidget(labelCarga);

    QxtSpanSlider *horizontalSlider = new QxtSpanSlider(Qt::Horizontal);

    horizontalSlider->setHandleMovementMode(QxtSpanSlider::NoOverlapping);
    horizontalSlider->setMaximum(tiempoPrueba*100);
    horizontalSlider->setLowerValue(0);
    horizontalSlider->setLowerPosition(0);
    horizontalSlider->setUpperValue(tiempoPrueba*100);
    horizontalSlider->setUpperPosition(tiempoPrueba*100);

    layoutBarraCarga->addWidget(horizontalSlider);

    QPushButton *QPushButtonAplicarRangos= new QPushButton("Aplicar");
    layoutBarraCarga->addWidget(QPushButtonAplicarRangos);

    QDialogAnalisis->setLayout(layoutBarraCarga);

    connect(QPushButtonAplicarRangos,QPushButton::clicked ,[=](){
        graficoAnguloX->graph(0)->clearData();
        graficoAnguloY->graph(0)->clearData();
        foreach (Angulo *var, listaAngulos) {
            if(var->getTiempo()>=horizontalSlider->lowerValue()/100.0 && var->getTiempo()<=horizontalSlider->upperValue()/100.0){
                graficoAnguloX->graph(0)->addData(var->getTiempo(),var->getAnguloX());
                graficoAnguloY->graph(0)->addData(var->getTiempo(),var->getAnguloY());
            }
        }
        graficoAnguloX->rescaleAxes();
        graficoAnguloY->rescaleAxes();
        graficoAnguloX->replot();
        graficoAnguloY->replot();
    });

    connect(horizontalSlider,QxtSpanSlider::lowerValueChanged, [=](const int &newValue){
        lineaLowerGraficoAnguloX->start->setCoords(newValue/100.0,-rangoHorizontal);
        lineaLowerGraficoAnguloX->end->setCoords(newValue/100.0,rangoHorizontal);
        lineaLowerGraficoAnguloY->start->setCoords(newValue/100.0,-rangoHorizontal);
        lineaLowerGraficoAnguloY->end->setCoords(newValue/100.0,rangoHorizontal);
        graficoAnguloX->replot();
        graficoAnguloY->replot();
    });

    connect(horizontalSlider,QxtSpanSlider::upperValueChanged, [=](const int &newValue){
        lineaUpperGraficoAnguloX->start->setCoords(newValue/100.0,-rangoHorizontal);
        lineaUpperGraficoAnguloX->end->setCoords(newValue/100.0,rangoHorizontal);
        lineaUpperGraficoAnguloY->start->setCoords(newValue/100.0,-rangoHorizontal);
        lineaUpperGraficoAnguloY->end->setCoords(newValue/100.0,rangoHorizontal);
        graficoAnguloX->replot();
        graficoAnguloY->replot();
    });

    connect(QDialogAnalisis,QDialog::rejected, [=](){
        graficoAnguloX->clearItems();
        graficoAnguloY->clearItems();
        graficoAnguloX->replot();
        graficoAnguloY->replot();
    });

    QDialogAnalisis->show();
    */
}

void Reportes::agregarFilaTablaAngulos(Angulo *angulo)
{
    //Se agrega una nueva fila a la tabla
    const int currentRow = tablaAngulos->rowCount();
    tablaAngulos->setRowCount(currentRow + 1);
    tablaAngulos->setItem(currentRow,0,new QTableWidgetItem(QString::number(angulo->getTiempo(),'f',presicion)));
    tablaAngulos->setItem(currentRow,1,new QTableWidgetItem(QString::number(angulo->getAnguloX(),'f',presicion)));
    tablaAngulos->setItem(currentRow,2,new QTableWidgetItem(QString::number(angulo->getAnguloY(),'f',presicion)));
}

void Reportes::agregarFilaTablaMuestras(Raw *datos)
{
    const int currentRow = tablaMuestras->rowCount();
    tablaMuestras->setRowCount(currentRow + 1);
    tablaMuestras->setItem(currentRow,0,new QTableWidgetItem(QString::number(datos->getTiempo(),'f',presicion)));
    tablaMuestras->setItem(currentRow,1,new QTableWidgetItem(QString::number(datos->getAcX(),'f',presicion)));
    tablaMuestras->setItem(currentRow,2,new QTableWidgetItem(QString::number(datos->getAcY(),'f',presicion)));
    tablaMuestras->setItem(currentRow,3,new QTableWidgetItem(QString::number(datos->getAcZ(),'f',presicion)));
    tablaMuestras->setItem(currentRow,4,new QTableWidgetItem(QString::number(datos->getGyX(),'f',presicion)));
    tablaMuestras->setItem(currentRow,5,new QTableWidgetItem(QString::number(datos->getGyY(),'f',presicion)));
    tablaMuestras->setItem(currentRow,6,new QTableWidgetItem(QString::number(datos->getGyZ(),'f',presicion)));
}

void Reportes::guardarImagenGrafico(QCustomPlot *grafico, int ancho, int alto)
{
    QString filters("Imagen PNG (*.png);;Imagen JPG (*.jpg);;Archivo PDF (*.pdf)");
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(0, "Guardar Imagen","",filters,&selectedFilter);

    if(selectedFilter.contains("PNG"))
        grafico->savePng(fileName,ancho,alto);
    if(selectedFilter.contains("JPG"))
       grafico->saveJpg(fileName,ancho,alto);
    if(selectedFilter.contains("PDF"))
      grafico->savePdf(fileName,false,ancho,alto);
}

void Reportes::guardarMuestrasEnArchivo(QList<Raw*> listaMuestras)
{
    QString selectedFilter;
    QString filters(tr("CSV (*.csv);;Archivo de Texto (*.txt)"));
    QString fileName = QFileDialog::getSaveFileName(0, tr("Guardar el Archivo"),"",filters,&selectedFilter);

    if (fileName != ""){
        QFile file(fileName);
        file.remove();
        if (file.open(QIODevice::Append)){
            QTextStream stream(&file);
            foreach (Raw *var, listaMuestras){
                if(selectedFilter.contains("txt")){
                    stream <<"Tiempo: " << QString::number(var->getTiempo(),'f',presicion) << " X: " << QString::number(var->getAcX(),'f',presicion)
                           << " Y: " << QString::number(var->getAcY(),'f',presicion) <<  " Z: " << QString::number(var->getAcZ(),'f',presicion) <<  " X: " << QString::number(var->getGyX(),'f',presicion) << endl;
                }
                if(selectedFilter.contains("csv")){
                    stream <<QString::number(var->getTiempo(),'f',presicion) << ";" <<QString::number(var->getAcX(),'f',presicion)
                           <<";" << QString::number(var->getAcY(),'f',presicion) <<";" << QString::number(var->getAcZ(),'f',presicion)
                           <<";" << QString::number(var->getGyX(),'f',presicion) <<";" << QString::number(var->getGyY(),'f',presicion)
                           <<";" << QString::number(var->getGyZ(),'f',presicion) << endl;
                }
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

void Reportes::guardarAngulosEnArchivo(QList<Angulo*> listaAngulos)
{
    QString selectedFilter;
    QString filters(tr("CSV (*.csv);;Archivo de Texto (*.txt)"));
    QString fileName = QFileDialog::getSaveFileName(0, tr("Guardar el Archivo"),"",filters,&selectedFilter);

    if (fileName != ""){
        QFile file(fileName);
        file.remove();
        if (file.open(QIODevice::Append)){
            QTextStream stream(&file);
            foreach (Angulo *var, listaAngulos){
                if(selectedFilter.contains("txt")){
                    stream <<"Tiempo: " << QString::number(var->getTiempo(),'f',presicion) << " X: " << QString::number(var->getAnguloX(),'f',presicion)
                           << " Y: " << QString::number(var->getAnguloY(),'f',presicion) << endl;
                }
                if(selectedFilter.contains("csv")){
                    stream <<QString::number(var->getTiempo(),'f',presicion) << ";" << QString::number(var->getAnguloX(),'f',presicion)
                           <<";" << QString::number(var->getAnguloY(),'f',presicion) << endl;
                }
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
