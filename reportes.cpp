#include "reportes.h"

Reportes::Reportes(QObject *parent) : QObject(parent)
{

}

void Reportes::limpiarGrafico(QCustomPlot *grafico){

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

void Reportes::limpiarTabla(QTableWidget *tabla)
{
    while (tabla->rowCount() > 0)
    {
        tabla->removeRow(0);
    }
}

void Reportes::graficarResultados(QCustomPlot *grafico, QList<Angulo *> listaAngulos)
{
    limpiarGrafico(grafico);
    double q1=0, q2=0, q3=0, q4=0;

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
//    QFont legendFont = font();
//    legendFont.setPointSize(10);
//    grafico->legend->setFont(legendFont);
    grafico->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    grafico->replot();
}

void Reportes::graficarMuestras(QCustomPlot *grafico, QList<Raw *> listaMuestras)
{

    grafico->plotLayout()->clear(); // let's start from scratch and remove the default axis rect

    // create a sub layout that we'll place in first row:

    QCPLayoutGrid *layoutAcelerometro = new QCPLayoutGrid;

    grafico->plotLayout()->addElement(0, 0, layoutAcelerometro);
    // add two axis rects in the sub layout next to each other:
    QCPAxisRect *leftAxisRectAcelerometro = new QCPAxisRect(grafico);
    QCPAxisRect *centerAxisRectAcelerometro = new QCPAxisRect(grafico);
    QCPAxisRect *rightAxisRectAcelerometro = new QCPAxisRect(grafico);
    layoutAcelerometro->addElement(0, 0, leftAxisRectAcelerometro);
    layoutAcelerometro->addElement(0, 1, centerAxisRectAcelerometro);
    layoutAcelerometro->addElement(0, 2, rightAxisRectAcelerometro);

    QCPLayoutGrid *layoutGiroscopio = new QCPLayoutGrid;
    grafico->plotLayout()->addElement(1, 0 , layoutGiroscopio);

    QCPAxisRect *leftAxisRectGiroscopio = new QCPAxisRect(grafico);
    QCPAxisRect *centerAxisRectGiroscopio = new QCPAxisRect(grafico);
    QCPAxisRect *rightAxisRectGiroscopio = new QCPAxisRect(grafico);
    layoutGiroscopio->addElement(0, 0, leftAxisRectGiroscopio);
    layoutGiroscopio->addElement(0, 1, centerAxisRectGiroscopio);
    layoutGiroscopio->addElement(0, 2, rightAxisRectGiroscopio);

    // create and configure plottables: Acelerometro
    QCPGraph *graficoAcX = grafico->addGraph(leftAxisRectAcelerometro->axis(QCPAxis::atBottom), leftAxisRectAcelerometro->axis(QCPAxis::atLeft));
    QCPGraph *graficoAcY = grafico->addGraph(centerAxisRectAcelerometro->axis(QCPAxis::atBottom), centerAxisRectAcelerometro->axis(QCPAxis::atLeft));
    QCPGraph *graficoAcZ = grafico->addGraph(rightAxisRectAcelerometro->axis(QCPAxis::atBottom), rightAxisRectAcelerometro->axis(QCPAxis::atLeft));

    // create and configure plottables: Giroscopio
    QCPGraph *graficoGyX = grafico->addGraph(leftAxisRectGiroscopio->axis(QCPAxis::atBottom), leftAxisRectGiroscopio->axis(QCPAxis::atLeft));
    QCPGraph *graficoGyY = grafico->addGraph(centerAxisRectGiroscopio->axis(QCPAxis::atBottom), centerAxisRectGiroscopio->axis(QCPAxis::atLeft));
    QCPGraph *graficoGyZ = grafico->addGraph(rightAxisRectGiroscopio->axis(QCPAxis::atBottom), rightAxisRectGiroscopio->axis(QCPAxis::atLeft));


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

    graficoAcX->setData(Tiempo,DatosAcX);
    graficoAcY->setData(Tiempo,DatosAcY);
    graficoAcZ->setData(Tiempo,DatosAcZ);

    graficoGyX->setData(Tiempo,DatosGyX);
    graficoGyY->setData(Tiempo,DatosGyY);
    graficoGyZ->setData(Tiempo,DatosGyZ);

    graficoAcX->setPen(QPen(Qt::blue));
    graficoAcY->setPen(QPen(Qt::blue));
    graficoAcZ->setPen(QPen(Qt::blue));

    graficoGyX->setPen(QPen(Qt::red));
    graficoGyY->setPen(QPen(Qt::red));
    graficoGyZ->setPen(QPen(Qt::red));

    // rescale axes according to graph's data:
    graficoAcX->rescaleAxes();
    graficoAcY->rescaleAxes();
    graficoAcZ->rescaleAxes();

    graficoGyX->rescaleAxes();
    graficoGyY->rescaleAxes();
    graficoGyZ->rescaleAxes();

    grafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

}

void Reportes::graficarAngulos(QCustomPlot *grafico, QList<Angulo *> listaAngulos)
{
    grafico->plotLayout()->clear();

    QCPAxisRect *topAxisRect = new QCPAxisRect(grafico);
    grafico->plotLayout()->addElement(0, 0, topAxisRect);

    QCPAxisRect *bottomAxisRect = new QCPAxisRect(grafico);
    grafico->plotLayout()->addElement(1, 0, bottomAxisRect);

    // create and configure plottables:
    QCPGraph *graficoAnguloX = grafico->addGraph(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));
    QCPGraph *graficoAnguloY = grafico->addGraph(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));


    foreach (Angulo *var, listaAngulos){
        graficoAnguloX->addData(var->getTiempo(),  var->getAnguloX());
        graficoAnguloY->addData(var->getTiempo() , var->getAnguloY());
    }

    graficoAnguloX->setPen(QPen(QColor(71, 71, 194), 2));
    graficoAnguloY->setPen(QPen(QColor(153, 102, 51), 2));
    //graficoAnguloY->setBrush(QColor(110, 170, 110, 30));
    //graficoAnguloX->setChannelFillGraph(graficoAnguloX);
    //graficoAnguloX->setBrush(QColor(255, 161, 0, 50));


    // rescale axes according to graph's data:
    graficoAnguloX->rescaleAxes();
    graficoAnguloY->rescaleAxes();

    grafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    /*
    limpiarGrafico(grafico);
    grafico->addGraph();
    grafico->graph(0)->setPen(QPen(Qt::blue));

    grafico->addGraph();
    grafico->graph(1)->setPen(QPen(Qt::red));

    grafico->xAxis->setLabel("Tiempo");
    grafico->yAxis->setLabel("Angulos vs Tiempo");


    */
}

void Reportes::tablaMuestras(QTableWidget *tabla, QList<Raw *> listaMuestras)
{
    limpiarTabla(tabla);
    for (int var = 0; var < listaMuestras.size(); ++var) {
        const int currentRow = tabla->rowCount();
        tabla->setRowCount(currentRow + 1);
        tabla->setItem(var,0,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getTiempo())));
        tabla->setItem(var,1,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getAcX())));
        tabla->setItem(var,2,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getAcY())));
        tabla->setItem(var,3,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getAcZ())));
        tabla->setItem(var,4,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getGyX())));
        tabla->setItem(var,5,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getGyY())));
        tabla->setItem(var,6,new QTableWidgetItem(QString::number(listaMuestras.at(var)->getGyZ())));
    }
}

void Reportes::tablaAngulos(QTableWidget *tabla, QList<Angulo *> listaAngulos)
{
    limpiarTabla(tabla);

    for (int var = 0; var < listaAngulos.size(); ++var) {
        const int currentRow = tabla->rowCount();
        tabla->setRowCount(currentRow + 1);
        tabla->setItem(var,0,new QTableWidgetItem(QString::number(listaAngulos.at(var)->getTiempo())));
        tabla->setItem(var,1,new QTableWidgetItem(QString::number(listaAngulos.at(var)->getAnguloX())));
        tabla->setItem(var,2,new QTableWidgetItem(QString::number(listaAngulos.at(var)->getAnguloY())));
    }
}

