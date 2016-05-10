#include "ajustesgrafico.h"
#include "ui_ajustesgrafico.h"

AjustesGrafico::AjustesGrafico(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjustesGrafico)
{
    inicializar();
    conexiones();
    llenarParametrosComboBox();
    aplicar();
    graficarObjetivo(ajustesActuales.RadioObjetivo,ajustesActuales.colorObjetivoSinMarcar);
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

void AjustesGrafico::llenarParametrosComboBox()
{
    QStringList colorNames = QColor::colorNames();

      for (int i = 0; i < colorNames.size(); ++i) {
          QColor color(colorNames[i]);

          ui->comboBoxColorObjetivosSinMarcar->insertItem(i, colorNames[i]);
          ui->comboBoxColorObjetivosSinMarcar->setItemData(i, color, Qt::DecorationRole);

          ui->comboBoxColorObjetivosMarcados->insertItem(i, colorNames[i]);
          ui->comboBoxColorObjetivosMarcados->setItemData(i, color, Qt::DecorationRole);
      }
      ui->comboBoxColorObjetivosSinMarcar->setCurrentIndex(119); //Se setea el color rojo
      ui->comboBoxColorObjetivosMarcados->setCurrentIndex(54);   //Se setea el color verde
}

void AjustesGrafico::aplicar()
{
    if(ui->comboBoxColorObjetivosMarcados->currentIndex()!=ui->comboBoxColorObjetivosSinMarcar->currentIndex()){
        ajustesActuales.RadioInterior=ui->spinBoxRInterior->value();
        ajustesActuales.RadioExterior=ui->spinBoxRExterior->value();
        ajustesActuales.RadioObjetivo=ui->spinBoxRObjetivo->value();
        hide();
        const QColor colorSinMarcar=qvariant_cast<QColor>(ui->comboBoxColorObjetivosSinMarcar->itemData(ui->comboBoxColorObjetivosSinMarcar->currentIndex(), Qt::DecorationRole));
        const QColor colorMarcado=qvariant_cast<QColor>(ui->comboBoxColorObjetivosMarcados->itemData(ui->comboBoxColorObjetivosMarcados->currentIndex(), Qt::DecorationRole));
        ajustesActuales.colorObjetivoSinMarcar = colorSinMarcar;
        ajustesActuales.colorObjetivoMarcado = colorMarcado;

        //QTextStream stdout <<ui->comboBoxColorObjetivosSinMarcar->currentIndex()<<sinMarcar.name()<<" "<<ui->comboBoxColorObjetivosMarcados->currentIndex()<<marcado.name()<<endl;
    }
    else
        QMessageBox::critical(0,"Error al seleccionar Colores","Debe seleccionar colores diferentes para los objetivos");
}

void AjustesGrafico::graficarObjetivo(const int rObjetivo,const QColor color){

    objetivo->topLeft->setCoords(-rObjetivo,rObjetivo);
    objetivo->bottomRight->setCoords(rObjetivo,-rObjetivo);
    objetivo->setBrush(QBrush(color));
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
    const QColor sinMarcar=qvariant_cast<QColor>(ui->comboBoxColorObjetivosSinMarcar->itemData(ui->comboBoxColorObjetivosSinMarcar->currentIndex(), Qt::DecorationRole));
    graficarObjetivo(arg1,sinMarcar);
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

void AjustesGrafico::on_comboBoxColorObjetivosSinMarcar_currentIndexChanged(int index)
{
    const QColor sinMarcar=qvariant_cast<QColor>(ui->comboBoxColorObjetivosSinMarcar->itemData(index, Qt::DecorationRole));
    graficarObjetivo(ui->spinBoxRObjetivo->value(),sinMarcar);
}
