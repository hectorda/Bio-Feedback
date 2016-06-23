#include "analisisgrafico.h"
#include "ui_analisisgrafico.h"
#include <QTextStream>

AnalisisGrafico::AnalisisGrafico(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalisisGrafico)
{
    ui->setupUi(this);
}

AnalisisGrafico::AnalisisGrafico(QWidget *parent,Reportes *reportes):
    QDialog(parent),
    ui(new Ui::AnalisisGrafico)
{
    ui->setupUi(this);
    this->reportes=reportes;
    ui->horizontalSlider->setHandleMovementMode(QxtSpanSlider::NoOverlapping);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->tableWidgetEstadisticos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetEstadisticos->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

AnalisisGrafico::~AnalisisGrafico()
{
    delete ui;
}

void AnalisisGrafico::setListaAngulos(QVector<Angulo *> LA)
{
    this->listaAngulos=LA;

    if(this->listaAngulos.isEmpty())
        QTextStream stdout<<"No hay datos de Angulos a analizar"<<endl;

    else
    {
        ui->tableWidgetEstadisticos->setColumnCount(8);
        QStringList headers;
        headers <<"Parámetro"<<"min"<< "@tiempo"<<"max"<<"@tiempo"<<"Media"<<"Desv Est"<<"Rango";
        ui->tableWidgetEstadisticos->setHorizontalHeaderLabels(headers);

        this->setWindowTitle("Analisis Lista Angulos");
        const int muestras=listaAngulos.size()-1;
        ui->horizontalSlider->setMaximum(muestras);

        connect(ui->horizontalSlider,QxtSpanSlider::lowerValueChanged, [=](const int &newValue){
            const double tiempo=listaAngulos.at(newValue)->getTiempo();
            reportes->moverLineasIzquierdaAngulos(tiempo);
            calcularEstadisticosAngulos(newValue,ui->horizontalSlider->upperValue());
            contarDatosAngulos(newValue,ui->horizontalSlider->upperValue());
            ui->labelRangoInf->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->horizontalSlider,QxtSpanSlider::upperValueChanged, [=](const int &newValue){
            const double tiempo=listaAngulos.at(newValue)->getTiempo();
            reportes->moverLineasDerechaAngulos(tiempo);
            calcularEstadisticosAngulos(ui->horizontalSlider->lowerValue(),newValue);
            contarDatosAngulos(ui->horizontalSlider->lowerValue(),newValue);
            ui->labelRangoSup->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->pushButtonRestaurar,QPushButton::clicked,[=](){
            reportes->moverLineasIzquierdaAngulos(0);
            reportes->moverLineasDerechaAngulos(listaAngulos.last()->getTiempo());
            ui->horizontalSlider->setLowerValue(0);
            ui->horizontalSlider->setUpperPosition(listaAngulos.size()-1);
            ajustarRangosGraficoAngulos(0,listaAngulos.size()-1);
        });

        connect(ui->pushButtonAplicarRango,QPushButton::clicked,[=](){
            const int inicio=ui->horizontalSlider->lowerValue();
            const int fin=ui->horizontalSlider->upperValue();
            ajustarRangosGraficoAngulos(inicio,fin);
        });

        connect(ui->pushButtonRescalar,QPushButton::clicked,[=](){
            reportes->replotGraficoAngulos();
        });

        ui->horizontalSlider->setLowerValue(0);
        ui->horizontalSlider->setLowerPosition(0);
        ui->horizontalSlider->lowerValueChanged(0); //para que se añada automaticamente el slider izquierdo
        ui->horizontalSlider->setUpperValue(muestras);
        ui->horizontalSlider->setUpperPosition(muestras);
    }
}

void AnalisisGrafico::setListaDesplazamientosProyeccion(QVector<Desplazamiento *> LD)
{
    this->listaDesplazamientos=LD;

    if(this->listaDesplazamientos.isEmpty())
        QTextStream stdout<<"No hay datos de Desplazamientos a analizar"<<endl;

    else
    {
        ui->tableWidgetEstadisticos->setColumnCount(9);
        QStringList headers;
        headers <<"Parámetro"<<"min"<< "@tiempo"<<"max"<<"@tiempo"<<"Media"<<"Desv Est"<<"Rango"<<"Vel Media";
        ui->tableWidgetEstadisticos->setHorizontalHeaderLabels(headers);

        this->setWindowTitle("Analisis Desplazamientos Calculado Proyeccion");
        const int muestras=listaDesplazamientos.size()-1;
        ui->horizontalSlider->setMaximum(muestras);

        connect(ui->horizontalSlider,QxtSpanSlider::lowerValueChanged, [=](const int &newValue){
            const double tiempo=listaDesplazamientos.at(newValue)->getTiempo();
            reportes->moverLineasIzquierdaDesplazamientosProyeccion(tiempo);
            calcularEstadisticosDesplazamientoProyeccion(newValue,ui->horizontalSlider->upperValue());
            contarDatosDesplazamientoProyeccion(newValue,ui->horizontalSlider->upperValue());
            ui->labelRangoInf->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->horizontalSlider,QxtSpanSlider::upperValueChanged, [=](const int &newValue){
            const double tiempo=listaDesplazamientos.at(newValue)->getTiempo();
            reportes->moverLineasDerechaDesplazamientosProyeccion(tiempo);
            calcularEstadisticosDesplazamientoProyeccion(ui->horizontalSlider->lowerValue(),newValue);
            contarDatosDesplazamientoProyeccion(ui->horizontalSlider->lowerValue(),newValue);
            ui->labelRangoSup->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->pushButtonRestaurar,QPushButton::clicked,[=](){
            reportes->moverLineasIzquierdaDesplazamientosProyeccion(0);
            reportes->moverLineasDerechaDesplazamientosProyeccion(listaDesplazamientos.last()->getTiempo());
            ui->horizontalSlider->setLowerValue(0);
            ui->horizontalSlider->setUpperPosition(listaDesplazamientos.size()-1);
            ajustarRangosGraficoDesplazamientoProyeccion(0,listaDesplazamientos.size()-1);
        });

        connect(ui->pushButtonAplicarRango,QPushButton::clicked,[=](){
            const int inicio=ui->horizontalSlider->lowerValue();
            const int fin=ui->horizontalSlider->upperValue();
            ajustarRangosGraficoDesplazamientoProyeccion(inicio,fin);
        });

        connect(ui->pushButtonRescalar,QPushButton::clicked,[=](){
            reportes->replotGraficoDesplazamientoProyeccion();
        });

        ui->horizontalSlider->setLowerValue(0);
        ui->horizontalSlider->setLowerPosition(0);
        ui->horizontalSlider->lowerValueChanged(0); //para que se añada automaticamente el slider izquierdo
        ui->horizontalSlider->setUpperValue(muestras);
        ui->horizontalSlider->setUpperPosition(muestras);
    }
}

void AnalisisGrafico::setListaDesplazamientosRecorridoCurvo(QVector<Desplazamiento *> LD)
{
    this->listaDesplazamientos=LD;

    if(this->listaDesplazamientos.isEmpty())
        QTextStream stdout<<"No hay datos de Desplazamientos a analizar"<<endl;

    else
    {
        ui->tableWidgetEstadisticos->setColumnCount(9);
        QStringList headers;
        headers <<"Parámetro"<<"min"<< "@tiempo"<<"max"<<"@tiempo"<<"Media"<<"Desv Est"<<"Rango"<<"Vel Media";
        ui->tableWidgetEstadisticos->setHorizontalHeaderLabels(headers);

        this->setWindowTitle("Analisis Desplazamientos Calculado Recorrido Curvo");
        const int muestras=listaDesplazamientos.size()-1;
        ui->horizontalSlider->setMaximum(muestras);

        connect(ui->horizontalSlider,QxtSpanSlider::lowerValueChanged, [=](const int &newValue){
            const double tiempo=listaDesplazamientos.at(newValue)->getTiempo();
            reportes->moverLineasIzquierdaDesplazamientosRecorridoCurvo(tiempo);
            calcularEstadisticosDesplazamientoRecorridoCurvo(newValue,ui->horizontalSlider->upperValue());
            contarDatosDesplazamientoRecorridoCurvo(newValue,ui->horizontalSlider->upperValue());
            ui->labelRangoInf->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->horizontalSlider,QxtSpanSlider::upperValueChanged, [=](const int &newValue){
            const double tiempo=listaDesplazamientos.at(newValue)->getTiempo();
            reportes->moverLineasDerechaDesplazamientosRecorridoCurvo(tiempo);
            calcularEstadisticosDesplazamientoRecorridoCurvo(ui->horizontalSlider->lowerValue(),newValue);
            contarDatosDesplazamientoRecorridoCurvo(ui->horizontalSlider->lowerValue(),newValue);
            ui->labelRangoSup->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->pushButtonRestaurar,QPushButton::clicked,[=](){
            reportes->moverLineasIzquierdaDesplazamientosRecorridoCurvo(0);
            reportes->moverLineasDerechaDesplazamientosRecorridoCurvo(listaDesplazamientos.last()->getTiempo());
            ui->horizontalSlider->setLowerValue(0);
            ui->horizontalSlider->setUpperPosition(listaDesplazamientos.size()-1);
            ajustarRangosGraficoDesplazamientoRecorridoCurvo(0,listaDesplazamientos.size()-1);
        });

        connect(ui->pushButtonAplicarRango,QPushButton::clicked,[=](){
            const int inicio=ui->horizontalSlider->lowerValue();
            const int fin=ui->horizontalSlider->upperValue();
            ajustarRangosGraficoDesplazamientoRecorridoCurvo(inicio,fin);
        });

        connect(ui->pushButtonRescalar,QPushButton::clicked,[=](){
            reportes->replotGraficoDesplazamientoRecorridoCurvo();
        });

        ui->horizontalSlider->setLowerValue(0);
        ui->horizontalSlider->setLowerPosition(0);
        ui->horizontalSlider->lowerValueChanged(0); //para que se añada automaticamente el slider izquierdo
        ui->horizontalSlider->setUpperValue(muestras);
        ui->horizontalSlider->setUpperPosition(muestras);
    }
}


void AnalisisGrafico::setListaMuestras(QVector<Muestra *> LR)
{
    this->listaMuestras=LR;

    if(this->listaMuestras.isEmpty())
        QTextStream stdout<<"No hay datos de Muestras a analizar"<<endl;

    else
    {
        this->setWindowTitle("Analisis Lista Muestras");
        ui->tableWidgetEstadisticos->setColumnCount(8);
        QStringList headers;
        headers <<"Parámetro"<<"min"<< "@tiempo"<<"max"<<"@tiempo"<<"Media"<<"Desv Est"<<"Rango";
        ui->tableWidgetEstadisticos->setHorizontalHeaderLabels(headers);
        const int muestras=listaMuestras.size()-1;
        ui->horizontalSlider->setMaximum(muestras);

        connect(ui->horizontalSlider,QxtSpanSlider::lowerValueChanged, [=](const int &newValue){
            const double tiempo=listaMuestras.at(newValue)->getTiempo();
            reportes->moverLineasIzquierdaMuestras(tiempo);
            calcularEstadisticosMuestras(newValue,ui->horizontalSlider->upperValue());
            contarDatosMuestras(newValue,ui->horizontalSlider->upperValue());
            ui->labelRangoInf->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->horizontalSlider,QxtSpanSlider::upperValueChanged, [=](const int &newValue){
            const double tiempo=listaMuestras.at(newValue)->getTiempo();
            reportes->moverLineasDerechaMuestras(tiempo);
            calcularEstadisticosMuestras(ui->horizontalSlider->lowerValue(),newValue);
            contarDatosMuestras(ui->horizontalSlider->lowerValue(),newValue);
            ui->labelRangoSup->setText(QString::number(tiempo,'f',3)+" seg");
        });

        connect(ui->pushButtonRestaurar,QPushButton::clicked,[=](){
            reportes->moverLineasIzquierdaMuestras(0);
            reportes->moverLineasDerechaMuestras(listaMuestras.last()->getTiempo());
            ui->horizontalSlider->setLowerValue(0);
            ui->horizontalSlider->setUpperPosition(listaMuestras.size()-1);
            ajustarRangosGraficoMuestras(0,listaMuestras.size()-1);

        });

        connect(ui->pushButtonAplicarRango,QPushButton::clicked,[=](){
            const int inicio=ui->horizontalSlider->lowerValue();
            const int fin=ui->horizontalSlider->upperValue();
            ajustarRangosGraficoMuestras(inicio,fin);
        });

        connect(ui->pushButtonRescalar,QPushButton::clicked,[=](){
            reportes->replotGraficoMuestras();
        });

        ui->horizontalSlider->setLowerValue(0);
        ui->horizontalSlider->setLowerPosition(0);
        ui->horizontalSlider->lowerValueChanged(0); //para que se añada automaticamente el slider izquierdo
        ui->horizontalSlider->setUpperValue(muestras);
        ui->horizontalSlider->setUpperPosition(muestras);
    }
}

void AnalisisGrafico::ajustarRangosGraficoAngulos(const int inicio, const int fin)
{
    reportes->vaciarGraficoAngulos();
    for (int var = inicio; var <= fin; ++var)
        reportes->agregarDatosGraficoAngulos(listaAngulos.at(var));

    reportes->replotGraficoAngulos();
}

void AnalisisGrafico::ajustarRangosGraficoDesplazamientoProyeccion(const int inicio, const int fin)
{
    reportes->vaciarGraficosDesplazamientoProyeccion();
    for (int var = inicio; var <= fin; ++var)
        reportes->agregarDatosGraficoDesplazamientoProyeccion(listaDesplazamientos.at(var));

    reportes->replotGraficoDesplazamientoProyeccion();
}

void AnalisisGrafico::ajustarRangosGraficoDesplazamientoRecorridoCurvo(const int inicio, const int fin)
{
    reportes->vaciarGraficosDesplazamientoRecorridoCurvo();
    for (int var = inicio; var <= fin; ++var)
        reportes->agregarDatosGraficoDesplazamientoRecorridoCurvo(listaDesplazamientos.at(var));

    reportes->replotGraficoDesplazamientoRecorridoCurvo();
}

void AnalisisGrafico::ajustarRangosGraficoMuestras(const int inicio, const int fin)
{
    reportes->vaciarGraficoMuestras();
    for (int var = inicio; var <= fin; ++var)
        reportes->agregarDatosGraficoMuestras(listaMuestras.at(var));

    reportes->replotGraficoMuestras();
}

void AnalisisGrafico::contarDatosAngulos(const int inicio, const int fin){
    const int datosTotales=listaAngulos.size();
    const int datosAnalizados=fin+1-inicio;
    double frecuenciaMuestreo=(listaAngulos.at(fin)->getTiempo()-listaAngulos.at(inicio)->getTiempo());
    frecuenciaMuestreo/=(fin-inicio);
    frecuenciaMuestreo=1/frecuenciaMuestreo;
    const double tiempoTotal=listaAngulos.last()->getTiempo();
    const double tiempoAnalizado=listaAngulos.at(fin)->getTiempo()-listaAngulos.at(inicio)->getTiempo();

    MostrarCantidadDatos(datosTotales,datosAnalizados,frecuenciaMuestreo,tiempoTotal,tiempoAnalizado);
}

void AnalisisGrafico::contarDatosDesplazamientoProyeccion(const int inicio, const int fin){
    const int datosTotales=listaDesplazamientos.size();
    const int datosAnalizados=fin+1-inicio;
    double frecuenciaMuestreo=(listaDesplazamientos.at(fin)->getTiempo()-listaDesplazamientos.at(inicio)->getTiempo());
    frecuenciaMuestreo/=(fin-inicio);
    frecuenciaMuestreo=1/frecuenciaMuestreo;
    const double tiempoTotal=listaDesplazamientos.last()->getTiempo();
    const double tiempoAnalizado=listaDesplazamientos.at(fin)->getTiempo()-listaDesplazamientos.at(inicio)->getTiempo();

    MostrarCantidadDatos(datosTotales,datosAnalizados,frecuenciaMuestreo,tiempoTotal,tiempoAnalizado);
}

void AnalisisGrafico::contarDatosDesplazamientoRecorridoCurvo(const int inicio, const int fin){
    const int datosTotales=listaDesplazamientos.size();
    const int datosAnalizados=fin+1-inicio;
    double frecuenciaMuestreo=(listaDesplazamientos.at(fin)->getTiempo()-listaDesplazamientos.at(inicio)->getTiempo());
    frecuenciaMuestreo/=(fin-inicio);
    frecuenciaMuestreo=1/frecuenciaMuestreo;
    const double tiempoTotal=listaDesplazamientos.last()->getTiempo();
    const double tiempoAnalizado=listaDesplazamientos.at(fin)->getTiempo()-listaDesplazamientos.at(inicio)->getTiempo();

    MostrarCantidadDatos(datosTotales,datosAnalizados,frecuenciaMuestreo,tiempoTotal,tiempoAnalizado);

}

void AnalisisGrafico::contarDatosMuestras(const int inicio, const int fin){
    const int datosTotales=listaMuestras.size();
    const int datosAnalizados=fin+1-inicio;
    double frecuenciaMuestreo=(listaMuestras.at(fin)->getTiempo()-listaMuestras.at(inicio)->getTiempo());
    frecuenciaMuestreo/=(fin-inicio);
    frecuenciaMuestreo=1/frecuenciaMuestreo;
    const double tiempoTotal=listaMuestras.last()->getTiempo();
    const double tiempoAnalizado=listaMuestras.at(fin)->getTiempo()-listaMuestras.at(inicio)->getTiempo();

    MostrarCantidadDatos(datosTotales,datosAnalizados,frecuenciaMuestreo,tiempoTotal,tiempoAnalizado);
}

void AnalisisGrafico::MostrarCantidadDatos(const int datosTotales,const int datosAnalizados,const double frecuenciaMuestreo,const double tiempoTotal,const double tiempoAnalizado)
{
    ui->labelCantidadDatos->setText(QString("Datos totales: %1, Datos analizados actualmente: %2"
                                            "\nFrecuencia de muestreo obtenida %3 Hz"
                                            "\nTiempo Total: %4seg, intervalo de Tiempo: %5 seg")
                                            .arg(datosTotales)
                                            .arg(datosAnalizados)
                                            .arg(frecuenciaMuestreo)
                                            .arg(tiempoTotal)
                                            .arg(tiempoAnalizado));
}

void AnalisisGrafico::calcularEstadisticosAngulos(const int inicio, const int fin)
{
    double media1=0,media2=0,varian1=0,varian2=0,desvEst1=0,desvEst2=0;
    int terminos=fin+1-inicio;
    double menorX=listaAngulos.at(inicio)->getAngulo1(),menorY=listaAngulos.at(inicio)->getAngulo2();
    double mayorX=listaAngulos.at(inicio)->getAngulo1(),mayorY=listaAngulos.at(inicio)->getAngulo2();
    double tMenorX=listaAngulos.at(inicio)->getTiempo(),tMenorY=listaAngulos.at(inicio)->getTiempo();
    double tMayorX=listaAngulos.at(inicio)->getTiempo(),tMayorY=listaAngulos.at(inicio)->getTiempo();
    double rangoX=0,rangoY=0;

    for (int var = inicio; var <= fin; ++var) {
        Angulo *angulo=listaAngulos.at(var);
        media1+=angulo->getAngulo1();
        media2+=angulo->getAngulo2();

        if(menorX>angulo->getAngulo1()){
            menorX=angulo->getAngulo1();
            tMenorX=angulo->getTiempo();
        }
        if(menorY>angulo->getAngulo2()){
            menorY=angulo->getAngulo2();
            tMenorY=angulo->getTiempo();
        }
        if(mayorX<angulo->getAngulo1()){
            mayorX=angulo->getAngulo1();
            tMayorX=angulo->getTiempo();
        }
        if(mayorY<angulo->getAngulo2()){
            mayorY=angulo->getAngulo2();
            tMayorY=angulo->getTiempo();
        }
    }
    media1/=terminos;
    media2/=terminos;
    rangoX=mayorX-menorX;
    rangoY=mayorY-menorY;

    for (int var = inicio; var <= fin; ++var) {
        varian1+=qPow((listaAngulos.at(var)->getAngulo1()-media1),2);
        varian2+=qPow((listaAngulos.at(var)->getAngulo2()-media2),2);
    }
    varian1/=terminos;
    varian2/=terminos;
    desvEst1=qSqrt(varian1);
    desvEst2=qSqrt(varian2);

    ui->tableWidgetEstadisticos->clearContents();
    ui->tableWidgetEstadisticos->setRowCount(2);

    ui->tableWidgetEstadisticos->setItem(0,0,new QTableWidgetItem("Medio-Lateral"));
    ui->tableWidgetEstadisticos->setItem(1,0,new QTableWidgetItem("Antero-Posterior"));
    ui->tableWidgetEstadisticos->setItem(0,1,new QTableWidgetItem(QString::number(menorX)));
    ui->tableWidgetEstadisticos->setItem(1,1,new QTableWidgetItem(QString::number(menorY)));
    ui->tableWidgetEstadisticos->setItem(0,2,new QTableWidgetItem(QString::number(tMenorX)));
    ui->tableWidgetEstadisticos->setItem(1,2,new QTableWidgetItem(QString::number(tMenorY)));
    ui->tableWidgetEstadisticos->setItem(0,3,new QTableWidgetItem(QString::number(mayorX)));
    ui->tableWidgetEstadisticos->setItem(1,3,new QTableWidgetItem(QString::number(mayorY)));
    ui->tableWidgetEstadisticos->setItem(0,4,new QTableWidgetItem(QString::number(tMayorX)));
    ui->tableWidgetEstadisticos->setItem(1,4,new QTableWidgetItem(QString::number(tMayorY)));
    ui->tableWidgetEstadisticos->setItem(0,5,new QTableWidgetItem(QString::number(media1)));
    ui->tableWidgetEstadisticos->setItem(1,5,new QTableWidgetItem(QString::number(media2)));
    ui->tableWidgetEstadisticos->setItem(0,6,new QTableWidgetItem(QString::number(desvEst1)));
    ui->tableWidgetEstadisticos->setItem(1,6,new QTableWidgetItem(QString::number(desvEst2)));
    ui->tableWidgetEstadisticos->setItem(0,7,new QTableWidgetItem(QString::number(rangoX)));
    ui->tableWidgetEstadisticos->setItem(1,7,new QTableWidgetItem(QString::number(rangoY)));
}

void AnalisisGrafico::calcularEstadisticosDesplazamientoProyeccion(const int inicio, const int fin)
{
    double media1=0,media2=0,varian1=0,varian2=0,desvEst1=0,desvEst2=0;
    int terminos=fin+1-inicio;
    double menorX=listaDesplazamientos.at(inicio)->getDesplazamientoProyeccion().Desplazamiento1,menorY=listaDesplazamientos.at(inicio)->getDesplazamientoProyeccion().Desplazamiento2;
    double mayorX=listaDesplazamientos.at(inicio)->getDesplazamientoProyeccion().Desplazamiento1,mayorY=listaDesplazamientos.at(inicio)->getDesplazamientoProyeccion().Desplazamiento2;
    double tMenorX=listaDesplazamientos.at(inicio)->getTiempo(),tMenorY=listaDesplazamientos.at(inicio)->getTiempo();
    double tMayorX=listaDesplazamientos.at(inicio)->getTiempo(),tMayorY=listaDesplazamientos.at(inicio)->getTiempo();
    double rangoX=0,rangoY=0;
    double velMediaX=0,velMediaY=0;
    double deltaT=(listaDesplazamientos.at(fin)->getTiempo()-listaDesplazamientos.at(inicio)->getTiempo())/(fin-inicio);
    for (int var = inicio; var <= fin; ++var) {
        Desplazamiento *desplazamiento=listaDesplazamientos.at(var);
        media1+=desplazamiento->getDesplazamientoProyeccion().Desplazamiento1;
        media2+=desplazamiento->getDesplazamientoProyeccion().Desplazamiento2;

        if(menorX>desplazamiento->getDesplazamientoProyeccion().Desplazamiento1){
            menorX=desplazamiento->getDesplazamientoProyeccion().Desplazamiento1;
            tMenorX=desplazamiento->getTiempo();
        }
        if(menorY>desplazamiento->getDesplazamientoProyeccion().Desplazamiento2){
            menorY=desplazamiento->getDesplazamientoProyeccion().Desplazamiento2;
            tMenorY=desplazamiento->getTiempo();
        }
        if(mayorX<desplazamiento->getDesplazamientoProyeccion().Desplazamiento1){
            mayorX=desplazamiento->getDesplazamientoProyeccion().Desplazamiento1;
            tMayorX=desplazamiento->getTiempo();
        }
        if(mayorY<desplazamiento->getDesplazamientoProyeccion().Desplazamiento2){
            mayorY=desplazamiento->getDesplazamientoProyeccion().Desplazamiento2;
            tMayorY=desplazamiento->getTiempo();
        }

        if(var<fin){
            Desplazamiento *despb=listaDesplazamientos.at(var+1);
            const double deltaX=despb->getDesplazamientoProyeccion().Desplazamiento1 - desplazamiento->getDesplazamientoProyeccion().Desplazamiento1;
            const double deltaY=despb->getDesplazamientoProyeccion().Desplazamiento2 - desplazamiento->getDesplazamientoProyeccion().Desplazamiento2;
            velMediaX+=(deltaX/deltaT);
            velMediaY+=(deltaY/deltaT);
        }
    }
    media1/=terminos;
    media2/=terminos;
    rangoX=mayorX-menorX;
    rangoY=mayorY-menorY;
    velMediaX/=terminos-1;
    velMediaY/=terminos-1;

    for (int var = inicio; var <= fin; ++var) {
        varian1+=qPow((listaDesplazamientos.at(var)->getDesplazamientoProyeccion().Desplazamiento1-media1),2);
        varian2+=qPow((listaDesplazamientos.at(var)->getDesplazamientoProyeccion().Desplazamiento2-media2),2);
    }
    varian1/=terminos;
    varian2/=terminos;
    desvEst1=qSqrt(varian1);
    desvEst2=qSqrt(varian2);

    ui->tableWidgetEstadisticos->clearContents();
    ui->tableWidgetEstadisticos->setRowCount(2);

    ui->tableWidgetEstadisticos->setItem(0,0,new QTableWidgetItem("Medio-Lateral"));
    ui->tableWidgetEstadisticos->setItem(1,0,new QTableWidgetItem("Antero-Posterior"));
    ui->tableWidgetEstadisticos->setItem(0,1,new QTableWidgetItem(QString::number(menorX)));
    ui->tableWidgetEstadisticos->setItem(1,1,new QTableWidgetItem(QString::number(menorY)));
    ui->tableWidgetEstadisticos->setItem(0,2,new QTableWidgetItem(QString::number(tMenorX)));
    ui->tableWidgetEstadisticos->setItem(1,2,new QTableWidgetItem(QString::number(tMenorY)));
    ui->tableWidgetEstadisticos->setItem(0,3,new QTableWidgetItem(QString::number(mayorX)));
    ui->tableWidgetEstadisticos->setItem(1,3,new QTableWidgetItem(QString::number(mayorY)));
    ui->tableWidgetEstadisticos->setItem(0,4,new QTableWidgetItem(QString::number(tMayorX)));
    ui->tableWidgetEstadisticos->setItem(1,4,new QTableWidgetItem(QString::number(tMayorY)));
    ui->tableWidgetEstadisticos->setItem(0,5,new QTableWidgetItem(QString::number(media1)));
    ui->tableWidgetEstadisticos->setItem(1,5,new QTableWidgetItem(QString::number(media2)));
    ui->tableWidgetEstadisticos->setItem(0,6,new QTableWidgetItem(QString::number(desvEst1)));
    ui->tableWidgetEstadisticos->setItem(1,6,new QTableWidgetItem(QString::number(desvEst2)));
    ui->tableWidgetEstadisticos->setItem(0,7,new QTableWidgetItem(QString::number(rangoX)));
    ui->tableWidgetEstadisticos->setItem(1,7,new QTableWidgetItem(QString::number(rangoY)));
    ui->tableWidgetEstadisticos->setItem(0,8,new QTableWidgetItem(QString::number(velMediaX)));
    ui->tableWidgetEstadisticos->setItem(1,8,new QTableWidgetItem(QString::number(velMediaY)));
}

void AnalisisGrafico::calcularEstadisticosDesplazamientoRecorridoCurvo(const int inicio, const int fin)
{
    double media1=0,media2=0,varian1=0,varian2=0,desvEst1=0,desvEst2=0;
    int terminos=fin+1-inicio;
    double menorX=listaDesplazamientos.at(inicio)->getDesplazamientoRecorridoCurvo().Desplazamiento1,menorY=listaDesplazamientos.at(inicio)->getDesplazamientoRecorridoCurvo().Desplazamiento2;
    double mayorX=listaDesplazamientos.at(inicio)->getDesplazamientoRecorridoCurvo().Desplazamiento1,mayorY=listaDesplazamientos.at(inicio)->getDesplazamientoRecorridoCurvo().Desplazamiento2;
    double tMenorX=listaDesplazamientos.at(inicio)->getTiempo(),tMenorY=listaDesplazamientos.at(inicio)->getTiempo();
    double tMayorX=listaDesplazamientos.at(inicio)->getTiempo(),tMayorY=listaDesplazamientos.at(inicio)->getTiempo();
    double rangoX=0,rangoY=0;
    double velMediaX=0,velMediaY=0;
    double deltaT=(listaDesplazamientos.at(fin)->getTiempo()-listaDesplazamientos.at(inicio)->getTiempo())/(fin-inicio);
    for (int var = inicio; var <= fin; ++var) {
        Desplazamiento *desplazamiento=listaDesplazamientos.at(var);
        media1+=desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento1;
        media2+=desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento2;

        if(menorX>desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento1){
            menorX=desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento1;
            tMenorX=desplazamiento->getTiempo();
        }
        if(menorY>desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento2){
            menorY=desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento2;
            tMenorY=desplazamiento->getTiempo();
        }
        if(mayorX<desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento1){
            mayorX=desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento1;
            tMayorX=desplazamiento->getTiempo();
        }
        if(mayorY<desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento2){
            mayorY=desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento2;
            tMayorY=desplazamiento->getTiempo();
        }

        if(var<fin){
            Desplazamiento *despb=listaDesplazamientos.at(var+1);
            const double deltaX=despb->getDesplazamientoRecorridoCurvo().Desplazamiento1 - desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento1;
            const double deltaY=despb->getDesplazamientoRecorridoCurvo().Desplazamiento2 - desplazamiento->getDesplazamientoRecorridoCurvo().Desplazamiento2;
            velMediaX+=(deltaX/deltaT);
            velMediaY+=(deltaY/deltaT);
        }
    }
    media1/=terminos;
    media2/=terminos;
    rangoX=mayorX-menorX;
    rangoY=mayorY-menorY;
    velMediaX/=terminos-1;
    velMediaY/=terminos-1;

    for (int var = inicio; var <= fin; ++var) {
        varian1+=qPow((listaDesplazamientos.at(var)->getDesplazamientoRecorridoCurvo().Desplazamiento1-media1),2);
        varian2+=qPow((listaDesplazamientos.at(var)->getDesplazamientoRecorridoCurvo().Desplazamiento2-media2),2);
    }
    varian1/=terminos;
    varian2/=terminos;
    desvEst1=qSqrt(varian1);
    desvEst2=qSqrt(varian2);

    ui->tableWidgetEstadisticos->clearContents();
    ui->tableWidgetEstadisticos->setRowCount(2);

    ui->tableWidgetEstadisticos->setItem(0,0,new QTableWidgetItem("Medio-Lateral"));
    ui->tableWidgetEstadisticos->setItem(1,0,new QTableWidgetItem("Antero-Posterior"));
    ui->tableWidgetEstadisticos->setItem(0,1,new QTableWidgetItem(QString::number(menorX)));
    ui->tableWidgetEstadisticos->setItem(1,1,new QTableWidgetItem(QString::number(menorY)));
    ui->tableWidgetEstadisticos->setItem(0,2,new QTableWidgetItem(QString::number(tMenorX)));
    ui->tableWidgetEstadisticos->setItem(1,2,new QTableWidgetItem(QString::number(tMenorY)));
    ui->tableWidgetEstadisticos->setItem(0,3,new QTableWidgetItem(QString::number(mayorX)));
    ui->tableWidgetEstadisticos->setItem(1,3,new QTableWidgetItem(QString::number(mayorY)));
    ui->tableWidgetEstadisticos->setItem(0,4,new QTableWidgetItem(QString::number(tMayorX)));
    ui->tableWidgetEstadisticos->setItem(1,4,new QTableWidgetItem(QString::number(tMayorY)));
    ui->tableWidgetEstadisticos->setItem(0,5,new QTableWidgetItem(QString::number(media1)));
    ui->tableWidgetEstadisticos->setItem(1,5,new QTableWidgetItem(QString::number(media2)));
    ui->tableWidgetEstadisticos->setItem(0,6,new QTableWidgetItem(QString::number(desvEst1)));
    ui->tableWidgetEstadisticos->setItem(1,6,new QTableWidgetItem(QString::number(desvEst2)));
    ui->tableWidgetEstadisticos->setItem(0,7,new QTableWidgetItem(QString::number(rangoX)));
    ui->tableWidgetEstadisticos->setItem(1,7,new QTableWidgetItem(QString::number(rangoY)));
    ui->tableWidgetEstadisticos->setItem(0,8,new QTableWidgetItem(QString::number(velMediaX)));
    ui->tableWidgetEstadisticos->setItem(1,8,new QTableWidgetItem(QString::number(velMediaY)));
}


void AnalisisGrafico::calcularEstadisticosMuestras(const int inicio, const int fin)
{
    double mediaAcX=0,mediaAcY=0,mediaAcZ=0,mediaGyX=0,mediaGyY=0,mediaGyZ=0;
    double varianAcX=0,varianAcY=0,varianAcZ=0,varianGyX=0,varianGyY=0,varianGyZ=0;
    double desvEstAcX=0,desvEstAcY=0,desvEstAcZ=0,desvEstGyX=0,desvEstGyY=0,desvEstGyZ=0;
    double menorAcX=listaMuestras.at(inicio)->getAcX(),menorAcY=listaMuestras.at(inicio)->getAcY(),menorAcZ=listaMuestras.at(inicio)->getAcZ();
    double menorGyX=listaMuestras.at(inicio)->getGyX(),menorGyY=listaMuestras.at(inicio)->getGyY(),menorGyZ=listaMuestras.at(inicio)->getGyZ();
    double mayorAcX=listaMuestras.at(inicio)->getAcX(),mayorAcY=listaMuestras.at(inicio)->getAcY(),mayorAcZ=listaMuestras.at(inicio)->getAcZ();
    double mayorGyX=listaMuestras.at(inicio)->getGyX(),mayorGyY=listaMuestras.at(inicio)->getGyY(),mayorGyZ=listaMuestras.at(inicio)->getGyZ();
    double tMenorAcX=listaMuestras.at(inicio)->getTiempo(),tMenorAcY=listaMuestras.at(inicio)->getTiempo(),tMenorAcZ=listaMuestras.at(inicio)->getTiempo();
    double tMenorGyX=listaMuestras.at(inicio)->getTiempo(),tMenorGyY=listaMuestras.at(inicio)->getTiempo(),tMenorGyZ=listaMuestras.at(inicio)->getTiempo();
    double tMayorAcX=listaMuestras.at(inicio)->getTiempo(),tMayorAcY=listaMuestras.at(inicio)->getTiempo(),tMayorAcZ=listaMuestras.at(inicio)->getTiempo();
    double tMayorGyX=listaMuestras.at(inicio)->getTiempo(),tMayorGyY=listaMuestras.at(inicio)->getTiempo(),tMayorGyZ=listaMuestras.at(inicio)->getTiempo();
    double rangoAcX=0,rangoAcY=0,rangoAcZ=0,rangoGyX=0,rangoGyY=0,rangoGyZ=0;
    int terminos=fin+1-inicio;
    for (int var = inicio; var <= fin; ++var) {
        Muestra *raw=listaMuestras.at(var);
        mediaAcX+=raw->getAcX();
        mediaAcY+=raw->getAcY();
        mediaAcZ+=raw->getAcZ();
        mediaGyX+=raw->getGyX();
        mediaGyY+=raw->getGyY();
        mediaGyZ+=raw->getGyZ();

        if(menorAcX>raw->getAcX()){
            menorAcX=raw->getAcX();
            tMenorAcX=raw->getTiempo();
        }
        if(menorAcY>raw->getAcY()){
            menorAcY=raw->getAcY();
            tMenorAcY=raw->getTiempo();
        }
        if(menorAcZ>raw->getAcZ()){
            menorAcZ=raw->getAcZ();
            tMenorAcZ=raw->getTiempo();
        }
        if(menorGyX>raw->getGyX()){
            menorGyX=raw->getGyX();
            tMenorGyX=raw->getTiempo();
        }
        if(menorGyY>raw->getGyY()){
            menorGyY=raw->getGyY();
            tMenorGyY=raw->getTiempo();
        }
        if(menorGyZ>raw->getGyZ()){
            menorGyZ=raw->getGyZ();
            tMenorGyZ=raw->getTiempo();
        }
        if(mayorAcX<raw->getAcX()){
            mayorAcX=raw->getAcX();
            tMayorAcX=raw->getTiempo();
        }
        if(mayorAcY<raw->getAcY()){
            mayorAcY=raw->getAcY();
            tMayorAcY=raw->getTiempo();
        }
        if(mayorAcZ<raw->getAcZ()){
            mayorAcZ=raw->getAcZ();
            tMayorAcZ=raw->getTiempo();
        }
        if(mayorGyX<raw->getGyX()){
            mayorGyX=raw->getGyX();
            tMayorGyX=raw->getTiempo();
        }
        if(mayorGyY<raw->getGyY()){
            mayorGyY=raw->getGyY();
            tMayorGyY=raw->getTiempo();
        }
        if(mayorGyZ<raw->getGyZ()){
            mayorGyZ=raw->getGyZ();
            tMayorGyZ=raw->getTiempo();
        }
    }

    mediaAcX/=terminos;
    mediaAcY/=terminos;
    mediaAcZ/=terminos;
    mediaGyX/=terminos;
    mediaGyY/=terminos;
    mediaGyZ/=terminos;

    rangoAcX=mayorAcX-menorAcX;
    rangoAcY=mayorAcY-menorAcY;
    rangoAcZ=mayorAcZ-menorAcZ;
    rangoGyX=mayorGyX-menorGyX;
    rangoGyY=mayorGyY-menorGyY;
    rangoGyZ=mayorGyZ-menorGyZ;

    for (int var = inicio; var <= fin; ++var) {
        varianAcX+=qPow((listaMuestras.at(var)->getAcX()-mediaAcX),2);
        varianAcY+=qPow((listaMuestras.at(var)->getAcY()-mediaAcY),2);
        varianAcZ+=qPow((listaMuestras.at(var)->getAcZ()-mediaAcZ),2);
        varianGyX+=qPow((listaMuestras.at(var)->getGyX()-mediaGyX),2);
        varianGyY+=qPow((listaMuestras.at(var)->getGyY()-mediaGyY),2);
        varianGyZ+=qPow((listaMuestras.at(var)->getGyZ()-mediaGyZ),2);
    }
    varianAcX/=terminos;
    varianAcY/=terminos;
    varianAcZ/=terminos;
    varianGyX/=terminos;
    varianGyY/=terminos;
    varianGyZ/=terminos;

    desvEstAcX=qSqrt(varianAcX);
    desvEstAcY=qSqrt(varianAcY);
    desvEstAcZ=qSqrt(varianAcZ);
    desvEstGyX=qSqrt(varianGyX);
    desvEstGyY=qSqrt(varianGyY);
    desvEstGyZ=qSqrt(varianGyZ);

    ui->tableWidgetEstadisticos->clearContents();
    ui->tableWidgetEstadisticos->setRowCount(6);

    ui->tableWidgetEstadisticos->setItem(0,0,new QTableWidgetItem("AcX"));
    ui->tableWidgetEstadisticos->setItem(1,0,new QTableWidgetItem("AcY"));
    ui->tableWidgetEstadisticos->setItem(2,0,new QTableWidgetItem("AcZ"));
    ui->tableWidgetEstadisticos->setItem(3,0,new QTableWidgetItem("GyX"));
    ui->tableWidgetEstadisticos->setItem(4,0,new QTableWidgetItem("GyY"));
    ui->tableWidgetEstadisticos->setItem(5,0,new QTableWidgetItem("GyZ"));

    ui->tableWidgetEstadisticos->setItem(0,1,new QTableWidgetItem(QString::number(menorAcX)));
    ui->tableWidgetEstadisticos->setItem(1,1,new QTableWidgetItem(QString::number(menorAcY)));
    ui->tableWidgetEstadisticos->setItem(2,1,new QTableWidgetItem(QString::number(menorAcZ)));
    ui->tableWidgetEstadisticos->setItem(3,1,new QTableWidgetItem(QString::number(menorGyX)));
    ui->tableWidgetEstadisticos->setItem(4,1,new QTableWidgetItem(QString::number(menorGyY)));
    ui->tableWidgetEstadisticos->setItem(5,1,new QTableWidgetItem(QString::number(menorGyZ)));

    ui->tableWidgetEstadisticos->setItem(0,2,new QTableWidgetItem(QString::number(tMenorAcX)));
    ui->tableWidgetEstadisticos->setItem(1,2,new QTableWidgetItem(QString::number(tMenorAcY)));
    ui->tableWidgetEstadisticos->setItem(2,2,new QTableWidgetItem(QString::number(tMenorAcZ)));
    ui->tableWidgetEstadisticos->setItem(3,2,new QTableWidgetItem(QString::number(tMenorGyX)));
    ui->tableWidgetEstadisticos->setItem(4,2,new QTableWidgetItem(QString::number(tMenorGyY)));
    ui->tableWidgetEstadisticos->setItem(5,2,new QTableWidgetItem(QString::number(tMenorGyZ)));

    ui->tableWidgetEstadisticos->setItem(0,3,new QTableWidgetItem(QString::number(mayorAcX)));
    ui->tableWidgetEstadisticos->setItem(1,3,new QTableWidgetItem(QString::number(mayorAcY)));
    ui->tableWidgetEstadisticos->setItem(2,3,new QTableWidgetItem(QString::number(mayorAcZ)));
    ui->tableWidgetEstadisticos->setItem(3,3,new QTableWidgetItem(QString::number(mayorGyX)));
    ui->tableWidgetEstadisticos->setItem(4,3,new QTableWidgetItem(QString::number(mayorGyY)));
    ui->tableWidgetEstadisticos->setItem(5,3,new QTableWidgetItem(QString::number(mayorGyZ)));

    ui->tableWidgetEstadisticos->setItem(0,4,new QTableWidgetItem(QString::number(tMayorAcX)));
    ui->tableWidgetEstadisticos->setItem(1,4,new QTableWidgetItem(QString::number(tMayorAcY)));
    ui->tableWidgetEstadisticos->setItem(2,4,new QTableWidgetItem(QString::number(tMayorAcZ)));
    ui->tableWidgetEstadisticos->setItem(3,4,new QTableWidgetItem(QString::number(tMayorGyX)));
    ui->tableWidgetEstadisticos->setItem(4,4,new QTableWidgetItem(QString::number(tMayorGyY)));
    ui->tableWidgetEstadisticos->setItem(5,4,new QTableWidgetItem(QString::number(tMayorGyZ)));

    ui->tableWidgetEstadisticos->setItem(0,5,new QTableWidgetItem(QString::number(mediaAcX)));
    ui->tableWidgetEstadisticos->setItem(1,5,new QTableWidgetItem(QString::number(mediaAcY)));
    ui->tableWidgetEstadisticos->setItem(2,5,new QTableWidgetItem(QString::number(mediaAcZ)));
    ui->tableWidgetEstadisticos->setItem(3,5,new QTableWidgetItem(QString::number(mediaGyX)));
    ui->tableWidgetEstadisticos->setItem(4,5,new QTableWidgetItem(QString::number(mediaGyY)));
    ui->tableWidgetEstadisticos->setItem(5,5,new QTableWidgetItem(QString::number(mediaGyZ)));

    ui->tableWidgetEstadisticos->setItem(0,6,new QTableWidgetItem(QString::number(desvEstAcX)));
    ui->tableWidgetEstadisticos->setItem(1,6,new QTableWidgetItem(QString::number(desvEstAcY)));
    ui->tableWidgetEstadisticos->setItem(2,6,new QTableWidgetItem(QString::number(desvEstAcZ)));
    ui->tableWidgetEstadisticos->setItem(3,6,new QTableWidgetItem(QString::number(desvEstGyX)));
    ui->tableWidgetEstadisticos->setItem(4,6,new QTableWidgetItem(QString::number(desvEstGyY)));
    ui->tableWidgetEstadisticos->setItem(5,6,new QTableWidgetItem(QString::number(desvEstGyZ)));

    ui->tableWidgetEstadisticos->setItem(0,7,new QTableWidgetItem(QString::number(rangoAcX)));
    ui->tableWidgetEstadisticos->setItem(1,7,new QTableWidgetItem(QString::number(rangoAcY)));
    ui->tableWidgetEstadisticos->setItem(2,7,new QTableWidgetItem(QString::number(rangoAcZ)));
    ui->tableWidgetEstadisticos->setItem(3,7,new QTableWidgetItem(QString::number(rangoGyX)));
    ui->tableWidgetEstadisticos->setItem(4,7,new QTableWidgetItem(QString::number(rangoGyY)));
    ui->tableWidgetEstadisticos->setItem(5,7,new QTableWidgetItem(QString::number(rangoGyZ)));
}

QImage AnalisisGrafico::obtenerImagenTablaEstadisticos()
{
    return ui->tableWidgetEstadisticos->grab().toImage();
}
