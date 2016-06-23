#include "reportes.h"

Reportes::Reportes(QObject *parent) : QObject(parent)
{
    this->presicion = 3;
}

Reportes::Reportes(QObject *parent, QCustomPlot *graficoResultados, QCustomPlot *graficoAngulos, QCustomPlot *graficoDesplazamientoPro,QCustomPlot *graficoDesplazamientoReCur, QCustomPlot *graficoMuestras, QTableWidget *tablaAngulos, QTableWidget *tablaDesplazamientosP, QTableWidget *tablaDesplazamientosRC, QTableWidget *tablaMuestras, QTextEdit *textEditReporte) : QObject(parent)
{
    this->presicion = 4;
    this->graficoResultados = graficoResultados;
    this->graficoAngulos = graficoAngulos;
    this->graficoDesplazamientosProyeccion = graficoDesplazamientoPro;
    this->graficoDesplazamientosRecorridoCurvo = graficoDesplazamientoReCur;
    this->graficoMuestras = graficoMuestras;
    this->tablaAngulos = tablaAngulos;
    this->tablaDesplazamientosProyeccion = tablaDesplazamientosP;
    this->tablaDesplazamientosRecorridoCurvo = tablaDesplazamientosRC;
    this->tablaMuestras = tablaMuestras;
    this->textEditReporte = textEditReporte;
    inicializarGraficoResultados();
    inicializarGraficoAngulos();
    inicializarGraficoDesplazamientosProyeccion();
    inicializarGraficoDesplazamientosRecorridoCurvo();
    inicializarGraficoMuestras();
    inicializarInformeReporte();
}

void Reportes::vaciarTablas()
{
    tablaAngulos->clearContents();
    tablaAngulos->setRowCount(0);
    tablaDesplazamientosProyeccion->clearContents();
    tablaDesplazamientosProyeccion->setRowCount(0);
    tablaDesplazamientosRecorridoCurvo->clearContents();
    tablaDesplazamientosRecorridoCurvo->setRowCount(0);
    tablaMuestras->clearContents();
    tablaMuestras->setRowCount(0);
}

void Reportes::vaciarGraficos()
{
    cuadrantes->clearData();

    vaciarGraficoAngulos();
    vaciarGraficoMuestras();
    vaciarGraficosDesplazamientoProyeccion();
    vaciarGraficosDesplazamientoRecorridoCurvo();
}

void Reportes::vaciarGraficoAngulos()
{
    graficoAnguloX->clearData();
    graficoAnguloY->clearData();
}

void Reportes::vaciarGraficosDesplazamientoProyeccion()
{
    graficoDesplazamientoProX->clearData();
    graficoDesplazamientoProY->clearData();
}

void Reportes::vaciarGraficosDesplazamientoRecorridoCurvo()
{
    graficoDesplazamientoReCurX->clearData();
    graficoDesplazamientoReCurY->clearData();
}

void Reportes::vaciarGraficoMuestras()
{
    graficoAcX->clearData();
    graficoAcY->clearData();
    graficoAcZ->clearData();
    graficoGyX->clearData();
    graficoGyY->clearData();
    graficoGyZ->clearData();
}

void Reportes::vaciarInformeReporte()
{
    textEditReporte->clear();
    inicializarInformeReporte();
}

void Reportes::agregarDatosInformeReporteImagen(const QString busq,const QImage imagen)
{
    textEditReporte->find(busq);
    textEditReporte->textCursor().insertImage(imagen);
}

void Reportes::agregarDatosInformeReportePlainText(const QString busq, const QString dato)
{
    textEditReporte->find(busq);
    textEditReporte->insertPlainText(dato);
}

void Reportes::agregarDatosInformeReporteHTML(const QString busq, const QString dato)
{
    textEditReporte->find(busq);
    textEditReporte->insertHtml(dato);
}

void Reportes::replotGraficoAngulos()
{
    graficoAnguloX->rescaleAxes();
    graficoAnguloY->rescaleAxes();
    graficoAngulos->replot();
}

void Reportes::replotGraficoDesplazamientoProyeccion()
{
    graficoDesplazamientoProX->rescaleAxes();
    graficoDesplazamientoProY->rescaleAxes();
    graficoDesplazamientosProyeccion->replot();
}

void Reportes::replotGraficoDesplazamientoRecorridoCurvo()
{
    graficoDesplazamientoReCurX->rescaleAxes();
    graficoDesplazamientoReCurY->rescaleAxes();
    graficoDesplazamientosRecorridoCurvo->replot();
}

void Reportes::replotGraficoMuestras()
{
    graficoAcX->rescaleAxes();
    graficoAcY->rescaleAxes();
    graficoAcZ->rescaleAxes();
    graficoGyX->rescaleAxes();
    graficoGyY->rescaleAxes();
    graficoGyZ->rescaleAxes();
    graficoMuestras->replot();
}

void Reportes::inicializarInformeReporte()
{
    QFile file(":/reporte/reporte.htm");
    file.open(QFile::ReadOnly);
    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str))
        textEditReporte->setHtml(data);
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

    QCPPlotTitle *tituloX=new QCPPlotTitle(graficoAngulos,"Grafico Angulo Medio-Lateral vs Tiempo");
    QCPPlotTitle *tituloY=new QCPPlotTitle(graficoAngulos,"Grafico Angulo Antero-Posterior vs Tiempo");

    //Se posicionan los layouts
    graficoAngulos->plotLayout()->addElement(0, 0, tituloX);
    graficoAngulos->plotLayout()->addElement(1, 0, topAxisRect);

    graficoAngulos->plotLayout()->addElement(2, 0, tituloY);
    graficoAngulos->plotLayout()->addElement(3, 0, bottomAxisRect);

     //create and configure plottables:
    graficoAnguloX = graficoAngulos->addGraph(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));
    graficoAnguloY = graficoAngulos->addGraph(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));

    lineaIzqAnguloX=new QCPItemLine(graficoAngulos);
    agregarQCPItemLine(lineaIzqAnguloX,graficoAngulos,topAxisRect);

    lineaDerAnguloX=new QCPItemLine(graficoAngulos);
    agregarQCPItemLine(lineaDerAnguloX,graficoAngulos,topAxisRect);

    lineaIzqAnguloY=new QCPItemLine(graficoAngulos);
    agregarQCPItemLine(lineaIzqAnguloY,graficoAngulos,bottomAxisRect);

    lineaDerAnguloY=new QCPItemLine(graficoAngulos);
    agregarQCPItemLine(lineaDerAnguloY,graficoAngulos,bottomAxisRect);

    //Colores de la Linea
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
}

void Reportes::inicializarGraficoDesplazamientosProyeccion()
{
    graficoDesplazamientosProyeccion->plotLayout()->clear();
    graficoDesplazamientosProyeccion->clearItems();
    graficoDesplazamientosProyeccion->clearGraphs();

    //Elementos del grafico
    QCPAxisRect *topAxisRect = new QCPAxisRect(graficoDesplazamientosProyeccion);
    QCPAxisRect *bottomAxisRect = new QCPAxisRect(graficoDesplazamientosProyeccion);

    bottomAxisRect->axis(QCPAxis::atLeft)->setRange(0,2);
    bottomAxisRect->axis(QCPAxis::atBottom)->setRange(0,2);

    QCPPlotTitle *tituloX=new QCPPlotTitle(graficoDesplazamientosProyeccion,"Grafico Desplazamiento Medio-Lateral vs Tiempo");
    QCPPlotTitle *tituloY=new QCPPlotTitle(graficoDesplazamientosProyeccion,"Grafico Desplazamiento Antero-Posterior vs Tiempo");

    //Se posicionan los layouts
    graficoDesplazamientosProyeccion->plotLayout()->addElement(0, 0, tituloX);
    graficoDesplazamientosProyeccion->plotLayout()->addElement(1, 0, topAxisRect);

    graficoDesplazamientosProyeccion->plotLayout()->addElement(2, 0, tituloY);
    graficoDesplazamientosProyeccion->plotLayout()->addElement(3, 0, bottomAxisRect);

     //create and configure plottables:
    graficoDesplazamientoProX = graficoDesplazamientosProyeccion->addGraph(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));
    graficoDesplazamientoProY = graficoDesplazamientosProyeccion->addGraph(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));

    //agregarQCPItemLine(lineaIzqDesplazamientoX,graficoDesplazamientos,topAxisRect);

    lineaIzqDesplazamientoProX=new QCPItemLine(graficoDesplazamientosProyeccion);
    agregarQCPItemLine(lineaIzqDesplazamientoProX,graficoDesplazamientosProyeccion,topAxisRect);

    lineaDerDesplazamientoProX=new QCPItemLine(graficoDesplazamientosProyeccion);
    agregarQCPItemLine(lineaDerDesplazamientoProX,graficoDesplazamientosProyeccion,topAxisRect);

    lineaIzqDesplazamientoProY=new QCPItemLine(graficoDesplazamientosProyeccion);
    agregarQCPItemLine(lineaIzqDesplazamientoProY,graficoDesplazamientosProyeccion,bottomAxisRect);

    lineaDerDesplazamientoProY=new QCPItemLine(graficoDesplazamientosProyeccion);
    agregarQCPItemLine(lineaDerDesplazamientoProY,graficoDesplazamientosProyeccion,bottomAxisRect);

    //Colores de la Line
    graficoDesplazamientoProX->setPen(QPen(QColor(71, 71, 194), 2));
    graficoDesplazamientoProY->setPen(QPen(QColor(153, 102, 51), 2));

    //Labels de los ejes
    topAxisRect->axis(QCPAxis::atLeft)->setLabel("Desplazamiento (centimetros)");
    topAxisRect->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");
    bottomAxisRect->axis(QCPAxis::atLeft)->setLabel("Desplazamiento (centimetros)");
    bottomAxisRect->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");

    //Se rescalan los ejes para el autoajuste
    graficoDesplazamientoProX->rescaleAxes();
    graficoDesplazamientoProY->rescaleAxes();

    graficoDesplazamientosProyeccion->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
}

void Reportes::inicializarGraficoDesplazamientosRecorridoCurvo()
{
    graficoDesplazamientosRecorridoCurvo->plotLayout()->clear();
    graficoDesplazamientosRecorridoCurvo->clearItems();
    graficoDesplazamientosRecorridoCurvo->clearGraphs();

    //Elementos del grafico
    QCPAxisRect *topAxisRect = new QCPAxisRect(graficoDesplazamientosRecorridoCurvo);
    QCPAxisRect *bottomAxisRect = new QCPAxisRect(graficoDesplazamientosRecorridoCurvo);

    bottomAxisRect->axis(QCPAxis::atLeft)->setRange(0,2);
    bottomAxisRect->axis(QCPAxis::atBottom)->setRange(0,2);

    QCPPlotTitle *tituloX=new QCPPlotTitle(graficoDesplazamientosRecorridoCurvo,"Grafico Desplazamiento Medio-Lateral vs Tiempo");
    QCPPlotTitle *tituloY=new QCPPlotTitle(graficoDesplazamientosRecorridoCurvo,"Grafico Desplazamiento Antero-Posterior vs Tiempo");

    //Se posicionan los layouts
    graficoDesplazamientosRecorridoCurvo->plotLayout()->addElement(0, 0, tituloX);
    graficoDesplazamientosRecorridoCurvo->plotLayout()->addElement(1, 0, topAxisRect);

    graficoDesplazamientosRecorridoCurvo->plotLayout()->addElement(2, 0, tituloY);
    graficoDesplazamientosRecorridoCurvo->plotLayout()->addElement(3, 0, bottomAxisRect);

     //create and configure plottables:
    graficoDesplazamientoReCurX = graficoDesplazamientosRecorridoCurvo->addGraph(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));
    graficoDesplazamientoReCurY = graficoDesplazamientosRecorridoCurvo->addGraph(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));

    //agregarQCPItemLine(lineaIzqDesplazamientoX,graficoDesplazamientos,topAxisRect);

    lineaIzqDesplazamientoReCurX=new QCPItemLine(graficoDesplazamientosRecorridoCurvo);
    agregarQCPItemLine(lineaIzqDesplazamientoReCurX,graficoDesplazamientosRecorridoCurvo,topAxisRect);

    lineaDerDesplazamientoReCurX=new QCPItemLine(graficoDesplazamientosRecorridoCurvo);
    agregarQCPItemLine(lineaDerDesplazamientoReCurX,graficoDesplazamientosRecorridoCurvo,topAxisRect);

    lineaIzqDesplazamientoReCurY=new QCPItemLine(graficoDesplazamientosRecorridoCurvo);
    agregarQCPItemLine(lineaIzqDesplazamientoReCurY,graficoDesplazamientosRecorridoCurvo,bottomAxisRect);

    lineaDerDesplazamientoReCurY=new QCPItemLine(graficoDesplazamientosRecorridoCurvo);
    agregarQCPItemLine(lineaDerDesplazamientoReCurY,graficoDesplazamientosRecorridoCurvo,bottomAxisRect);

    //Colores de la Line
    graficoDesplazamientoReCurX->setPen(QPen(QColor(71, 71, 194), 2));
    graficoDesplazamientoReCurY->setPen(QPen(QColor(153, 102, 51), 2));

    //Labels de los ejes
    topAxisRect->axis(QCPAxis::atLeft)->setLabel("Desplazamiento (centimetros)");
    topAxisRect->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");
    bottomAxisRect->axis(QCPAxis::atLeft)->setLabel("Desplazamiento (centimetros)");
    bottomAxisRect->axis(QCPAxis::atBottom)->setLabel("Tiempo (segundos)");

    //Se rescalan los ejes para el autoajuste
    graficoDesplazamientoReCurX->rescaleAxes();
    graficoDesplazamientoReCurY->rescaleAxes();

    graficoDesplazamientosRecorridoCurvo->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
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

    // Lineas para el analisis
    lineaIzqAcX=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaIzqAcX,graficoMuestras,leftAxisRectAcelerometro);
    lineaDerAcX=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaDerAcX,graficoMuestras,leftAxisRectAcelerometro);
    lineaIzqAcY=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaIzqAcY,graficoMuestras,centerAxisRectAcelerometro);
    lineaDerAcY=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaDerAcY,graficoMuestras,centerAxisRectAcelerometro);
    lineaIzqAcZ=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaIzqAcZ,graficoMuestras,rightAxisRectAcelerometro);
    lineaDerAcZ=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaDerAcZ,graficoMuestras,rightAxisRectAcelerometro);

    lineaIzqGyX=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaIzqGyX,graficoMuestras,leftAxisRectGiroscopio);
    lineaDerGyX=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaDerGyX,graficoMuestras,leftAxisRectGiroscopio);
    lineaIzqGyY=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaIzqGyY,graficoMuestras,centerAxisRectGiroscopio);
    lineaDerGyY=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaDerGyY,graficoMuestras,centerAxisRectGiroscopio);
    lineaIzqGyZ=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaIzqGyZ,graficoMuestras,rightAxisRectGiroscopio);
    lineaDerGyZ=new QCPItemLine(graficoMuestras);
    agregarQCPItemLine(lineaDerGyZ,graficoMuestras,rightAxisRectGiroscopio);

    //Estilo del lapiz para la linea
    graficoAcX->setPen(QPen(Qt::blue));
    graficoAcY->setPen(QPen(Qt::blue));
    graficoAcZ->setPen(QPen(Qt::blue));

    graficoGyX->setPen(QPen(QColor(153, 102, 51), 2));
    graficoGyY->setPen(QPen(QColor(153, 102, 51), 2));
    graficoGyZ->setPen(QPen(QColor(153, 102, 51), 2));

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
    graficoAnguloX->addData(angulo->getTiempo() , angulo->getAngulo1());
    graficoAnguloY->addData(angulo->getTiempo() , angulo->getAngulo2());
}

void Reportes::agregarDatosGraficoDesplazamientoProyeccion(Desplazamiento *desp)
{
    //Se agregan los datos al grafico Desplazamiento Proyeccion
    graficoDesplazamientoProX->addData(desp->getTiempo() , desp->getDesplazamientoProyeccion().Desplazamiento1);
    graficoDesplazamientoProY->addData(desp->getTiempo() , desp->getDesplazamientoProyeccion().Desplazamiento2);
}

void Reportes::agregarDatosGraficoDesplazamientoRecorridoCurvo(Desplazamiento *desp)
{
    //Se agregan los datos al grafico Desplazamiento Recorrido Curvo
    graficoDesplazamientoReCurX->addData(desp->getTiempo() , desp->getDesplazamientoRecorridoCurvo().Desplazamiento1);
    graficoDesplazamientoReCurY->addData(desp->getTiempo() , desp->getDesplazamientoRecorridoCurvo().Desplazamiento2);
}

void Reportes::agregarDatosGraficoMuestras(Muestra *datos)
{
    //Se rellenar los datos de los graficos de las Muestras
    graficoAcX->addData(datos->getTiempo(), datos->getAcX());
    graficoAcY->addData(datos->getTiempo(), datos->getAcY());
    graficoAcZ->addData(datos->getTiempo(), datos->getAcZ());

    graficoGyX->addData(datos->getTiempo(), datos->getGyX());
    graficoGyY->addData(datos->getTiempo(), datos->getGyY());
    graficoGyZ->addData(datos->getTiempo(), datos->getGyZ());
}

void Reportes::setDatosGraficoAngulos(QVector<Angulo*> listaAngulos)
{
    const int size=listaAngulos.size();
    QVector<double> tiempo(size);
    QVector<double> angX(size);
    QVector<double> angY(size);
    for (int var = 0; var < size; ++var) {
        Angulo *angulo=listaAngulos.at(var);
        tiempo[var]=angulo->getTiempo();
        angX[var]=angulo->getAngulo1();
        angY[var]=angulo->getAngulo2();
    }
    this->vaciarGraficoAngulos();
    graficoAnguloX->setData(tiempo,angX);
    graficoAnguloY->addData(tiempo,angY);
    this->replotGraficoAngulos();
}

void Reportes::setDatosGraficoDezplazamiento(QVector<Desplazamiento*> listaDesplazamientos)
{
    const int size=listaDesplazamientos.size();
    QVector<double> tiempo(size);
    QVector<double> despX(size);
    QVector<double> despY(size);
    for (int var = 0; var < size; ++var) {
        Desplazamiento *desp=listaDesplazamientos.at(var);
        tiempo[var]=desp->getTiempo();
        despX[var]=desp->getDesplazamientoProyeccion().Desplazamiento1;
        despY[var]=desp->getDesplazamientoProyeccion().Desplazamiento2;
    }
    this->vaciarGraficosDesplazamientoProyeccion();
    graficoDesplazamientoProX->setData(tiempo,despX);
    graficoDesplazamientoProY->addData(tiempo,despY);
    this->replotGraficoDesplazamientoProyeccion();
}

void Reportes::setDatosGraficoMuestras(QVector<Muestra*> listaMuestras)
{
    const int size=listaMuestras.size();
    QVector<double> tiempo(size);
    QVector<double> AcX(size);
    QVector<double> AcY(size);
    QVector<double> AcZ(size);
    QVector<double> GyX(size);
    QVector<double> GyY(size);
    QVector<double> GyZ(size);
    for (int var = 0; var < size; ++var) {
        Muestra *muestra=listaMuestras.at(var);
        tiempo[var]=muestra->getTiempo();
        AcX[var]=muestra->getAcX();
        AcY[var]=muestra->getAcY();
        AcZ[var]=muestra->getAcZ();
        GyX[var]=muestra->getGyX();
        GyY[var]=muestra->getGyY();
        GyZ[var]=muestra->getGyZ();
    }
    this->vaciarGraficoMuestras();
    graficoAcX->setData(tiempo,AcX);
    graficoAcY->setData(tiempo,AcY);
    graficoAcZ->setData(tiempo,AcZ);
    graficoGyX->setData(tiempo,GyX);
    graficoGyY->setData(tiempo,GyY);
    graficoGyZ->setData(tiempo,GyZ);
    this->replotGraficoMuestras();
}

void Reportes::graficarResultados(QVector<Angulo*> listaAngulos)
{
    // Add data:
    QVector<double> quadrantData;
    QVector<double> ticks;

    double q1=0, q2=0, q3=0, q4=0;

    ticks << 1 << 2 << 3 << 4;

    foreach (Angulo *var, listaAngulos) {//Se recorren las muestras y compara para determinar en que cuadrante estan.
        if(var->getAngulo1()>0){
            if(var->getAngulo2()>0)//Cuadrante 1
                q1+=1;
            else //Cuadrante 4
                q4+=1;
        }
        else{
            if(var->getAngulo2()>0) //Cuadrante 2
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

void Reportes::agregarQCPItemLine(QCPItemLine *linea,QCustomPlot *grafico,QCPAxisRect *contenedor)
{
    grafico->addItem(linea);
    linea->setClipAxisRect(contenedor);
    linea->setPen(QPen(Qt::red));
    linea->start->setAxes(contenedor->axis(QCPAxis::atBottom), contenedor->axis(QCPAxis::atLeft));
    linea->end->setAxes(contenedor->axis(QCPAxis::atBottom), contenedor->axis(QCPAxis::atLeft));
}

void Reportes::moverLineasIzquierdaAngulos(const double newValue)
{
    lineaIzqAnguloX->start->setCoords(newValue,-5000);
    lineaIzqAnguloX->end->setCoords(newValue,5000);
    lineaIzqAnguloY->start->setCoords(newValue,-5000);
    lineaIzqAnguloY->end->setCoords(newValue,5000);
    graficoAngulos->replot();
}

void Reportes::moverLineasDerechaAngulos(const double newValue)
{
    lineaDerAnguloX->start->setCoords(newValue,-5000);
    lineaDerAnguloX->end->setCoords(newValue,5000);
    lineaDerAnguloY->start->setCoords(newValue,-5000);
    lineaDerAnguloY->end->setCoords(newValue,5000);
    graficoAngulos->replot();
}

void Reportes::moverLineasIzquierdaDesplazamientosProyeccion(const double newValue)
{
    lineaIzqDesplazamientoProX->start->setCoords(newValue,-5000);
    lineaIzqDesplazamientoProX->end->setCoords(newValue,5000);
    lineaIzqDesplazamientoProY->start->setCoords(newValue,-5000);
    lineaIzqDesplazamientoProY->end->setCoords(newValue,5000);
    graficoDesplazamientosProyeccion->replot();
}

void Reportes::moverLineasDerechaDesplazamientosProyeccion(const double newValue)
{
    lineaDerDesplazamientoProX->start->setCoords(newValue,-5000);
    lineaDerDesplazamientoProX->end->setCoords(newValue,5000);
    lineaDerDesplazamientoProY->start->setCoords(newValue,-5000);
    lineaDerDesplazamientoProY->end->setCoords(newValue,5000);
    graficoDesplazamientosProyeccion->replot();
}

void Reportes::moverLineasIzquierdaDesplazamientosRecorridoCurvo(const double newValue)
{
    lineaIzqDesplazamientoReCurX->start->setCoords(newValue,-5000);
    lineaIzqDesplazamientoReCurX->end->setCoords(newValue,5000);
    lineaIzqDesplazamientoReCurY->start->setCoords(newValue,-5000);
    lineaIzqDesplazamientoReCurY->end->setCoords(newValue,5000);
    graficoDesplazamientosRecorridoCurvo->replot();

}

void Reportes::moverLineasDerechaDesplazamientosRecorridoCurvo(const double newValue)
{
    lineaDerDesplazamientoReCurX->start->setCoords(newValue,-5000);
    lineaDerDesplazamientoReCurX->end->setCoords(newValue,5000);
    lineaDerDesplazamientoReCurY->start->setCoords(newValue,-5000);
    lineaDerDesplazamientoReCurY->end->setCoords(newValue,5000);
    graficoDesplazamientosRecorridoCurvo->replot();
}

void Reportes::moverLineasIzquierdaMuestras(const double newValue)
{
    lineaIzqAcX->start->setCoords(newValue,-5000);
    lineaIzqAcX->end->setCoords(newValue,5000);
    lineaIzqAcY->start->setCoords(newValue,-5000);
    lineaIzqAcY->end->setCoords(newValue,5000);
    lineaIzqAcZ->start->setCoords(newValue,-5000);
    lineaIzqAcZ->end->setCoords(newValue,5000);
    lineaIzqGyX->start->setCoords(newValue,-5000);
    lineaIzqGyX->end->setCoords(newValue,5000);
    lineaIzqGyY->start->setCoords(newValue,-5000);
    lineaIzqGyY->end->setCoords(newValue,5000);
    lineaIzqGyZ->start->setCoords(newValue,-5000);
    lineaIzqGyZ->end->setCoords(newValue,5000);
    graficoMuestras->replot();
}

void Reportes::moverLineasDerechaMuestras(const double newValue)
{
    lineaDerAcX->start->setCoords(newValue,-5000);
    lineaDerAcX->end->setCoords(newValue,5000);
    lineaDerAcY->start->setCoords(newValue,-5000);
    lineaDerAcY->end->setCoords(newValue,5000);
    lineaDerAcZ->start->setCoords(newValue,-5000);
    lineaDerAcZ->end->setCoords(newValue,5000);
    lineaDerGyX->start->setCoords(newValue,-5000);
    lineaDerGyX->end->setCoords(newValue,5000);
    lineaDerGyY->start->setCoords(newValue,-5000);
    lineaDerGyY->end->setCoords(newValue,5000);
    lineaDerGyZ->start->setCoords(newValue,-5000);
    lineaDerGyZ->end->setCoords(newValue,5000);
    graficoMuestras->replot();
}

//Para Agregar Datos fila a fila a las Tablas

void Reportes::agregarFilaTablaAngulos(Angulo *angulo)
{
    //Se agrega una nueva fila a la tabla
    const int currentRow = tablaAngulos->rowCount();
    tablaAngulos->setRowCount(currentRow + 1);
    tablaAngulos->setItem(currentRow,0,new QTableWidgetItem(QString::number(angulo->getTiempo(),'f',presicion)));
    tablaAngulos->setItem(currentRow,1,new QTableWidgetItem(QString::number(angulo->getAngulo1(),'f',presicion)));
    tablaAngulos->setItem(currentRow,2,new QTableWidgetItem(QString::number(angulo->getAngulo2(),'f',presicion)));
}

void Reportes::agregarFilaTablasDesplazamientos(Desplazamiento *desp)
{
    const int currentRow = tablaDesplazamientosProyeccion->rowCount();
    tablaDesplazamientosProyeccion->setRowCount(currentRow + 1);
    tablaDesplazamientosRecorridoCurvo->setRowCount(currentRow + 1);

    tablaDesplazamientosProyeccion->setItem(currentRow,0,new QTableWidgetItem(QString::number(desp->getTiempo(),'f',presicion)));
    tablaDesplazamientosProyeccion->setItem(currentRow,1,new QTableWidgetItem(QString::number(desp->getDesplazamientoProyeccion().Desplazamiento1,'f',presicion)));
    tablaDesplazamientosProyeccion->setItem(currentRow,2,new QTableWidgetItem(QString::number(desp->getDesplazamientoProyeccion().Desplazamiento2,'f',presicion)));

    tablaDesplazamientosRecorridoCurvo->setItem(currentRow,0,new QTableWidgetItem(QString::number(desp->getTiempo(),'f',presicion)));
    tablaDesplazamientosRecorridoCurvo->setItem(currentRow,1,new QTableWidgetItem(QString::number(desp->getDesplazamientoRecorridoCurvo().Desplazamiento1,'f',presicion)));
    tablaDesplazamientosRecorridoCurvo->setItem(currentRow,2,new QTableWidgetItem(QString::number(desp->getDesplazamientoRecorridoCurvo().Desplazamiento2,'f',presicion)));
}

void Reportes::agregarFilaTablaMuestras(Muestra *datos)
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

//Para Agregar todas las filas de la Tabla

void Reportes::setDatosTablaAngulos(QVector<Angulo*> listaAngulos)
{
    //Se agrega una nueva fila a la tabla
    tablaAngulos->setRowCount(listaAngulos.size());
    for(int i = 0; i < listaAngulos.size(); ++i){
        Angulo *angulo=listaAngulos.at(i);
        tablaAngulos->setItem(i,0,new QTableWidgetItem(QString::number(angulo->getTiempo(),'f',presicion)));
        tablaAngulos->setItem(i,1,new QTableWidgetItem(QString::number(angulo->getAngulo1(),'f',presicion)));
        tablaAngulos->setItem(i,2,new QTableWidgetItem(QString::number(angulo->getAngulo2(),'f',presicion)));
    }
}

void Reportes::setDatosTablasDesplazamientos(QVector<Desplazamiento*> listaDesplazamientos){
    tablaDesplazamientosProyeccion->setRowCount(listaDesplazamientos.size());
    tablaDesplazamientosRecorridoCurvo->setRowCount(listaDesplazamientos.size());
    for(int i = 0; i < listaDesplazamientos.size(); ++i){
        Desplazamiento *desp=listaDesplazamientos.at(i);
        tablaDesplazamientosProyeccion->setItem(i,0,new QTableWidgetItem(QString::number(desp->getTiempo(),'f',presicion)));
        tablaDesplazamientosProyeccion->setItem(i,1,new QTableWidgetItem(QString::number(desp->getDesplazamientoProyeccion().Desplazamiento1,'f',presicion)));
        tablaDesplazamientosProyeccion->setItem(i,2,new QTableWidgetItem(QString::number(desp->getDesplazamientoProyeccion().Desplazamiento2,'f',presicion)));

        tablaDesplazamientosRecorridoCurvo->setItem(i,0,new QTableWidgetItem(QString::number(desp->getTiempo(),'f',presicion)));
        tablaDesplazamientosRecorridoCurvo->setItem(i,1,new QTableWidgetItem(QString::number(desp->getDesplazamientoRecorridoCurvo().Desplazamiento1,'f',presicion)));
        tablaDesplazamientosRecorridoCurvo->setItem(i,2,new QTableWidgetItem(QString::number(desp->getDesplazamientoRecorridoCurvo().Desplazamiento2,'f',presicion)));
    }
}

void Reportes::setDatosTablaMuestras(QVector<Muestra*> listaMuestras){

    tablaMuestras->setRowCount(listaMuestras.size());
    for (int var = 0; var < listaMuestras.size(); ++var) {
        Muestra *muestra=listaMuestras.at(var);
        tablaMuestras->setItem(var,0,new QTableWidgetItem(QString::number(muestra->getTiempo(),'f',presicion)));
        tablaMuestras->setItem(var,1,new QTableWidgetItem(QString::number(muestra->getAcX(),'f',presicion)));
        tablaMuestras->setItem(var,2,new QTableWidgetItem(QString::number(muestra->getAcY(),'f',presicion)));
        tablaMuestras->setItem(var,3,new QTableWidgetItem(QString::number(muestra->getAcZ(),'f',presicion)));
        tablaMuestras->setItem(var,4,new QTableWidgetItem(QString::number(muestra->getGyX(),'f',presicion)));
        tablaMuestras->setItem(var,5,new QTableWidgetItem(QString::number(muestra->getGyY(),'f',presicion)));
        tablaMuestras->setItem(var,6,new QTableWidgetItem(QString::number(muestra->getGyZ(),'f',presicion)));
    }
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

void Reportes::guardarAngulosEnArchivo(QVector<Angulo*> listaAngulos)
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
                    /*stream <<"Tiempo: " << QString::number(var->getTiempo()) << " X: " << QString::number(var->getAnguloX())
                           << " Y: " << QString::number(var->getAnguloY()) << endl;*/
                    stream <<QString::number(var->getTiempo(),'f',presicion) << " " << QString::number(var->getAngulo1())
                           <<" " << QString::number(var->getAngulo2()) << endl;
                }
                if(selectedFilter.contains("csv")){
                    stream <<QString::number(var->getTiempo(),'f',presicion) << "," << QString::number(var->getAngulo1())
                           <<"," << QString::number(var->getAngulo2()) << endl;
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

void Reportes::guardarDesplazamientosEnArchivo(QVector<Desplazamiento*> listaDesplazamientos,QString tipo)
{
    QString selectedFilter;
    QString filters(tr("CSV (*.csv);;Archivo de Texto (*.txt)"));
    QString fileName = QFileDialog::getSaveFileName(0, tr("Guardar el Archivo"),"",filters,&selectedFilter);

    if (fileName != ""){
        QFile file(fileName);
        file.remove();
        if (file.open(QIODevice::Append)){
            QTextStream stream(&file);
            foreach (Desplazamiento *var, listaDesplazamientos){
                if(selectedFilter.contains("txt")){
                    if(tipo.contains("proyeccion"))
                        stream <<"Tiempo: " << QString::number(var->getTiempo()) << " X: " << QString::number(var->getDesplazamientoProyeccion().Desplazamiento1)
                           << " Y: " << QString::number(var->getDesplazamientoProyeccion().Desplazamiento2) << endl;
                    if(tipo.contains("curvo"))
                        stream <<"Tiempo: " << QString::number(var->getTiempo()) << " X: " << QString::number(var->getDesplazamientoRecorridoCurvo().Desplazamiento1)
                           << " Y: " << QString::number(var->getDesplazamientoRecorridoCurvo().Desplazamiento2) << endl;
                }
                if(selectedFilter.contains("csv")){
                    if(tipo.contains("proyeccion"))
                        stream <<QString::number(var->getTiempo(),'f',presicion) << "," << QString::number(var->getDesplazamientoProyeccion().Desplazamiento1)
                           <<"," << QString::number(var->getDesplazamientoProyeccion().Desplazamiento2) << endl;
                    if(tipo.contains("curvo"))
                        stream <<QString::number(var->getTiempo(),'f',presicion) << "," << QString::number(var->getDesplazamientoRecorridoCurvo().Desplazamiento1)
                           <<"," << QString::number(var->getDesplazamientoRecorridoCurvo().Desplazamiento2) << endl;
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

void Reportes::guardarMuestrasEnArchivo(QVector<Muestra*> listaMuestras)
{
    QString selectedFilter;
    QString filters(tr("CSV (*.csv);;Archivo de Texto (*.txt)"));
    QString fileName = QFileDialog::getSaveFileName(0, tr("Guardar el Archivo"),"",filters,&selectedFilter);

    if (fileName != ""){
        QFile file(fileName);
        file.remove();
        if (file.open(QIODevice::Append)){
            QTextStream stream(&file);
            foreach (Muestra *var, listaMuestras){
                if(selectedFilter.contains("txt")){
                    stream <<"Tiempo: " << QString::number(var->getTiempo()) << " X: " << QString::number(var->getAcX())
                           << " Y: " << QString::number(var->getAcY()) <<  " Z: " << QString::number(var->getAcZ()) <<  " X: " << QString::number(var->getGyX(),'f',presicion) << endl;
                }
                if(selectedFilter.contains("csv")){
                    stream <<QString::number(var->getTiempo()) << "," <<QString::number(var->getAcX())
                           <<"," << QString::number(var->getAcY()) <<"," << QString::number(var->getAcZ())
                           <<"," << QString::number(var->getGyX()) <<"," << QString::number(var->getGyY())
                           <<"," << QString::number(var->getGyZ()) << endl;
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

void Reportes::guardarInformeReportePDF()
{
    QString filename = QFileDialog::getSaveFileName(0,"Guardar Reporte","","Archivo pdf(*.pdf)");
    if(!filename.isEmpty()) {
        if(QFileInfo(filename).suffix().isEmpty())
            filename.append(".pdf");

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFileName(filename);
        textEditReporte->print(&printer);
    }
}
