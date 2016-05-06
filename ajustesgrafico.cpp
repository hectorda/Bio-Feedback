#include "ajustesgrafico.h"
#include "ui_ajustesgrafico.h"

AjustesGrafico::AjustesGrafico(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjustesGrafico)
{
    inicializar();
    conexiones();
    aplicar();
    graficarObjetivo(ajustesActuales.RadioObjetivo);
    graficarRepresentacionGrafico(ajustesActuales.RadioInterior,ajustesActuales.RadioExterior);

}

AjustesGrafico::~AjustesGrafico()
{
    delete ui;
}

AjustesGrafico::Ajustes AjustesGrafico::getAjustes()
{
    return ajustesActuales;
}

void AjustesGrafico::inicializar()
{
    ui->setupUi(this);
    objetivo=new QCPItemEllipse(ui->qCustomPlotObjetivo);
    circuloExterior= new QCPItemEllipse(ui->qCustomRepresentacionGrafico);
    circuloInterior= new QCPItemEllipse(ui->qCustomRepresentacionGrafico);
}

void AjustesGrafico::conexiones()
{
    connect(ui->pushButtonCancelar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pushButtonOK,SIGNAL(clicked()),this,SLOT(aplicar()));
}

void AjustesGrafico::aplicar()
{
    ajustesActuales.RadioInterior=ui->spinBoxRInterior->value();
    ajustesActuales.RadioExterior=ui->spinBoxRExterior->value();
    ajustesActuales.RadioObjetivo=ui->spinBoxRObjetivo->value();
    hide();
}

void AjustesGrafico::graficarObjetivo(const int rObjetivo){

    objetivo->topLeft->setCoords(-rObjetivo,rObjetivo);
    objetivo->bottomRight->setCoords(rObjetivo,-rObjetivo);
    objetivo->setBrush(QBrush(Qt::red));
    ui->qCustomPlotObjetivo->replot();

}

void AjustesGrafico::relacionAspecto(QCustomPlot *grafico)
{
    const int w=grafico->width();
    const int h=grafico->height();
    const QRect rect=grafico->geometry();

    if(w>h)
        grafico->setGeometry(rect.x()+((w-h)/2),rect.y(),h,h);
    else
        grafico->setGeometry(rect.x(),rect.y()+((h-w)/2),w,w);

}

void AjustesGrafico::graficarRepresentacionGrafico(const int rInterior,const int rExterior)
{
    ui->qCustomRepresentacionGrafico->clearFocus();
    ui->qCustomRepresentacionGrafico->clearGraphs();
    ui->qCustomRepresentacionGrafico->clearItems();
    ui->qCustomRepresentacionGrafico->clearPlottables();
    circuloExterior->topLeft->setCoords(-rExterior,rExterior);
    circuloExterior->bottomRight->setCoords(rExterior,-rExterior);
    circuloExterior->setBrush(QBrush(Qt::yellow));

    circuloInterior->topLeft->setCoords(-rInterior,rInterior);
    circuloInterior->bottomRight->setCoords(rInterior,-rInterior);

    const int range=rExterior+10;
    ui->qCustomRepresentacionGrafico->xAxis->setRange(-range,range);
    ui->qCustomRepresentacionGrafico->yAxis->setRange(-range,range);
    //Para que sea representado en los mismos rangos el grafico del objetivo
    ui->qCustomPlotObjetivo->xAxis->setRange(-range,range);
    ui->qCustomPlotObjetivo->yAxis->setRange(-range,range);
    ui->qCustomPlotObjetivo->replot();

    ui->qCustomRepresentacionGrafico->replot();
}

bool AjustesGrafico::event(QEvent *event)
{
    if (event->type() == QEvent::Resize) {
        relacionAspecto(ui->qCustomPlotObjetivo);
        relacionAspecto(ui->qCustomRepresentacionGrafico);
    }

    if (event->type() == QEvent::Show){
        relacionAspecto(ui->qCustomPlotObjetivo);
        relacionAspecto(ui->qCustomRepresentacionGrafico);
    }

    return QWidget::event(event);
}

void AjustesGrafico::on_spinBoxRObjetivo_valueChanged(int arg1)
{
    graficarObjetivo(arg1);
}

void AjustesGrafico::on_spinBoxRExterior_valueChanged(int arg1)
{
    const int rInterior=ui->spinBoxRInterior->value();
    graficarRepresentacionGrafico(rInterior,arg1);
}

void AjustesGrafico::on_spinBoxRInterior_valueChanged(int arg1)
{
    const int rEnterior=ui->spinBoxRExterior->value();
    graficarRepresentacionGrafico(arg1,rEnterior);
}
