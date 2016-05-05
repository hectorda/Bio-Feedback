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

    q1=q1/listaAngulos.size()*100;
    q2=q2/listaAngulos.size()*100;
    q3=q3/listaAngulos.size()*100;
    q4=q4/listaAngulos.size()*100;
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
//    QFont legendFont = font();
//    legendFont.setPointSize(10);
//    grafico->legend->setFont(legendFont);
    grafico->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    grafico->replot();
}

void Reportes::graficarMuestras(QCustomPlot *grafico, QList<Raw *> listaMuestras)
{

    // configure axis rect:
    grafico->plotLayout()->clear(); // clear default axis rect so we can start from scratch
    QCPAxisRect *wideAxisRect = new QCPAxisRect(grafico);
    wideAxisRect->setupFullAxesBox(true);
    wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabels(true);
    wideAxisRect->addAxis(QCPAxis::atLeft)->setTickLabelColor(QColor("#6050F8")); // add an extra axis on the left and color its numbers
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    grafico->plotLayout()->addElement(0, 0, wideAxisRect); // insert axis rect in first row
    grafico->plotLayout()->addElement(1, 0, subLayout); // sub layout in second row (grid layout will grow accordingly)
    //customPlot->plotLayout()->setRowStretchFactor(1, 2);
    // prepare axis rects that will be placed in the sublayout:
    QCPAxisRect *subRectLeft = new QCPAxisRect(grafico, false); // false means to not setup default axes
    QCPAxisRect *subRectRight = new QCPAxisRect(grafico, false);
    subLayout->addElement(0, 0, subRectLeft);
    subLayout->addElement(0, 1, subRectRight);
    subRectRight->setMaximumSize(150, 150); // make bottom right axis rect size fixed 150x150
    subRectRight->setMinimumSize(150, 150); // make bottom right axis rect size fixed 150x150
    // setup axes in sub layout axis rects:
    subRectLeft->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
    subRectRight->addAxes(QCPAxis::atBottom | QCPAxis::atRight);
    subRectLeft->axis(QCPAxis::atLeft)->setAutoTickCount(2);
    subRectRight->axis(QCPAxis::atRight)->setAutoTickCount(2);
    subRectRight->axis(QCPAxis::atBottom)->setAutoTickCount(2);
    subRectLeft->axis(QCPAxis::atBottom)->grid()->setVisible(true);
    // synchronize the left and right margins of the top and bottom axis rects:
    QCPMarginGroup *marginGroup = new QCPMarginGroup(grafico);
    subRectLeft->setMarginGroup(QCP::msLeft, marginGroup);
    subRectRight->setMarginGroup(QCP::msRight, marginGroup);
    wideAxisRect->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);
    // move newly created axes on "axes" layer and grids on "grid" layer:
    foreach (QCPAxisRect *rect, grafico->axisRects())
    {
      foreach (QCPAxis *axis, rect->axes())
      {
        axis->setLayer("axes");
        axis->grid()->setLayer("grid");
      }
    }

    // prepare data:
    QVector<double> x1a(20), y1a(20);
    QVector<double> x1b(50), y1b(50);
    QVector<double> x2(100), y2(100);
    QVector<double> x3, y3;
    qsrand(3);
    for (int i=0; i<x1a.size(); ++i)
    {
      x1a[i] = i/(double)(x1a.size()-1)*10-5.0;
      y1a[i] = qCos(x1a[i]);
    }
    for (int i=0; i<x1b.size(); ++i)
    {
      x1b[i] = i/(double)x1b.size()*10-5.0;
      y1b[i] = qExp(-x1b[i]*x1b[i]*0.2)*1000;
    }
    for (int i=0; i<x2.size(); ++i)
    {
      x2[i] = i/(double)x2.size()*10;
      y2[i] = qrand()/(double)RAND_MAX-0.5+y2[qAbs(i-1)];
    }
    x3 << 1 << 2 << 3 << 4;
    y3 << 2 << 2.5 << 4 << 1.5;

    // create and configure plottables:
    QCPGraph *mainGraph1 = grafico->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
    mainGraph1->setData(x1a, y1a);
    mainGraph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black), QBrush(Qt::white), 6));
    mainGraph1->setPen(QPen(QColor(120, 120, 120), 2));
    QCPGraph *mainGraph2 = grafico->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft, 1));
    mainGraph2->setData(x1b, y1b);
    mainGraph2->setPen(QPen(QColor("#8070B8"), 2));
    mainGraph2->setBrush(QColor(110, 170, 110, 30));
    mainGraph1->setChannelFillGraph(mainGraph2);
    mainGraph1->setBrush(QColor(255, 161, 0, 50));

    QCPGraph *graph2 = grafico->addGraph(subRectLeft->axis(QCPAxis::atBottom), subRectLeft->axis(QCPAxis::atLeft));
    graph2->setData(x2, y2);
    graph2->setLineStyle(QCPGraph::lsImpulse);
    graph2->setPen(QPen(QColor("#FFA100"), 1.5));

    QCPBars *bars1 = new QCPBars(subRectRight->axis(QCPAxis::atBottom), subRectRight->axis(QCPAxis::atRight));
    grafico->addPlottable(bars1);
    bars1->setWidth(3/(double)x3.size());
    bars1->setData(x3, y3);
    bars1->setPen(QPen(Qt::black));
    bars1->setAntialiased(false);
    bars1->setAntialiasedFill(false);
    bars1->setBrush(QColor("#705BE8"));
    bars1->keyAxis()->setAutoTicks(false);
    bars1->keyAxis()->setTickVector(x3);
    bars1->keyAxis()->setSubTickCount(0);

    // rescale axes according to graph's data:
    mainGraph1->rescaleAxes();
    mainGraph2->rescaleAxes();
    graph2->rescaleAxes();
    bars1->rescaleAxes();
    wideAxisRect->axis(QCPAxis::atLeft, 1)->setRangeLower(0);
    /*
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
    limpiarGrafico(grafico);
    grafico->addGraph();
    grafico->graph(0)->setData(Tiempo, DatosAcX);
    grafico->xAxis->setLabel("Tiempo");
    grafico->yAxis->setLabel("Aceleracion X");
    grafico->xAxis->setRange(0, Tiempo.last());
    grafico->yAxis->setRange(-1, 1);
    grafico->replot();
    grafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

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
    */
}

void Reportes::graficarAngulos(QCustomPlot *grafico, QList<Angulo *> listaAngulos)
{
    QVector<double> Tiempo;
    QVector<double> DatosAnguloX;
    QVector<double> DatosAnguloY;
    foreach (Angulo *var, listaAngulos) {
        Tiempo.append(var->getTiempo());
        DatosAnguloX.append(var->getAnguloX());
        DatosAnguloY.append(var->getAnguloY());
    }
    limpiarGrafico(grafico);
    grafico->addGraph();
    grafico->graph(0)->setData(Tiempo, DatosAnguloX);

    grafico->addGraph();
    grafico->graph(1)->setData(Tiempo, DatosAnguloY);
    grafico->graph(1)->setPen(QPen(Qt::red));

    grafico->xAxis->setLabel("Tiempo");
    grafico->yAxis->setLabel("Angulos vs Tiempo");

    grafico->xAxis->setRange(0, Tiempo.last());
    grafico->rescaleAxes();
    grafico->replot();
    grafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
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

