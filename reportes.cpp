#include "reportes.h"

//class hiloGraficarMuestras :public QThread {
//    QCustomPlot *grafico;
//    QList<Raw*> listaMuestras;
//    Reportes *reporte;


//public:
//    hiloGraficarMuestras(){

//    }

//    hiloGraficarMuestras(QCustomPlot *grafico,QList<Raw*> listaMuestras)
//    {
//        this->grafico=grafico;
//        this->listaMuestras=listaMuestras;
//    }

//    void run()override
//    {

//    }
//};


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

void Reportes::graficarResultados(QCustomPlot *grafico, QList<Angulo*> listaAngulos)
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

void Reportes::graficarMuestras(QCustomPlot *grafico, QList<Raw*> listaMuestras)
{
    grafico->plotLayout()->clear(); // let's start from scratch and remove the default axis rect
    grafico->clearItems();
    grafico->clearGraphs();

    //Se crean los LAYOUTS principales
    QCPLayoutGrid *layoutAcelerometro = new QCPLayoutGrid;
    QCPLayoutGrid *layoutGiroscopio = new QCPLayoutGrid;

    //Se crean los titulos
    QCPPlotTitle *tituloAcelerometro = new QCPPlotTitle(grafico, "Gráficos Aceleración X Y Z vs Tiempo");
    QCPPlotTitle *tituloGiroscopio = new QCPPlotTitle(grafico, "Gráficos Velocidad Angular X Y Z vs Tiempo");

    //Se añaden los layouts principales y los titulos
    grafico->plotLayout()->addElement(0, 0, tituloAcelerometro);
    grafico->plotLayout()->addElement(1, 0, layoutAcelerometro);
    grafico->plotLayout()->addElement(2, 0, tituloGiroscopio);
    grafico->plotLayout()->addElement(3, 0, layoutGiroscopio);

    // Se crean los sublayouts del acelerometro
    QCPAxisRect *leftAxisRectAcelerometro = new QCPAxisRect(grafico);
    QCPAxisRect *centerAxisRectAcelerometro = new QCPAxisRect(grafico);
    QCPAxisRect *rightAxisRectAcelerometro = new QCPAxisRect(grafico);


    //Se agregan al layout principal
    layoutAcelerometro->addElement(0, 0, leftAxisRectAcelerometro);
    layoutAcelerometro->addElement(0, 1, centerAxisRectAcelerometro);
    layoutAcelerometro->addElement(0, 2, rightAxisRectAcelerometro);

    //Se crean los sublayouts del giroscopio
    QCPAxisRect *leftAxisRectGiroscopio = new QCPAxisRect(grafico);
    QCPAxisRect *centerAxisRectGiroscopio = new QCPAxisRect(grafico);
    QCPAxisRect *rightAxisRectGiroscopio = new QCPAxisRect(grafico);

    //Se agregan al layout principal
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

    //Se rellenar los datos de los graficos
    foreach (Raw *var, listaMuestras) {
        graficoAcX->addData(var->getTiempo(), var->getAcX());
        graficoAcY->addData(var->getTiempo(), var->getAcY());
        graficoAcZ->addData(var->getTiempo(), var->getAcZ());

        graficoGyX->addData(var->getTiempo(), var->getGyX());
        graficoGyY->addData(var->getTiempo(), var->getGyZ());
        graficoGyZ->addData(var->getTiempo(), var->getGyZ());
    }

    //Estilo del lapiz para la linea
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
    grafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    grafico->replot(); //Se redibuja para actualizar la vista
}

void Reportes::graficarAngulos(QCustomPlot *grafico, QList<Angulo*> listaAngulos)
{
    grafico->plotLayout()->clear();
    grafico->clearItems();
    grafico->clearGraphs();

    //Elementos del grafico
    QCPAxisRect *topAxisRect = new QCPAxisRect(grafico);
    QCPAxisRect *bottomAxisRect = new QCPAxisRect(grafico);

    QCPPlotTitle *tituloX=new QCPPlotTitle(grafico,"Grafico Angulo X vs Tiempo");
    QCPPlotTitle *tituloY=new QCPPlotTitle(grafico,"Grafico Angulo Y vs Tiempo");

    //Se posicionan los layouts
    grafico->plotLayout()->addElement(0, 0, tituloX);
    grafico->plotLayout()->addElement(1, 0, topAxisRect);

    grafico->plotLayout()->addElement(2, 0, tituloY);
    grafico->plotLayout()->addElement(3, 0, bottomAxisRect);

    // create and configure plottables:
    QCPGraph *graficoAnguloX = grafico->addGraph(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));
    QCPGraph *graficoAnguloY = grafico->addGraph(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));

    //Se agregan los datos al grafico
    foreach (Angulo *var, listaAngulos) {
        graficoAnguloX->addData(var->getTiempo() , var->getAnguloX());
        graficoAnguloY->addData(var->getTiempo() , var->getAnguloY());
    }

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

    grafico->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    grafico->replot(); //Se redibuja para actualizar para actualizar la vista

}

void Reportes::tablaMuestras(QTableWidget *tabla, QList<Raw*> listaMuestras)
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

void Reportes::tablaAngulos(QTableWidget *tabla, QList<Angulo*> listaAngulos)
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
                    stream <<"Tiempo: " << QString::number(var->getTiempo(),'f',3) << " X: " << QString::number(var->getAnguloX(),'f',3)
                           << " Y: " << QString::number(var->getAnguloY(),'f',3) << endl;
                }
                if(selectedFilter.contains("csv")){
                    stream <<QString::number(var->getTiempo(),'f',3) << ";" << QString::number(var->getAnguloX(),'f',3)
                           <<";" << QString::number(var->getAnguloY(),'f',3) << endl;
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

