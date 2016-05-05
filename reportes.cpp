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
/*
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
    grafico->yAxis->setRange(-20, 20);
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

